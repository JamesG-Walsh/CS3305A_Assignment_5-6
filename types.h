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

typedef struct thread_params
{
  int cid; //TODO is this necessary?
  bank_data *bd;
  char tra_str[]; //tranasction string
} thread_params;

typedef struct node
{
	int value;
	struct node *next;
} node;

typedef struct transaction_node
{
	int tran_type; //withdrawal <= -1 , transfer == 0, deposit >= 1 
  int cid; //customer id
  int account_num_a; //deposit, withdrawal, transfer origin
  int account_num_b; //transfer destination
  int dollar_amount;
  int prev_node_was_final_transaction; //1 for true, 0 for false
	struct transaction_node *next;
} transaction_node;

#endif
