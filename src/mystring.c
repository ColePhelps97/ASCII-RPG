#include <stdlib.h>
#include <stdio.h>

int isEquals(char* string1, char* string2) {
	size_t i = 0;
	while(string1[i] != '\n' || string2[i] != '\n') {
		if(string1[i] != string2[i]) return 0;
		i++;
	}
	return 1;
}

int isSubString(char* string1, char* string2) {
	size_t i = 0;
	while(1) {
		if(string1[i] == '\0' || string2[i] == '\0') return 1;
		if(string1[i] != string2[i]) return 0;
		if(string1[i] == string2[i]) i++;
	}
}
