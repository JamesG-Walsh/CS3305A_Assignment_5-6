#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "io.h"

int read_input_file(char *filename, bank_data *bd)
{
  puts("Starting read_input_file()");
  FILE *fp = fopen(filename, "r");
  if (fp == NULL)
  {
    perror(filename);
    exit(1);
  }
  //int num_lines = count_lines(fp); //printf("Number of lines: %d\n", num_lines);
  bd->num_accounts = count_accounts(fp); //printf("inp->num_accounts: %d\n", inp->num_accounts);
  bd->num_customers = count_customers(fp);  //printf("inp->num_cutomers: %d\n", inp->num_customers);

  //bd->transaction_string_lengths = malloc(sizeof(int) * bd->num_customers);
  initialize_balances(fp, bd);
  populate_transaction_string_lengths(fp, bd);

  print_bank_data(bd);

  process_all_customer_transactions(fp, bd);



  sleep(1);//TODO using this until i can get join to work

  print_bank_data(bd);

  fclose(fp);

  return 0;
}

int count_accounts(FILE *fp)
{
  //puts("inside count_account()");
  int num_accounts = 0;
  char c;
  c = getc(fp);
  //printf("c: %c\n", c);
  if (c == 'a')
  {
    num_accounts += 1;
    //printf("num_accounts: %d\n", num_accounts);
  }
  if (c != 'a')
  {
    printf("First line of input file is not an account balance.\n"); //TODO unless the input has a customer first...
    return 0;
  }
  while(c != EOF)
  {
    c = getc(fp); //printf("c: %c\n", c);
    if (c == 'a') //check first character of line
    {
      num_accounts += 1;  //printf("num_accounts: %d\n", num_accounts);
    }
    while(c != '\n' && c != EOF) //go to next line
    {
      c = getc(fp);
    }
  }
  fseek(fp, 0, SEEK_SET);
  return num_accounts;
}

int count_customers(FILE *fp)
{
  //puts("inside count_customers()");
  int num_customers = 0;
  char c = getc(fp);  //printf("c: %c\n", c);
  if (c == 'c')
  {
    num_customers += 1; //printf("num_customers: %d\n", num_accounts);
  }
  while(c != EOF)
  {
    c = getc(fp); //printf("c: %c\n", c);
    if (c == 'c') //check first character of line
    {
      num_customers += 1;  //printf("num_accounts: %d\n", num_accounts);
    }
    while(c != '\n' && c != EOF) //go to next line
    {
      c = getc(fp);
    }
  }
  fseek(fp, 0, SEEK_SET);
  return num_customers;
}

void populate_transaction_string_lengths(FILE *fp, bank_data *bd)
{
  char c;
  int str_len;
  int *tsl = malloc(sizeof(int) * bd->num_customers);
  int i = 1;

  for(c = getc(fp); i<= bd->num_accounts; i++) //this loop skips over the account initial balance lines
  {
    while(c != '\n') //go to next line
    {
      c = getc(fp);
    }
    c = getc(fp);
  }

  c = getc(fp);
  i = 1;
  for(i = 1 ; i <= bd->num_customers ; i++)
  {
    str_len = 0;
    c = getc(fp);
    while(c != '\n' && c != EOF) //go to next line
    {
      if(c == '\0')
      {
        puts("aha");
      }
      str_len += 1;
      //printf("%c", c);
      //printf("c: %c - str_len: %d\n", c, str_len);
      c = getc(fp);
    }
    //printf("str_len: %d\n\n", str_len);
    tsl[i-1] = str_len;
  }
  bd->transaction_string_lengths = tsl;
  fseek(fp, 0, SEEK_SET);
}

void process_all_customer_transactions(FILE *fp, bank_data *bd)
{
  //printf("pact\n");

  int i;
  char *transaction_string;
  char throwaway_str[20];


  for(i = 1 ; i <= bd->num_accounts ; i++) //skip over account initial balance lines
  {
    fgets(throwaway_str, 80, fp);
  }

  pthread_t thread = malloc(sizeof(pthread_t) * bd->num_customers);
  if(thread == NULL)
  {
    printf("Out of memory\n");
    exit(1);
  }
  for(i = 1 ; i <= bd->num_customers ; i++)
  {
    transaction_string = malloc(sizeof(char) * bd->transaction_string_lengths[i-1]);

    fgets(transaction_string, bd->transaction_string_lengths[i-1] + 4, fp); //TODO play around with the +4

    printf("\n%s", transaction_string);

    thread_params *tp = malloc(sizeof(thread_params));

    tp->cid = i;
    tp->tra_str = transaction_string;
    tp->bd = bd;

    //process_customer2(tp); //TODO threads
    if(pthread_create( &thread[i-1], NULL, process_customer, (void*) tp) != 0)
    {
      perror("Thread creation error\n");
      exit(1);
    }

  }

  for(i = 0; i < bd->num_customers; i++)
  {
    pthread_join(&thread[i], NULL);
  }

  fseek(fp, 0, SEEK_SET);
}

void initialize_balances(FILE *fp, bank_data *bd)
{
  //printf("In i_b\n");
  int *bals = malloc(sizeof(int) * bd->num_accounts);
  char c;

  for (int i = 0; i < bd->num_accounts; i++)
  {
    c = getc(fp); //a
    c = getc(fp); //account number
    c = getc(fp); //space
    c = getc(fp); //b
    c = getc(fp); //space
    //printf("c: %c\n", c);
    fscanf(fp, "%d", &bals[i]); //the balance
    //printf("bals[%d]: %d\n", i, bals[i]);
    for (; c!= '\n' ; c = getc(fp)) //proceed to next line
    {
    }
  }
  bd->balances = bals;
  fseek(fp, 0, SEEK_SET);
}

