#include <Arduino.h>

/*
 *	created:	2013-06-21
 *	by:		lisper (leyapin@gmail.com)
 *	function:	build command string cmdstr
 */


int split_in (char **cmdstr, char *str, int leng) {
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


