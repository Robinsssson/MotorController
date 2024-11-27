#ifndef __MIRCO_STRING__H__
#define __MIRCO_STRING__H__
#include <stdint.h>
#include <mirco_lib/mirco.h>
#if defined(__GNUC__)
#define MIRCO_LIBC_API_EXPORT __attribute__((visibility("default")))
#else
#define MIRCO_LIBC_API_EXPORT
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif
MIRCO_LIBC_API uint16_t mirco_strlen(uint8_t *str);
MIRCO_LIBC_API uint8_t *mirco_strcpy(uint8_t *dest, uint8_t *src);
MIRCO_LIBC_API uint8_t *mirco_strncpy(uint8_t *dest, uint8_t *src, uint16_t n);
MIRCO_LIBC_API uint8_t *mirco_strcat(uint8_t *dest, uint8_t *src);
MIRCO_LIBC_API uint8_t *mirco_strncat(uint8_t *dest, uint8_t *src, uint16_t n);
MIRCO_LIBC_API int mirco_strcmp(uint8_t *str1, uint8_t *str2);
MIRCO_LIBC_API int mirco_strncmp(uint8_t *str1, uint8_t *str2, uint16_t n);
MIRCO_LIBC_API uint8_t *mirco_strchr(uint8_t *str, uint8_t c);
MIRCO_LIBC_API uint8_t *mirco_strrchr(uint8_t *str, uint8_t c);
MIRCO_LIBC_API uint8_t *mirco_strstr(uint8_t *haystack, uint8_t *needle);
MIRCO_LIBC_API void mirco_itoa(uint8_t* arr, uint8_t length, int32_t num);
#endif  //!__MIRCO_STRING__H__