void process_customer(thread_params *tp)
{
  printf("\nProcessing Customer %d\tTransaction String: %s", tp->cid, tp->tra_str);

  char c = tp->tra_str[0];

  const char delim[2] = " ";
  char *buf = NULL;

  int account_a;
  int account_b;
  int dollar_amount;

  int loop_count = 0;
  char *tok = strtok(tp->tra_str, delim);
  while (tok != NULL && loop_count < 5)
  {
    loop_count += 1;
    //printf("tok[0]: %c\n", tok[0]);
    switch (tok[0]) //TODO make sure this is robust for accounts and customer ids bigger than 10
    {
      case 'c':
        //printf("found the c at the start.\n");
        tok = strtok(NULL, delim);
        break;
      case 'd':
        //printf("Depositing\n");

        tok = strtok(NULL, delim);
        buf = strdup(tok); //printf("buf: %s\n", buf);
        if (tok[0] == 'a')
        {
          buf++; //printf("buf: %s\n", buf);
        }
        account_a = atoi(buf);

        tok = strtok(NULL, delim);
        buf = strdup(tok);//printf("buf: %s\n", buf);
        dollar_amount = atoi(buf);

        deposit(tp->cid, dollar_amount, account_a, tp->bd);
        tok = strtok(NULL, delim);
        //free(buf); //TODO why is this causing error?
        break;
      case 'w':
        //printf("Withdrawing\n");

        tok = strtok(NULL, delim);
        buf = strdup(tok);
        //printf("buf: %s\n", buf);
        if (tok[0] == 'a')
        {
          buf++;
          //printf("buf: %s\n", buf);
        }
        account_a = atoi(buf);

        tok = strtok(NULL, delim);
        buf = strdup(tok); //printf("buf: %s\n", buf);
        dollar_amount = atoi(buf);

        withdraw(tp->cid, dollar_amount, account_a, tp->bd);
        tok = strtok(NULL, delim);

        break;
      case 't':
        //printf("Transfering\n");
        tok = strtok(NULL, delim);
        buf = strdup(tok); //printf("buf: %s\n", buf);
        if (tok[0] == 'a')
        {
          buf++; //printf("buf: %s\n", buf);
        }
        account_a = atoi(buf);

        tok = strtok(NULL, delim);
        buf = strdup(tok); //printf("buf: %s\n", buf);
        if (tok[0] == 'a')
        {
          buf++; //printf("buf: %s\n", buf);
        }
        account_b = atoi(buf);

        tok = strtok(NULL, delim);
        buf = strdup(tok); //printf("buf: %s\n", buf);
        dollar_amount = atoi(buf);

        transfer(tp->cid, dollar_amount, account_a, account_b, tp->bd);
        tok = strtok(NULL, delim);

        break;
      default:
        printf("Unexpected character found while processing customer %d c: '%c'\n", tp->cid, tok[0]);
    }
  }
  //fseek(fp, 0, SEEK_SET);
}

void deposit(int cid, int amount, int account_number, bank_data *bd)
{
  printf("DEPOSIT Customer %d depositing $%d into a%d\n", cid, amount, account_number);
  bd->balances[account_number - 1] += amount;
}

void withdraw(int cid, int amount, int account_number, bank_data *bd)
{
  //TODO mutual exclusion
  if(amount <= bd->balances[account_number -1])
  {
    bd->balances[account_number - 1] -= amount;
    printf("WITHDRAWAL Customer %d withdrawing $%d from a%d\n", cid, amount, account_number);
  }
  else
  {
    printf("DECLINED.  Insufficient funds for withdrawal.  Customer %d withdrawing $%d from a%d\n", cid, amount, account_number);
  }
}

void transfer(int cid, int amount, int origin_account_number, int destination_account_number, bank_data *bd)
{
  //TODO mutual exclusion
  if(amount <= bd->balances[origin_account_number -1])
  {
    bd->balances[origin_account_number - 1] -= amount;
    bd->balances[destination_account_number - 1] += amount;
    printf("TRANSFER Customer %d transfering $%d from a%d to a%d\n", cid, amount, origin_account_number, destination_account_number);
  }
  else
  {
    printf("DECLINED.  Insufficient funds for transfer.  Customer %d transfering $%d from a%d to a%d\n", cid, amount, origin_account_number, destination_account_number);
  }
}


void print_bank_data(bank_data *bd)
{
  printf("\nbd->num_accounts: %d\n", bd->num_accounts);
  printf("bd->num_cutomers: %d\n", bd->num_customers);
  for (int i = 0 ; i < bd->num_accounts ; i++)
  {
    printf("Balance %d: %d\n", i+1, bd->balances[i]);
  }
  for (int i = 1 ; i <= bd->num_customers ; i++)
  {
    printf("Customer %d tsl: %d\n", i, bd->transaction_string_lengths[i-1]);
  }
}

int count_lines(FILE *fp)
{
  int count = 0; // Line counter (result)
  char c;		   // To store a character read from file

  // Extract characters from file and store in character c
  for (c = getc(fp); c != EOF; c = getc(fp))
    if (c == '\n') //if this character is newline
      count++; //increment count
      //printf("count: %d", count);

  fseek(fp, 0, SEEK_SET);
  return count;
}
