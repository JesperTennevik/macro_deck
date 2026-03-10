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

int main(void){
    millis_init();
    adc_init();
    uart_init(9600);
    sei();
    uint16_t slide_pot_0 = 0;
    uint16_t slide_pot_1 = 0;
    millis_t now;
    millis_t last_tick = millis_get();
    millis_t interval = 100;
    char send_buff[38];

    Button mx_switch;
    button_init(&mx_switch, &DDRB, &PORTB, &PINB, PB5);

    while(1){
        now = millis_get();

        if(get_button_click(&mx_switch)){
            uart_send_str_endl("Click!");
        }

        if(now - last_tick >= interval){
            // sprintf(send_buff, "%u : %u", slide_pot_0, slide_pot_1);
            // uart_send_str_endl(send_buff);
            // last_tick = now;
        }

        slide_pot_0 = adc_read(0);
        slide_pot_1 = adc_read(1);
    }
    return 0;
}