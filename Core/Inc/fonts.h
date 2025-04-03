#ifndef __FONTS_H__
#define __FONTS_H__

#include <stdint.h>

typedef struct {
    const uint8_t width;
    const uint8_t height;
    const uint32_t *data;
} FontDef;

typedef struct {
    const uint8_t width;
    const uint8_t height;
    const uint8_t *data;
} FontMeDef;


extern const FontDef Font_7x10;
extern const FontDef Font_11x18;
extern const FontDef Font_Custom;
extern const FontMeDef Font_14x28;

extern const FontMeDef Dino1_32x32;
extern const FontMeDef Dino2_32x32;
extern const FontMeDef Block_32x32;
#endif
