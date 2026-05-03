#ifndef WPROGRAM_H
#define WPROGRAM_H

#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// 1. Đánh lừa Adafruit_GFX rằng đây là Arduino bản mới (>= 1.0)
// Để nó đồng bộ kiểu trả về size_t cho hàm write()
#define ARDUINO 100

// 2. Giả lập hàm toán học của Arduino
#ifndef radians
#define radians(deg) ((deg) * 3.14159265359 / 180.0)
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

// 3. Giả lập macro đọc Flash (PROGMEM)
#define PROGMEM
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define pgm_read_word(addr) (*(const uint16_t *)(addr))
#define pgm_read_dword(addr) (*(const uint32_t *)(addr))
#define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))

// Giả lập kiểu dữ liệu chuỗi trên Flash của Arduino
class __FlashStringHelper;
#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(string_literal))

// 4. Giả lập class String cơ bản của Arduino
// (Chỉ cần đủ các hàm length() và c_str() để compiler không báo lỗi)
class String {
private:
    const char* _str;
public:
    String(const char* s = "") : _str(s) {}
    size_t length() const { return _str ? strlen(_str) : 0; }
    const char* c_str() const { return _str; }
};

// 5. Giả lập class Print để dùng được hàm display.print()
class Print {
public:
    virtual size_t write(uint8_t) = 0;
    
    virtual size_t write(const uint8_t *buffer, size_t size) {
        size_t n = 0;
        while (size--) {
            write(*buffer++);
            n++;
        }
        return n;
    }
    
    size_t print(const char str[]) { 
        return write((const uint8_t *)str, strlen(str)); 
    }
    
    size_t println(const char str[]) {
        size_t n = print(str);
        n += write((const uint8_t *)"\r\n", 2);
        return n;
    }

    size_t print(int n) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%d", n);
        return print(buf);
    }
    
    size_t println(int n) {
        size_t ret = print(n);
        ret += write((const uint8_t *)"\r\n", 2);
        return ret;
    }
};

#endif // WPROGRAM_H