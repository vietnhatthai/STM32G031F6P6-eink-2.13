#ifndef EINK_H
#define EINK_H

#include <stdint.h>

// Kích thước chuẩn của màn hình 2.13 inch (GDEY0213Z98 - SSD1680)
#define EINK_WIDTH   122
#define EINK_HEIGHT  250

// Các hàm API người dùng gọi
void Eink_Init(void *spi_handle);
void Eink_Sleep(void);

// Các hàm thao tác hiển thị
void Eink_Clear(void);
void Eink_UpdateDisplay(void);
void Eink_WriteBuffer(uint8_t *black_buffer, uint8_t *red_buffer, uint16_t w, uint16_t h);
void Eink_WriteWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *black_buffer, uint8_t *red_buffer);

#endif