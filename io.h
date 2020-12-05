#ifndef IO_H
#define IO_H

#include <stdio.h>
#include "types.h"

int read_input_file(char *filename, bank_data *input, int threadedMode);

int count_accounts(FILE *fp);
int count_customers(FILE *fp);

void initialize_balances(FILE *fp, bank_data *inp);
void populate_transaction_string_lengths(FILE *fp, bank_data *inp);

void process_all_customer_transactions(FILE *fp, bank_data *bd);
void process_all_customer_transactions_unthreaded(FILE *fp, bank_data *bd);
void process_customer(thread_params *tp, transaction_node *first_node);
void process_customer_nodes(transaction_node* first_node);

void deposit(int cid, int amount, int account_number, bank_data *bd);
void withdraw(int cid, int amount, int account_number, bank_data *bd);
void transfer(int cid, int amount, int origin_account_number, int destination_account_number, bank_data *bd);

void print_bank_data(bank_data *inp);
void print_formatted_output(bank_data *bd);

int count_lines(FILE *fp);

#endif
