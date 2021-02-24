#ifndef TYPES_H
#define TYPES_H

#include <Arduino.h>

typedef struct{
    uint16_t w;
    uint16_t h;
} Size;

typedef struct{
    uint16_t x;
    uint16_t y;
} Vector2;

#endif