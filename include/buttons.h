#ifndef BUTTONS_H
#define BUTTONS_H

#include "millis.h"
#include <stdint.h>

typedef volatile uint8_t vui8;

typedef struct {
    vui8 *DDRx;
    vui8 *PORTx;
    vui8 *PINx;
    int pin_number;
    uint8_t button_state;
    uint8_t last_state;
    millis_t prev_debounce;
} Button;

void button_init(Button *button, vui8 *DDR, vui8 *PORT, vui8 *PIN, int pin_number);
uint8_t get_button_click(Button *button);

#endif