#include "usart.h"

// Defines
#define DEL        0x7F
#define BACKSPACE  0x08
#define CR         0x0D
#define LF         0x0A

// Global variables
ringbuffer_t g_rx_buf = { 0 };
ringbuffer_t g_tx_buf = { 0 };
volatile uint8_t g_tx_sending = 0;
uint8_t g_echo_serial = 1;

uint8_t g_rx_buffer_data[RXBUF_SIZE];
uint8_t g_tx_buffer_data[TXBUF_SIZE];


void init_uart(void)
{
	MX_USART1_UART_Init();

	// Initialise the RX and TX buffers
	ring_buf_init(&g_rx_buf, g_rx_buffer_data, RXBUF_SIZE);
	ring_buf_init(&g_tx_buf, g_tx_buffer_data, TXBUF_SIZE);

	// Clear Overrun flag, in case characters have already been sent to USART
	USART1->ICR = USART_ICR_ORECF;

	// Enable RXNE interrupt
	USART1->CR1 |= USART_CR1_RXNEIE;
}

void ring_buf_init(ringbuffer_t *buf, uint8_t *bufData, uint16_t size)
{
	buf->buf = bufData;
	buf->head = 0;
	buf->tail = 0;
	buf->count = 0;
	buf->size = size;
	buf->mask = size - 1;
}

void ring_buf_wait_full(ringbuffer_t *buf)
{
	while (is_ring_buf_full(buf))
	{
		if (!g_tx_sending)
		{
			g_tx_sending = 1;
			USART1->CR1 |= USART_CR1_TXEIE;		// Enable the TX empty interrupt
		}
	}
}


uint8_t ring_buf_get(ringbuffer_t *buf)
{
	if (!buf->buf)
		return 0;

	uint8_t ret = buf->buf[buf->tail];
	buf->tail = (buf->tail + 1) & buf->mask;
	buf->count--;

	return ret;
}

void ring_buf_put(ringbuffer_t *buf, const uint8_t c)
{
	if (!buf->buf)
		return;

	buf->buf[buf->head] = c;
	// increment the head index
	buf->head = (buf->head + 1) & buf->mask;
	buf->count++;
}

inline uint8_t is_ring_buf_full(ringbuffer_t *buf)
{
	return ((buf->head - buf->tail) & buf->mask) == buf->mask;
}

inline uint8_t is_ring_buf_empty(ringbuffer_t *buf)
{
	return (buf->tail == buf->head);
}

int __io_putchar(int ch)
{
	if (ch == '\n')
	{
		ring_buf_wait_full(&g_tx_buf);
    	ring_buf_put(&g_tx_buf, '\r');
	}

	ring_buf_wait_full(&g_tx_buf);
	ring_buf_put(&g_tx_buf, ch);

	if (!g_tx_sending)
	{
		g_tx_sending = 1;
		USART1->CR1 |= USART_CR1_TXEIE;		// Enable the TX empty interrupt
	}

	return ch;
}

int _write(int file, char *ptr, int len)
{
	for (int i= 0; i < len; i++)
	   __io_putchar( *ptr++ );

	return len;
}

char get_line(char *lineBuf, uint16_t maxChars)
{
	static int count = 0;
	char res = 0;

	if (g_rx_buf.count == 0)
		return 0;

  	if (count < maxChars - 1)
	{
		uint8_t c = ring_buf_get(&g_rx_buf);

		if (c == CR)
			c = LF;    						// Read character

    	if (c == BACKSPACE || c == DEL)  	// Process backspace
		{
      		if (count != 0)
			{
        		count--;
				if (g_echo_serial)
				{
					__io_putchar(0x08);			// Send Backspace and erase previous character
					__io_putchar(' ');
					__io_putchar(0x08);
				}
      		}
    	}
       	else
		{
			if (g_echo_serial)
			{
				__io_putchar(c);
			}
			lineBuf[count] = c;					// store character
			count++;
		}

		if (c == LF)
		{
			lineBuf[count] = 0;
			res = 1;
			count = 0;
		}
  	}
  	else
  	{
  		// return the string without waiting for the CR if the line is too long
  		lineBuf[count] = 0;
		count = 0;
  		return 1;
  	}

	return res;
}

void USART1_IRQHandler(void)
{
	// Check Receive Not Empty interrupt enabled and active
	if ((USART1->CR1 & USART_CR1_RXNEIE) && (USART1->ISR & USART_ISR_RXNE))
	{
		// Read the character from the serial port
		uint8_t c = (uint8_t)(USART1->RDR);

		// If the buffer is not full add the character otherwise it will be discarded
		if (!is_ring_buf_full(&g_rx_buf))
		{
			ring_buf_put(&g_rx_buf, c);
		}
	}

	// Check Transmit Empty interrupt enabled and active
	if ((USART1->CR1 & USART_CR1_TXEIE) && (USART1->ISR & USART_ISR_TXE))
	{
		// Send the next character
		if (!is_ring_buf_empty(&g_tx_buf))
		{
			USART1->TDR = ring_buf_get(&g_tx_buf);
		}
		else
		{

			USART1->CR1 &= ~USART_CR1_TXEIE;	// Disable the TXE interrupt
			USART1->CR1 |= USART_CR1_TCIE;		// Enable the TC interrupt
			g_tx_sending = 0;
		}
	}

	// Check Transmission Complete interrupt enabled and active
	if ((USART1->CR1 & USART_CR1_TCIE) && (USART1->ISR, USART_ISR_TC))
	{
		USART1->ICR = USART_ICR_TCCF;			// Clear the TC flag

		if (g_tx_buf.count == 0)
		{
			USART1->CR1 &= ~USART_CR1_TCIE;		// Disable the TC interrupt
			g_tx_sending = 0;
		}
	}
}
