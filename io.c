#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "io.h"

int num_accounts(FILE *fp)
{
  int num_accounts = 0;
  char c;

  c = getc(fp);
  if (c == 'a')
  {
    num_accounts += 1;
  }
  if (c != 'a')
  {
    printf("No accounts in input file."); //TODO unless the input has a customer first...
    return 0;
  }

  while(c != EOF)
  {

  }


  //for (c = getc(fp); c != EOF;/* c = getc(fp)*/)
  //for(c = getc(fp); c!= '\n'; c=getc(fp))

  fseek(fp, 0, SEEK_SET);
  return num_accounts;
}

int num_lines(FILE *fp)
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

int read_input_file(char *filename, input_data *inp)
{
  puts("Hello from read_input_file()");
  FILE *fp = fopen(filename, "r");
  if (fp == NULL)
  {
    perror(filename);
    exit(1);
  }

  printf("Calling num_lines()\n");
  int n_lines = num_lines(fp);
  printf("Number of lines: %d\n", n_lines);

  //int count_accounts = num_accounts(fp);
  //printf("Number of Accounts: %d ", count_accounts);


  fclose(fp);
  //free(buffer);

  return 0;
}
