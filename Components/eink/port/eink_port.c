#include "eink_port.h"
#include "main.h" // Chứa các macro EINK_CS_Pin, EINK_DC_Pin do CubeMX sinh ra

static SPI_HandleTypeDef *eink_spi = NULL;

void Eink_Port_Init(void *spi_handle) {
    eink_spi = (SPI_HandleTypeDef *)spi_handle;
}

void Eink_Port_Delay(uint32_t ms) {
    HAL_Delay(ms);
}

void Eink_Port_Set_CS(uint8_t state) {
    HAL_GPIO_WritePin(EINK_CS_GPIO_Port, EINK_CS_Pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void Eink_Port_Set_DC(uint8_t state) {
    HAL_GPIO_WritePin(EINK_DC_GPIO_Port, EINK_DC_Pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void Eink_Port_Set_RST(uint8_t state) {
    HAL_GPIO_WritePin(EINK_RST_GPIO_Port, EINK_RST_Pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

uint8_t Eink_Port_Get_BUSY(void) {
    return (HAL_GPIO_ReadPin(EINK_BUSY_GPIO_Port, EINK_BUSY_Pin) == GPIO_PIN_SET) ? 1 : 0;
}

void Eink_Port_SpiWrite(uint8_t *data, uint16_t len) {
    if (eink_spi != NULL) {
        HAL_SPI_Transmit(eink_spi, data, len, HAL_MAX_DELAY);
    }
}

void Eink_Port_SpiWrite_DMA(uint8_t *data, uint16_t len) {
    if (eink_spi != NULL) {
        HAL_SPI_Transmit_DMA(eink_spi, data, len);
        while (HAL_SPI_GetState(eink_spi) != HAL_SPI_STATE_READY) {
        }
    }
}