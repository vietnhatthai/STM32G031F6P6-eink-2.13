#ifndef EINK_PORT_H
#define EINK_PORT_H

#include <stdint.h>

void Eink_Port_Init(void *spi_handle);
void Eink_Port_Delay(uint32_t ms);
void Eink_Port_Set_CS(uint8_t state);
void Eink_Port_Set_DC(uint8_t state);
void Eink_Port_Set_RST(uint8_t state);
uint8_t Eink_Port_Get_BUSY(void);
void Eink_Port_SpiWrite(uint8_t *data, uint16_t len);
void Eink_Port_SpiWrite_DMA(uint8_t *data, uint16_t len);

#endif