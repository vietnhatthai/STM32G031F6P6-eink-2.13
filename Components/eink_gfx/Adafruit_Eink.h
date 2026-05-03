#ifndef ADAFRUIT_EINK_H
#define ADAFRUIT_EINK_H

#include "Adafruit_GFX.h"

extern "C" {
    #include "eink.h"
}

// Kích thước vật lý của màn hình 2.13 inch SSD1680
#define EINK_WIDTH      122
#define EINK_HEIGHT     250

#define EINK_PAGE_HEIGHT    50
#define EINK_PAGES          (EINK_HEIGHT / EINK_PAGE_HEIGHT) // 250/50 = 5
#define EINK_BUFFER_WIDTH_BYTES ((EINK_WIDTH + 7) / 8) 
#define EINK_BUFFER_SIZE    (EINK_BUFFER_WIDTH_BYTES * EINK_PAGE_HEIGHT)

// Định nghĩa màu sắc
#define COLOR_BLACK  0x0000
#define COLOR_WHITE  0xFFFF
#define COLOR_RED    0xF800

class Adafruit_Eink : public Adafruit_GFX {
private:
    uint8_t black_buffer[EINK_BUFFER_SIZE];
    uint8_t red_buffer[EINK_BUFFER_SIZE];
    uint8_t current_page;

public:
    Adafruit_Eink();
    
    void begin(void *spi_handle);
    void display(void);
    void clearBuffer(void);
    void sleep(void);

    // BẮT BUỘC TỪ ADAFRUIT_GFX: Hàm vẽ 1 điểm ảnh
    void drawPixel(int16_t x, int16_t y, uint16_t color) override;

    void firstPage(void);
    bool nextPage(void);
};

#endif // ADAFRUIT_EINK_H