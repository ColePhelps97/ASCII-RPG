#include <stdlib.h>
#include <stdio.h>

int is_equals(char* string1, char* string2) {
	size_t i = 0;
	while(string1[i] != '\0' || string2[i] != '\0') {
		if(string1[i] != string2[i]) return 0;
		i++;
	}
	return 1;
}

int is_sub_string(char* string1, char* string2) {
	size_t i = 0;
	while(1) {
		if(string1[i] == '\0' || string2[i] == '\0') return 1;
		if(string1[i] != string2[i]) return 0;
		if(string1[i] == string2[i]) i++;
	}
}


/* variants: 0-level; 1-enemy */
char* concat(char* string1, char* string2, char* string3) {
	char* string;
	size_t iter = 0, string1_length, string2_length, string3_length;
	while(string1[iter] != '\0') {
		iter++;
	}
	string1_length = iter;
	iter = 0;
	while(string2[iter] != '\0') {
		iter++;
	}
	string2_length = iter;
	iter = 0;
	while(string3[iter] != '\0') {
		iter++;
	}
	string3_length = iter;	
	string = (char*) malloc(sizeof(char) * (string1_length + string2_length + string3_length));
	for(iter = 0; iter < (string1_length + string2_length + string3_length); iter++) {
		if(iter < string1_length) 
			string[iter] = string1[iter];
		if((iter >= string1_length) && (iter < string1_length + string2_length))
			string[iter] = string2[iter - string1_length];
		if((iter >= string1_length + string2_length) && (iter < string1_length + string2_length + string3_length))
			string[iter] = string3[iter - string1_length - string2_length];
	}
	return string;
}
	
		
char* ultostr(unsigned long value, int base) {
	char* ptr;
	unsigned long t = 0, res = 0;
	unsigned long tmp = value;
	int count = 0;
	ptr = (char*) malloc(sizeof(char)*1024);

	if (tmp == 0) {
		count++;
	}

	while(tmp > 0) {
		tmp = tmp/base;
		count++;
	}

	ptr += count;
	*ptr = '\0';

	do {
		res = value - base * (t = value / base);
		if (res < 10) {
			* -- ptr = '0' + res;
		}
		else if ((res >= 10) && (res < 16)) {
			* --ptr = 'A' - 10 + res;
		}
	} while ((value = t) != 0);

	return ptr;
}	
