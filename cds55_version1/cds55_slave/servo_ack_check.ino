//for cds55 
//function:	is checksum ok 

boolean servo_ack_check (uint8_t *data) {
	uint8_t sum = 0;
	uint8_t leng = data[3] + 3;
	for (uint8_t i=2; i<leng; i++) {
		sum += data[i];
	}
	if (data[leng] == (uint8_t )~sum)
		return true;
	else
		return false;
}


