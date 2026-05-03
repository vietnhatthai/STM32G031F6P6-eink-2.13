#include "Adafruit_Eink.h"
#include <string.h>

Adafruit_Eink::Adafruit_Eink() : Adafruit_GFX(EINK_WIDTH, EINK_HEIGHT) {
    clearBuffer(); // Đảm bảo buffer trống (màu trắng) khi khởi động
}

void Adafruit_Eink::begin(void *spi_handle) {
    Eink_Init(spi_handle);
}

void Adafruit_Eink::clearBuffer(void) {
    memset(black_buffer, 0xFF, EINK_BUFFER_SIZE); // 0xFF (Toàn 1) = Trắng
    memset(red_buffer, 0x00, EINK_BUFFER_SIZE);   // 0x00 (Toàn 0) = Không Đỏ
}

void Adafruit_Eink::display(void) {
    // Lưu ý: SSD1680 nhận thông số chiều rộng theo byte (128 bits = 16 bytes)
    // Dù kích thước hiển thị là 122, nhưng ta phải gửi 128 cho phần cứng
    Eink_WriteBuffer(black_buffer, red_buffer, EINK_BUFFER_WIDTH_BYTES * 8, EINK_HEIGHT);
    Eink_UpdateDisplay();
}

void Adafruit_Eink::sleep(void) {
    Eink_Sleep();
}

void Adafruit_Eink::drawPixel(int16_t x, int16_t y, uint16_t color) {
    // 1. Xử lý xoay màn hình (Tính năng tích hợp của Adafruit GFX)
    // Tùy thuộc vào lệnh display.setRotation(0, 1, 2, 3)
    switch (getRotation()) {
        case 1: // Xoay 90 độ
            _swap_int16_t(x, y);
            x = EINK_WIDTH - x - 1;
            break;
        case 2: // Xoay 180 độ
            x = EINK_WIDTH - x - 1;
            y = EINK_HEIGHT - y - 1;
            break;
        case 3: // Xoay 270 độ
            _swap_int16_t(x, y);
            y = EINK_HEIGHT - y - 1;
            break;
    }

    // 2. Chặn các điểm ảnh vượt quá giới hạn vật lý
    if ((x < 0) || (x >= EINK_WIDTH) || (y < 0) || (y >= EINK_HEIGHT)) return;

    int16_t page_y_start = current_page * EINK_PAGE_HEIGHT;
    int16_t page_y_end = page_y_start + EINK_PAGE_HEIGHT;

    // Nếu điểm ảnh không nằm trong trang này -> Bỏ qua không vẽ
    if (y < page_y_start || y >= page_y_end) return;

    // Tính toán index dựa trên tọa độ y đã được trừ đi offset của trang
    uint16_t y_in_page = y - page_y_start;
    uint16_t byte_idx = (y_in_page * EINK_BUFFER_WIDTH_BYTES) + (x / 8);
    uint8_t bit_idx = 7 - (x % 8);

    // Mặc định luôn set pixel đó về màu Trắng trước khi đè màu mới
    black_buffer[byte_idx] |= (1 << bit_idx);  // Set bit lên 1 (Trắng)
    red_buffer[byte_idx]   &= ~(1 << bit_idx); // Kéo bit về 0 (Không đỏ)

    if (color == COLOR_BLACK) {
        black_buffer[byte_idx] &= ~(1 << bit_idx); // 0 = Đen
    } 
    else if (color == COLOR_RED) {
        red_buffer[byte_idx] |= (1 << bit_idx);
    }
}

void Adafruit_Eink::firstPage(void) {
    current_page = 0;
    clearBuffer(); // Xóa RAM nội bộ
}

bool Adafruit_Eink::nextPage(void) {
    // 1. Gửi trang hiện tại xuống E-ink
    uint16_t y_start = current_page * EINK_PAGE_HEIGHT;
    Eink_WriteWindow(0, y_start, EINK_WIDTH, EINK_PAGE_HEIGHT, black_buffer, red_buffer);
    
    // 2. Chuyển sang trang tiếp theo
    current_page++;
    
    // 3. Nếu đã vẽ xong tất cả các trang
    if (current_page >= EINK_PAGES) {
        Eink_UpdateDisplay(); // Gọi lệnh Refresh toàn màn hình 1 lần duy nhất ở đây
        return false;         // Kết thúc vòng lặp
    }
    
    // 4. Chuẩn bị buffer trắng cho trang tiếp theo
    clearBuffer();
    return true;
}