#ifndef IO_H
#define IO_H

#include <stdio.h>
#include "types.h"

int read_input_file(char *filename, input_data *input);

int count_accounts(FILE *fp);
int count_customers(FILE *fp);

void initialize_balances(FILE *fp, input_data *inp);

void print_input_data(input_data *inp);

int count_lines(FILE *fp);

#endif
