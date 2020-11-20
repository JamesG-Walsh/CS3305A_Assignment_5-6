#ifndef TYPES_H
#define TYPES_H

typedef struct input_data
{
  int num_accounts;
  int num_customers;
  int *balances;
} input_data;

typedef struct node
{
	int value;
	struct node *next;
} node;

#endif
