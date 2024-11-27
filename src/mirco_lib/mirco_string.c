#include "mirco_lib/mirco_string.h"
#include <stdint.h>

MIRCO_LIBC_API uint16_t mirco_strlen(uint8_t* str)
{
    uint16_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

MIRCO_LIBC_API uint8_t* mirco_strcpy(uint8_t* dest, uint8_t* src)
{
    uint16_t i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}

MIRCO_LIBC_API uint8_t* mirco_strncpy(uint8_t* dest, uint8_t* src, uint16_t n)
{
    uint16_t i = 0;
    while (i < n && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    while (i < n) {
        dest[i] = '\0';
        i++;
    }
    return dest;
}

MIRCO_LIBC_API int mirco_strcmp(uint8_t* str1, uint8_t* str2)
{
    uint16_t i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return str1[i] - str2[i];
        }
        i++;
    }
    if (str1[i] != '\0') {
        return 1;
    }
    if (str2[i] != '\0') {
        return -1;
    }
    return 0;
}

MIRCO_LIBC_API int mirco_strncmp(uint8_t* str1, uint8_t* str2, uint16_t n)
{
    uint16_t i = 0;
    while (i < n && str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return str1[i] - str2[i];
        }
        i++;
    }
    if (i < n) {
        return 0;
    }
    if (str1[i] != '\0') {
        return 1;
    }
    if (str2[i] != '\0') {
        return -1;
    }
    return 0;
}

MIRCO_LIBC_API uint8_t* mirco_strcat(uint8_t* dest, uint8_t* src)
{
    uint16_t i = mirco_strlen(dest);
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}

MIRCO_LIBC_API uint8_t* mirco_strncat(uint8_t* dest, uint8_t* src, uint16_t n)
{
    uint16_t i = mirco_strlen(dest);
    while (i < n && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    while (i < n) {
        dest[i] = '\0';
        i++;
    }
    return dest;
}

MIRCO_LIBC_API uint8_t* mirco_strchr(uint8_t* str, uint8_t c)
{
    uint16_t i = 0;
    while (str[i] != '\0') {
        if (str[i] == c) {
            return &str[i];
        }
        i++;
    }
    return NULL;
}

MIRCO_LIBC_API uint8_t* mirco_strrchr(uint8_t* str, uint8_t c)
{
    uint16_t i = mirco_strlen(str);
    while (i > 0) {
        i--;
        if (str[i] == c) {
            return &str[i];
        }
    }
    return NULL;
}

MIRCO_LIBC_API uint8_t* mirco_strstr(uint8_t* haystack, uint8_t* needle)
{
    uint16_t i = 0;
    uint16_t j = 0;
    while (haystack[i] != '\0') {
        if (haystack[i] == needle[j]) {
            j++;
            if (needle[j] == '\0') {
                return &haystack[i];
            }
        } else {
            i -= j;
            j = 0;
        }
        i++;
    }
    return NULL;
}

MIRCO_LIBC_API void mirco_itoa(uint8_t* arr, uint8_t length, int32_t num)
{
    int32_t cpy, len = 0;
    uint8_t f = 0;
    if (num < 0) {
        f      = 1;
        arr[0] = '-';
    }
    if (f) {
        cpy = -num;
    } else {
        cpy = num;
    }
    while (cpy != 0) {
        cpy /= 10;
        len++;
    }
    if (len > length) {
        arr[0] = '\0';
        return;
    }
    for (uint8_t i = f; i < len + f; i++) {
        arr[len - i - 1] = num % 10 + '0';
        num /= 10;
    }
    arr[len + f] = '\0';
}