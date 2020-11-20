#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

  process_all_customer_transactions(fp, bd);

  print_bank_data(bd);
  for (int cid = 1; cid <= bd->num_customers ; cid++)
  {
    //process_customer_transactions(fp, bd, cid);
  }
  print_bank_data(bd);

  fclose(fp);
  //free(buffer);

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
  int i;
  char c;
  char *transaction_string;
  char throwaway_str[20];
  // = malloc(sizeof(char) * bd->transaction_string_lengths);
  for(i = 1 ; i <= bd->num_accounts ; i++) //skip over account initial balance lines
  {
    /*while(c != '\n') //go to next line
    {
      c = getc(fp);
    }
    c = getc(fp);*/
    fgets(throwaway_str, 80, fp);
  }

  for(i = 1 ; i <= bd->num_customers ; i++)
  {
    transaction_string = malloc(sizeof(char) * bd->transaction_string_lengths[i-1]);
    //for(; c != '\n' && c != EOF; c = getc(fp))
    //{
    fgets(transaction_string, bd->transaction_string_lengths[i-1] + 4, fp); //TODO play around with the +4
    //}
    puts(transaction_string);
    process_customer(transaction_string);
    //bd->transaction_strings[i-1] = transaction_string;
    //c = getc(fp);
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

//void process_customer_transactions(FILE *fp, bank_data *bd, int cid)
void process_customer(char *transaction_str)
{
  printf("Processing Customer\n");
  int i = 0;
  char c = transaction_str[i];

  const char delim[2] = " ";
  char *buffer = NULL;

  int account_a;
  int account_b;
  int amount;

  char *tok = strtok(buffer, delim);
  while (tok != NULL)
  {
    printf("tok[0]: %c\n", tok[0]);
    switch (tok[0]) //TODO make sure this is robust for accounts and customer ids bigger than 10
    {
      case 'c':
        printf("found the c at the start.\n");
        //i += 3;
        //c = transaction_str[i];
        //printf("c: %c\n", c);

        tok = strtok(NULL, delim);
        break;
      case 'd':
        printf("Depositing");
        //i += 3;
        //c = transaction_str[i];
        //account_a = c;
        //printf(" into account %d", account_a - 48); //subtract 48 to convert from ASCII code



        //i += 2;
        //c = transaction_str[i];

        break;
      case 'w':
        printf("Withdrawing\n");
        break;
      case 't':
        printf("Transfering\n");
        break;
      default:
        printf("");
        //printf("Unexpected character found. c: %c\n", c);
    }
  }

  //fseek(fp, 0, SEEK_SET);
}

void deposit(int amount, int account_number)
{

}

void withdraw(int amount, int account_number)
{

}

void transfer(int amount, int origin_account_number, int destination_account_number)
{

}

void print_bank_data(bank_data *bd)
{
  printf("\ninp->num_accounts: %d\n", bd->num_accounts);
  printf("inp->num_cutomers: %d\n", bd->num_customers);
  for (int i = 0 ; i < bd->num_accounts ; i++)
  {
    printf("Balance %d: ", i+1);
    printf("%d\n", bd->balances[i]);
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
