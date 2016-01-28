#ifndef _READ_FILE_H_
#define _READ_FILE_H_

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

char* read_fline(FILE *file);
size_t count_frows(FILE *file);
char** read_file(FILE *file);
void print_file(FILE *file);

#endif
