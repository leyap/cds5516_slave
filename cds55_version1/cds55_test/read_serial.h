#include <Arduino.h>

int read_serial(char *buf, int leng) {
	int i;
	for (i=0; i<leng-1 && Serial.available (); i++) {
		buf[i] = Serial.read ();
                delay (10);
	}
	buf[i] = '\0';
	return i;
}
