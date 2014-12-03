/*
 *	created:	lisper
 *	by:		2013-09-10
 *	function:	firmware of digitalservo shield
 *
 */
#include <SPI.h>

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

//  //for cds55 
//  //function:	is checksum ok 
//  boolean servo_ack_check (uint8_t *data) {
//  	uint8_t sum = 0;
//  	uint8_t leng = data[3] + 3;
//  	for (uint8_t i=2; i<leng; i++) {
//  		sum += data[i];
//  	}
//  	if (data[leng] == (uint8_t )~sum)
//  		return true;
//  	else
//  		return false;
//  }
