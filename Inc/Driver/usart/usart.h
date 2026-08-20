#ifndef USART_H
#define USART_H

#include "stm32f103.h"

void USART_Init(USART_TypeDef* USARTx, uint32_t APB_Clock, uint32_t baudrate);
void USART_SendChar(USART_TypeDef* USARTx, char ch);
void USART_SendString(USART_TypeDef* USARTx, char* str);
char USART_Received(USART_TypeDef* USARTx);
void USART_Recieved_String(USART_TypeDef* USARTx, char* buffer, int lenght);

#endif // USART_H