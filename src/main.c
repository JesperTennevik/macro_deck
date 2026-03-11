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

int main(void){
    millis_init();
    adc_init();
    uart_init(9600);
    sei();
    // uint16_t slide_pot_0 = 0;
    // uint16_t slide_pot_1 = 0;
    millis_t now;
    millis_t last_tick = millis_get();
    millis_t interval = 100;
    // char send_buff[38];

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
            // sprintf(send_buff, "%u", slide_pot_0);
            // uart_send_str_endl(send_buff);
            // last_tick = now;
        }

        // slide_pot_0 = adc_read(0);
    }
    return 0;
}