#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "millis.h"
#include "adc.h"
#include "uart.h"
#include "buttons.h"

#define NUM_BUTTONS sizeof(buttons) / sizeof(buttons[0])

#define NUM_SLIDERS 2
#define interval 100

int main(void){
    millis_init();
    adc_init();
    uart_init(115200);
    sei();

    uint16_t sliders[NUM_SLIDERS] = {0};
    millis_t now;
    millis_t last_tick = millis_get();

    Button buttons[] = {
        { &DDRB, &PORTB, &PINB, PB4, 0, 0, 0 },
        { &DDRB, &PORTB, &PINB, PB3, 0, 0, 0 },
        { &DDRB, &PORTB, &PINB, PB2, 0, 0, 0 },
        { &DDRB, &PORTB, &PINB, PB1, 0, 0, 0 },
        { &DDRB, &PORTB, &PINB, PB0, 0, 0, 0 },

        { &DDRD, &PORTD, &PIND, PD7, 0, 0, 0 }
    };

    for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
        button_init(&buttons[i], buttons[i].DDRx, buttons[i].PORTx, buttons[i].PINx, buttons[i].pin_number);
    }

    while(1){
        now = millis_get();

        for(uint8_t i = 0; i < NUM_BUTTONS; i++){
            if(get_button_click(&buttons[i])){
                char buff[10];
                sprintf(buff, "Button: %u", i);
                uart_send_str_endl(buff);
            }
        }

        if(now - last_tick >= interval){
            for(uint8_t i = 0; i < NUM_SLIDERS; i++){
                char buff[17];
                sprintf(buff, "Slider %u: %lu%%", i, ((uint32_t)sliders[i]*100)/1023);
                uart_send_str_endl(buff);
            }
            last_tick = now;
        }

        for(uint8_t i = 0; i < NUM_SLIDERS; i++){
            sliders[i] = adc_read(i);
        }
    }

    return 0;
}