#ifndef TYPES_H
#define TYPES_H

typedef struct bank_data
{
  int num_accounts;
  int num_customers;
  int *balances;
  int *transaction_string_lengths;
  //char **transaction_strings;
} bank_data;

typedef struct node
{
	int value;
	struct node *next;
} node;

#endif
