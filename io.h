#ifndef IO_H
#define IO_H

#include <stdio.h>
#include "types.h"

int read_input_file(char *filename, bank_data *input);

int count_accounts(FILE *fp);
int count_customers(FILE *fp);

void initialize_balances(FILE *fp, bank_data *inp);
void populate_transaction_string_lengths(FILE *fp, bank_data *inp);
void process_all_customer_transactions(FILE *fp, bank_data *bd);
//void process_customer_transactions(FILE *fp, bank_data *bd, int cid);
void process_customer(char *transaction_str);

void deposit(int amount, int account_number);
void withdraw(int amount, int account_number);
void transfer(int amount, int origin_account_number, int destination_account_number);

void print_bank_data(bank_data *inp);

int count_lines(FILE *fp);

#endif
