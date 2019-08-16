#ifndef USART_H
#define USART_H

#include "stm32f0xx_system.h"

// Buffer sizes. Buffer size must be a power of 2
#define RXBUF_SIZE		512
#define TXBUF_SIZE		256

#if RXBUF_SIZE < 2
#error RXBUF_SIZE is too small.  It must be larger than 1.
#elif ((RXBUF_SIZE & (RXBUF_SIZE-1)) != 0)
#error RXBUF_SIZE must be a power of 2.
#endif

 #if TXBUF_SIZE < 2
#error TXBUF_SIZE is too small.  It must be larger than 1.
#elif ((TXBUF_SIZE & (TXBUF_SIZE-1)) != 0)
#error TXBUF_SIZE must be a power of 2.
#endif



typedef struct
{
	uint8_t 			*buf;
	volatile uint16_t 	head;
	volatile uint16_t 	tail;
	volatile uint16_t 	count;
	volatile uint16_t 	size;
	uint16_t			mask;
} ringbuffer_t;

extern ringbuffer_t g_rx_buf;
extern ringbuffer_t g_tx_buf;
volatile extern uint8_t g_tx_sending;

// Function prototypes
void init_uart(void);

void ring_buf_init(ringbuffer_t *buf, uint8_t *bufData, uint16_t size);
uint8_t ring_buf_get(ringbuffer_t *buf);
void ring_buf_put(ringbuffer_t *buf, const uint8_t c);
void ring_buf_wait_full(ringbuffer_t *buf);
uint8_t is_ring_buf_full(ringbuffer_t *buf);
uint8_t is_ring_buf_empty(ringbuffer_t *buf);

char get_line(char *lineBuf, uint16_t maxChars);

void USART1_IRQHandler(void);

#endif
