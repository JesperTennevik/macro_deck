#include <avr/io.h>
#include <stdint.h>
#include "buttons.h"
#include "millis.h"

static const millis_t debounce_ms = 30;

void button_init(Button *button, vui8 *DDR, vui8 *PORT, vui8 *PIN, int pin_number){
    button->DDRx = DDR;
    button->PORTx = PORT;
    button->PINx = PIN;
    button->prev_debounce = 0;
    button->pin_number = pin_number;

    *(button->DDRx) &= ~(1 << button->pin_number);
    *(button->PORTx) |=  (1 << button->pin_number);

    button->button_state = (*(button->PINx) >> button->pin_number) & 1;
    button->last_state = button->button_state;
}

uint8_t get_button_click(Button *button){
    uint8_t raw = (*(button->PINx) >> button->pin_number) & 1;
    uint8_t ret = 0;
    if(raw != button->last_state){
        button->prev_debounce = millis_get();
        button->last_state = raw;
    }

    if((millis_get() - button->prev_debounce) >= debounce_ms){
        if(button->button_state != raw) {
            button->button_state = raw;
            if(button->button_state == 0){
                ret = 1;
            }
        }
    }
    return ret;
}