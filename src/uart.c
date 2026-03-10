#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "uart.h"

static volatile uint8_t tx_buffer[TX_BUFFER_SIZE] = {0};
static volatile uint8_t tx_buffer_head = 0;
static volatile uint8_t tx_buffer_tail = 0;

static volatile uint8_t rx_buffer[RX_BUFFER_SIZE] = {0};
static volatile uint8_t rx_buffer_head = 0;
static volatile uint8_t rx_buffer_tail = 0;

ISR(USART_UDRE_vect){
    UDR0 = tx_buffer[tx_buffer_tail];
    tx_buffer_tail = (tx_buffer_tail + 1) % TX_BUFFER_SIZE;

    if(tx_buffer_head == tx_buffer_tail){
        UCSR0B &= ~(1 << UDRIE0);
    }
}

ISR(USART_RX_vect){
    if(((rx_buffer_head + 1) % RX_BUFFER_SIZE) == rx_buffer_tail){  
        return;
    }

    rx_buffer[rx_buffer_head] = UDR0;
    rx_buffer_head = (rx_buffer_head + 1) % RX_BUFFER_SIZE;
}

void uart_init(uint32_t baud_rate){
    baud_rate = (F_CPU / (16 * baud_rate)) - 1;
    
    UBRR0H = (uint8_t)(baud_rate >> 8);
    UBRR0L = (uint8_t)baud_rate;
    
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
}

void uart_send_byte(uint8_t byte){
    if(((tx_buffer_head + 1) % TX_BUFFER_SIZE) == tx_buffer_tail){
        return;
    }

    uint8_t buffer_empty = (tx_buffer_head == tx_buffer_tail);

    tx_buffer[tx_buffer_head] = byte;
    tx_buffer_head = (tx_buffer_head + 1) % TX_BUFFER_SIZE;

    if(buffer_empty) {
        UCSR0B |= (1 << UDRIE0);
    }
}

void uart_send_str(char *str){
    uint16_t i = 0;
    while(str[i] != '\0'){
        uart_send_byte(str[i]);
        i++;
    }
}

void uart_send_str_endl(char *str){
    uint16_t i = 0;
    while(str[i] != '\0'){
        uart_send_byte(str[i]);
        i++;
    }
    uart_send_byte('\r');
    uart_send_byte('\n');
}

void uart_send_line(char *str){
    uart_send_byte('\r');
    uart_send_byte('\n');
    uint16_t i = 0;
    while(str[i] != '\0'){
        uart_send_byte(str[i]);
        i++;
    }
    uart_send_byte('\r');
    uart_send_byte('\n');
}

void uart_send_byte_arr(uint8_t *arr, uint16_t len){
    for(uint16_t i = 0; i < len; i++){
        uart_send_byte(arr[i]);
    }
}

uint8_t uart_avaliable(){
    return ((rx_buffer_head - rx_buffer_tail + RX_BUFFER_SIZE) % RX_BUFFER_SIZE);
}

uint8_t uart_read(){
    if (rx_buffer_head == rx_buffer_tail) {
        return 0;
    }

    uint8_t byte = rx_buffer[rx_buffer_tail];
    rx_buffer_tail = (rx_buffer_tail + 1) % RX_BUFFER_SIZE;
    return byte;
}