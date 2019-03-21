#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H
#include <stdint.h>
int16_t* generateMessageOut(uint16_t command ,int16_t *data);
uint8_t checkMessageIn(uint8_t *data);
uint16_t setMessageIn(uint8_t *data);
int16_t* getDataIn();
#endif // MESSAGEPARSER_H
