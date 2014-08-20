/*
 *	created:	lisper
 *	by:		2013-09-10
 *	function:	test the cds55
 *	need:		servo_ack_check, SPI.h
 *
 */
#include <SPI.h>
//#include "servo_ack_check.h"

#define SPI_BUF_SIZE 100

uint8_t spi_buf[SPI_BUF_SIZE];
volatile uint8_t spi_data_size = SPI_BUF_SIZE;
volatile enum {inread, inwrite, inreturn } state = inread;
volatile uint8_t pos = 0;

//
void setup (void) {
	Serial.begin (1000000);
	pinMode(MISO, OUTPUT);	// have to send on master in, *slave out*
	SPCR |= _BV(SPE);	// 设置为接收状态
	SPI.attachInterrupt();	// 开启中断
}

// SPI 中断程序
ISR (SPI_STC_vect) {
	byte spi_data = SPDR;
	if (state == inread) {
		if (pos < spi_data_size) {
			spi_buf[pos++] = spi_data;
			if (pos == 4 && (spi_buf[3] + 4) <= SPI_BUF_SIZE) { 
				spi_data_size = spi_buf[3] + 4;
			}
		} else 
			state = inwrite;
	} 
}

//
void loop (void) { 
	if (state == inwrite) {
		//if (servo_ack_check(spi_buf)) { }
		Serial.write (spi_buf, spi_buf[3] + 4);
		pos = 0;
		state = inread;
		spi_data_size = SPI_BUF_SIZE;
	}
}

