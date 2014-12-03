/*
 *	created:	lisper
 *	by:		2013-09-10
 *	description:	test the digitalservo shield with cds556 servo
 *
 */

#include <SPI.h>
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
	if (serialRead (Serial, (uint8_t *)cmd_buf, CMD_SIZE, 4)) {	//read serial data to cmd_buf
		int leng = split (cmdp_buf, cmd_buf, CMDP_SIZE);	//build cmdp_buf by cmd_buf

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

int split (char **cmdstr, char *str, int leng) {
	int i;
	for (i=0; *str && i<leng-1; i++) {
		while (isspace (*str))
			*str++='\0';
		if (*str == '\0')
			break;
		cmdstr[i] = str;
		while (isgraph (*str))
			str++;
	}
	cmdstr[i] = '\0';
	return i;
}

//call like : serialRead (Serial1, buffer, 12, 5)
uint8_t serialRead (HardwareSerial the_serial, 
		uint8_t *buf, uint8_t leng, uint8_t timeout) {
	int sub;
	if (the_serial.available ()) {
		for (sub=0; sub<leng; sub++) {
			uint32_t start_time = millis ();
			while (!the_serial.available ()) {
				if (millis () - start_time > timeout)
					return sub;
			}
			buf[sub] = the_serial.read ();
		}
		return sub;
	}
	return 0;
}



