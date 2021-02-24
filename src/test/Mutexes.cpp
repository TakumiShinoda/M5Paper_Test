#include "./Mutexes.h"

QueueHandle_t Mutexes::i2c = xSemaphoreCreateMutex();