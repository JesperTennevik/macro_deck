#ifndef UART_H
#define UART_H
#include <stdint.h>

#define TX_BUFFER_SIZE 128
#define RX_BUFFER_SIZE 128

void uart_init(uint32_t baud_rate);
void uart_send_byte(uint8_t byte);
void uart_send_str(char* str);
void uart_send_str_endl(char* str);
void uart_send_line(char* str);
void uart_send_byte_arr(uint8_t *arr, uint16_t len);
uint8_t uart_avaliable();
uint8_t uart_read();

#endif