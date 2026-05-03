#include "eink.h"
#include "eink_port.h"

// Gửi lệnh (DC = 0)
static void Eink_SendCmd(uint8_t cmd) {
    Eink_Port_Set_DC(0);
    Eink_Port_Set_CS(0);
    Eink_Port_SpiWrite(&cmd, 1);
    Eink_Port_Set_CS(1);
}

// Gửi dữ liệu (DC = 1)
static void Eink_SendData(uint8_t data) {
    Eink_Port_Set_DC(1);
    Eink_Port_Set_CS(0);
    Eink_Port_SpiWrite(&data, 1);
    Eink_Port_Set_CS(1);
}

// Chờ màn hình rảnh (Busy = 1 theo đặc tả của SSD1680)
static void Eink_WaitBusy(void) {
    // Chờ cho đến khi chân BUSY xuống 0
    while(Eink_Port_Get_BUSY() == 1) {
        Eink_Port_Delay(5); 
    }
}

// Hardware Reset
static void Eink_Reset(void) {
    Eink_Port_Set_RST(0);
    Eink_Port_Delay(20);
    Eink_Port_Set_RST(1);
    Eink_Port_Delay(20);
}

// Thiết lập vùng cập nhật RAM
static void Eink_SetWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    Eink_SendCmd(0x44); // Lệnh cấu hình giới hạn trục X (tính theo byte)
    Eink_SendData(x / 8);
    Eink_SendData((x + w - 1) / 8);
    
    Eink_SendCmd(0x45); // Lệnh cấu hình giới hạn trục Y (tính theo pixel)
    Eink_SendData(y % 256);
    Eink_SendData(y / 256);
    Eink_SendData((y + h - 1) % 256);
    Eink_SendData((y + h - 1) / 256);
    
    Eink_SendCmd(0x4E); // Đặt con trỏ X về điểm xuất phát của cửa sổ
    Eink_SendData(x / 8);
    
    Eink_SendCmd(0x4F); // Đặt con trỏ Y về điểm xuất phát của cửa sổ
    Eink_SendData(y % 256);
    Eink_SendData(y / 256);
}

// Hàm khởi tạo
void Eink_Init(void *spi_handle) {
    Eink_Port_Init(spi_handle);
    Eink_Reset();
    
    Eink_SendCmd(0x12);  // SWRESET
    Eink_WaitBusy();     // Chờ reset xong (khác với delay thuần túy)
    
    Eink_SendCmd(0x01);  // Driver output control
    Eink_SendData(0xF9); // (250-1)
    Eink_SendData(0x00);
    Eink_SendData(0x00);
    
    Eink_SendCmd(0x11);  // Data entry mode
    Eink_SendData(0x03); // Y increment, X increment
    
    Eink_SendCmd(0x3C);  // BorderWaveform
    Eink_SendData(0x05);
    
    Eink_SendCmd(0x18);  // Read built-in temperature sensor
    Eink_SendData(0x80);
    
    Eink_SendCmd(0x21);  // Display update control
    Eink_SendData(0x00);
    Eink_SendData(0x80);
}

// Kích hoạt Update
void Eink_UpdateDisplay(void) {
    Eink_SendCmd(0x22);  // Display Update Control
    Eink_SendData(0xF7); // Mode full refresh
    Eink_SendCmd(0x20);  // Activate Display Update Sequence
    Eink_WaitBusy();
}

// Đưa vào chế độ ngủ sâu
void Eink_Sleep(void) {
    Eink_SendCmd(0x10); // deep sleep mode
    Eink_SendData(0x01); // enter deep sleep
}

void Eink_Clear(void) {
    Eink_SetWindow(0, 0, EINK_WIDTH, EINK_HEIGHT);
    
    Eink_SendCmd(0x24); // RAM Đen/Trắng
    for (uint32_t i = 0; i < (EINK_WIDTH * EINK_HEIGHT / 8); i++) {
        Eink_SendData(0xFF); // 1 = Trắng
    }
    
    Eink_SendCmd(0x26); // RAM Đỏ
    for (uint32_t i = 0; i < (EINK_WIDTH * EINK_HEIGHT / 8); i++) {
        Eink_SendData(0x00); // 0 = Không Đỏ
    }
    Eink_UpdateDisplay();
}

void Eink_WriteBuffer(uint8_t *black_buffer, uint8_t *red_buffer, uint16_t w, uint16_t h) {
    Eink_SetWindow(0, 0, w, h);
    
    Eink_SendCmd(0x24); // Đẩy mảng Đen/Trắng
    for(uint32_t i = 0; i < (w * h / 8); i++) {
        Eink_SendData(black_buffer[i]);
    }
    
    Eink_SendCmd(0x26); // Đẩy mảng Đỏ
    for(uint32_t i = 0; i < (w * h / 8); i++) {
        Eink_SendData(red_buffer[i]);
    }
}

void Eink_WriteWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *black_buffer, uint8_t *red_buffer) {
    // 1. Set khung cửa sổ
    Eink_SetWindow(x, y, w, h);
    
    // 2. Tính toán tổng số byte cần truyền cho khu vực này
    uint32_t byte_count = (w * h) / 8;
    
    // 3. Ghi dữ liệu vào Black/White RAM (Lệnh 0x24)
    // Eink_SendCmd(0x24);
    // for(uint32_t i = 0; i < byte_count; i++) {
    //     Eink_SendData(black_buffer[i]);
    // }
    
    // 4. Ghi dữ liệu vào Red RAM (Lệnh 0x26)
    // Eink_SendCmd(0x26);
    // for(uint32_t i = 0; i < byte_count; i++) {
    //     Eink_SendData(red_buffer[i]);
    // }

    // 3. Ghi dữ liệu vào Black/White RAM (Lệnh 0x24)
    Eink_SendCmd(0x24);
    Eink_Port_Set_CS(0);
    Eink_Port_Set_DC(1);
    Eink_Port_SpiWrite_DMA(black_buffer, byte_count);
    Eink_Port_Set_CS(1);
    
    // 4. Ghi dữ liệu vào Red RAM (Lệnh 0x26)
    Eink_SendCmd(0x26);
    Eink_Port_Set_CS(0); 
    Eink_Port_Set_DC(1);
    Eink_Port_SpiWrite_DMA(red_buffer, byte_count);
    Eink_Port_Set_CS(1);
}