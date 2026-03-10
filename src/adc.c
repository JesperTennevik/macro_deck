#include <avr/io.h>
#include <stdint.h>
#include "adc.h"

void adc_init(){
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRA |= (1 << ADEN);
}

void adc_start(uint8_t channel){
    ADMUX = (1 << REFS0) | (channel & 0x0F);

    ADCSRA |= (1 << ADSC);
}

uint8_t adc_ready(){
    return !(ADCSRA & (1 << ADSC));
}

uint16_t read_adc(){
    uint8_t adc_data_low = ADCL;
    uint8_t adc_data_high = ADCH;
    return (adc_data_high << 8) | adc_data_low;
}

uint16_t adc_read(uint8_t channel){
    adc_start(channel);
    while(!adc_ready());
    return read_adc();
}