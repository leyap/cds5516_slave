/*
 *	created:	2013-09-10
 *	by:		lisper (leyapin@gmail.com)
 *	description:	library for cds5516 servo
 *	version:	1.0
 *	last:		2013-09-11 11:02
 *
 *	function:
 *
 *	void cds55_pos		(uint8_t id, uint16_t pos)
 *				//move to pos (0~300)
 *
 *	void cds55_velocity	(uint8_t id, uint16_t velocity)
 *				//change one servo velocity (0~300)
 *
 *	void cds55_setID	(uint8_t id, uint8_t new_id)
 *				//change id to new_id
 *
 *	void cds55_led		(uint8_t id, uint8_t state)
 *				// open or close led by 1 or 0
 *
 *	void cds55_torque	(uint8_t id, uint8_t state)
 *				//enable or disable torque
 *
 *	void cds55_return_level	(uint8_t id, uint8_t level)
 *				//set the return level
 *
 *	void cds55_reset	(uint8_t id)
 *				//reset servo
 */

#include <SPI.h>


#define startByte		0xFF

#define P_MODEL_NUMBER_L	0
#define P_MODEL_NUMBER_H	1
#define P_VERSION		2
#define P_ID			3
#define P_BAUD_RATE		4
#define P_RETURN_DELAY_TIME	5
#define P_CW_ANGLE_LIMIT_L	6
#define P_CW_ANGLE_LIMIT_H	7
#define P_CCW_ANGLE_LIMIT_L	8
#define P_CCW_ANGLE_LIMIT_H	9
#define P_SYSTEM_DATA2		10
#define P_LIMIT_TEMPERATURE	11
#define P_DOWN_LIMIT_VOLTAGE	12
#define P_UP_LIMIT_VOLTAGE	13
#define P_MAX_TORQUE_L		14
#define P_MAX_TORQUE_H		15
#define P_RETURN_LEVEL		16
#define P_ALARM_LED		17
#define P_ALARM_SHUTDOWN	18
#define P_OPERATING_MODE	19
#define P_DOWN_CALIBRATION_L	20
#define P_DOWN_CALIBRATION_H	21
#define P_UP_CALIBRATION_L	22
#define P_UP_CALIBRATION_H	23
#define P_TORQUE_ENABLE		24
#define P_LED			25
#define P_CW_COMPLIANCE_MARGIN	26
#define P_CCW_COMPLIANCE_MARGIN	27
#define P_CW_COMPLIANCE_SLOPE	28
#define P_CCW_COMPLIANCE_SLOPE	29
#define P_GOAL_POSITION_L	30
#define P_GOAL_POSITION_H	31
#define P_GOAL_SPEED_L		32
#define P_GOAL_SPEED_H		33
#define P_TORQUE_LIMIT_L	34
#define P_TORQUE_LIMIT_H	35
#define P_PRESENT_POSITION_L	36
#define P_PRESENT_POSITION_H	37
#define P_PRESENT_SPEED_L	38
#define P_PRESENT_SPEED_H	39
#define P_PRESENT_LOAD_L	40
#define P_PRESENT_LOAD_H	41
#define P_PRESENT_VOLTAGE	42
#define P_PRESENT_TEMPERATURE	43
#define P_REG_WRITE_FLAG	44
#define P_PAUSE_TIME		45
#define P_MOVING		46
#define P_LOCK			47    	
#define P_PUNCH_L		48
#define P_PUNCH_H		49

//—	Instruction —
#define INST_PING		0x01
#define INST_READ		0x02
#define INST_WRITE		0x03
#define INST_REG_WRITE		0x04
#define INST_ACTION		0x05
#define INST_RESET		0x06
#define INST_DIGITAL_RESET	0x07
#define INST_SYSTEM_READ	0x0C
#define INST_SYSTEM_WRITE	0x0D
#define INST_SYNC_WRITE		0x83
#define INST_SYNC_REG_WRITE	0x84  


#define SPI_BUF_SIZE 128
char spi_buf[SPI_BUF_SIZE] = {0xff, 0xff};	//command buf, will send by spi

//send some data by spi, delay 5us
void spi_send (char *data, int leng) {
	for (int i=0; i<leng; i++) {
		SPI.transfer (data[i]);
		delayMicroseconds (5);
	}
}

