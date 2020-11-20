#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "io.h"

int read_input_file(char *filename, input_data *inp)
{
  puts("Hello from read_input_file()");
  FILE *fp = fopen(filename, "r");
  if (fp == NULL)
  {
    perror(filename);
    exit(1);
  }

  int num_lines = count_lines(fp);
  //printf("Number of lines: %d\n", num_lines);

  //int num_accounts = count_accounts(fp);
  //printf("Number of Accounts: %d\n", num_accounts);
  inp->num_accounts = count_accounts(fp);
  //printf("inp->num_accounts: %d\n", inp->num_accounts);

  inp->num_customers = count_customers(fp);
  //printf("inp->num_cutomers: %d\n", inp->num_customers);

  initialize_balances(fp, inp);

  print_input_data(inp);

  //TODO

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

void initialize_balances(FILE *fp, input_data *inp)
{
  //printf("In i_b\n");
  int *bals = malloc(sizeof(int) * inp->num_accounts);
  char c;

  for (int i = 0; i < inp->num_accounts; i++)
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
  inp->balances = bals;
  fseek(fp, 0, SEEK_SET);
}

/*void process_customers()
{

}*/

void print_input_data(input_data *inp)
{
  printf("inp->num_accounts: %d\n", inp->num_accounts);
  printf("inp->num_cutomers: %d\n", inp->num_customers);

  //int the_balances[inp->num_accounts] = inp->balances;

  for (int i = 0 ; i < inp->num_accounts ; i++)
  {
    printf("Balance %d: ", i+1);
    printf("%d\n", (inp->balances)[i]);
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
