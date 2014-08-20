/*
 *	created:	lisper
 *	by:		2013-09-10
 *	function:	test the cds55
 *	need:		read_serial.h, split.h, cds55.h, SPI.h
 *
 */

#include <SPI.h>
#include "split.h"
#include "read_serial.h"
#include "cds55.h"

#define CMD_SIZE 32
#define CMDP_SIZE 8
char cmd_buf[CMD_SIZE];	//buf from serial
char *cmdp_buf[CMD_SIZE];	//build from cmd_buf, easy to get argument by index

//
void setup (void) {
	Serial.begin(9600);	// 开始串口通讯
	pinMode (SS, OUTPUT);	//set SS as OUTPUT
	digitalWrite(SS, LOW);	//write SS to LOW
	SPI.begin ();		// 开始SPI通讯
}

//
void loop (void) {
	digitalWrite (SS, LOW);	// 片选为从机
	if (read_serial (cmd_buf, CMD_SIZE)) {	//read serial data to cmd_buf
		int leng = split_in (cmdp_buf, cmd_buf, CMDP_SIZE);	//build cmdp_buf by cmd_buf

		char command = cmdp_buf[0][0];	//get command
		uint8_t id = atoi (cmdp_buf[1]); //get id
		uint16_t value = atoi (cmdp_buf[2]);	//get a argument

		Serial.print ("command = ");	// display command
		Serial.print (command);     	//
		Serial.print ("  id = ");   	//
		Serial.print (id);          	//
		Serial.print ("  value = ");	//
		Serial.println (value);     	//

		switch (command) {
			case 'p':		//move to value (0~300)
				cds55_pos (id, value);
				break;
			case 'r':		//reset servo
				cds55_reset (id);
				break;
			case 'l':		//open or close led by 1 or 0
				cds55_led (id, value);
				break;
			case 't':		//enable or disable torque by 1 or 0
				cds55_torque (id, value);
				break;
			case 'i':		//change id to newid by value
				cds55_setID (id, value);
				break;
			case 'v':		//change move velocity
				cds55_velocity (id, value);
				break;
			default:
				Serial.println ("error! no this command!");
		}
	}
	digitalWrite (SS, HIGH);	//取消从机
}