//move to pos (0~300)
void cds55_pos (uint8_t id, uint16_t pos) {
	spi_buf[2] = id;
	spi_buf[3] = 5;	//length
	spi_buf[4] = INST_WRITE;	//command
	spi_buf[5] = P_GOAL_POSITION_L;	//position
	pos = pos > 300 ? 300 : pos;
	//pos = pos/300.0 * 1023;	//change 0~300 to 0~1023
	pos = pos * 3.41;		//change 0~300 to 0~1023
	spi_buf[6] = (uint8_t)pos;	//low
	spi_buf[7] = (uint8_t)(pos>>8);	//high
	spi_buf[8] = ~(id + 5 + spi_buf[4] + spi_buf[5] + spi_buf[6] + spi_buf[7]);	//checksum
	spi_buf[9] = '\n';	//more one
	spi_send (spi_buf, 5 + 5);	//send command data by spi
	delayMicroseconds (160);
}

void cds55_all_pos (uint8_t* id_array, uint16_t* pos_array, uint8_t length) {
}

//P_GOAL_SPEED_L		32
//change one servo velocity (0~300)
void cds55_velocity (uint8_t id, uint16_t velocity) {
	spi_buf[2] = id;
	spi_buf[3] = 5;			//length
	spi_buf[4] = INST_WRITE;	//command
	spi_buf[5] = P_GOAL_SPEED_L;	//position
	velocity = velocity > 300 ? 300 : velocity;
	//velocity = velocity/300.0 * 1023;	//change 0~300 to 0~1023
	velocity = velocity * 3.41;		//change 0~300 to 0~1023
	spi_buf[6] = (uint8_t)velocity;		//low
	spi_buf[7] = (uint8_t)(velocity>>8);	//high
	spi_buf[8] = ~(id + 5 + spi_buf[4] + spi_buf[5] + spi_buf[6] + spi_buf[7]);	//checksum
	spi_buf[9] = '\n';		//more one
	spi_send (spi_buf, 5 + 5);	//send command data by spi
	delayMicroseconds (160);
}

// change all servo velocity
void cds55_all_velocity (uint8_t *idstr, uint8_t length, uint16_t velocity) {
}

//change id to new_id
void cds55_setID (uint8_t id, uint8_t new_id) {
	spi_buf[2] = id;
	spi_buf[3] = 4;		//length
	spi_buf[4] = INST_WRITE;	//command
	spi_buf[5] = P_ID;	//position
	spi_buf[6] = new_id;
	spi_buf[7] = ~(id + 4 + spi_buf[4] + spi_buf[5] + spi_buf[6]);	//checksum
	spi_buf[8] = '\n';	//more one
	spi_send (spi_buf, 4 + 5);	//send command data by spi
	delayMicroseconds (160);
}

// open or close led by 1 or 0
void cds55_led (uint8_t id, uint8_t state) {
	spi_buf[2] = id;
	spi_buf[3] = 4;		//length
	spi_buf[4] = INST_WRITE;	//command
	spi_buf[5] = P_LED;	//position
	spi_buf[6] = state;
	spi_buf[7] = ~(id + 4 + spi_buf[4] + spi_buf[5] + spi_buf[6]);	//checksum
	spi_buf[8] = '\n';	//more one
	spi_send (spi_buf, 4 + 5);	//send command data by spi
	delayMicroseconds (160);
}

//enable or disable torque
void cds55_torque (uint8_t id, uint8_t state) {
	spi_buf[2] = id;
	spi_buf[3] = 4;	//length
	spi_buf[4] = INST_WRITE;	//command
	spi_buf[5] = P_TORQUE_ENABLE;	//ram position
	spi_buf[6] = state;
	spi_buf[7] = ~(id + 4 + spi_buf[4] + spi_buf[5] + spi_buf[6]);	//checksum
	spi_buf[8] = '\n';	//more one
	spi_send (spi_buf, 4 + 5);	//send command data by spi
	delayMicroseconds (160);
}

//set the return level
void cds55_return_level (uint8_t id, uint8_t level) {
	spi_buf[2] = id;
	spi_buf[3] = 4;	//length
	spi_buf[4] = INST_WRITE;	//command
	spi_buf[5] = P_RETURN_LEVEL;	//ram position
	spi_buf[6] = level;
	spi_buf[7] = ~(id + 4 + spi_buf[4] + spi_buf[5] + spi_buf[6]);	//checksum
	spi_buf[8] = '\n';	//more one
	spi_send (spi_buf, 4 + 5);	//send command data by spi
	delayMicroseconds (160);
}

//reset servo
void cds55_reset (uint8_t id) {
	spi_buf[2] = id;
	spi_buf[3] = 2;	//length
	spi_buf[4] = INST_RESET;	//command
	spi_buf[5] = ~(id + 2 + spi_buf[4]);	//checksum
	spi_buf[6] = '\n';	//more one
	spi_send (spi_buf, 2 + 5);	//send command data by spi
	delayMicroseconds (160);
}


