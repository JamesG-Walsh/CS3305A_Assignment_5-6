/*	CS3305A       Fall 2020   Assignment 4
		James Walsh   jwalsh57    250481718
*/

#define FILENAME_INPUT "assignment_6_input_file.txt"
#define FILENAME_OUTPUT "assignment_6_output_file.txt"

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "io.h"
#include "types.h"

int main(int argc, char* argv[])
{
  puts("Starting main()");

  int threadedMode = 1;
  if (argc < 2 || argc > 3)
  {
  printf("Usage: %s input.txt\n", argv[0]);
  exit(1);
  }
  if (argc == 3 && strncmp(argv[2], "ut", 2) == 0)
  {
    printf("Running without using threads.\n");
    threadedMode = 0;
  }

  bank_data *ban_dat = malloc(sizeof(bank_data));
  //puts("About to call read_input_file()");
  read_input_file(argv[1], ban_dat, threadedMode);
  puts("Back in main from read_input_file()");
  print_formatted_output(ban_dat);

  if (access(FILENAME_OUTPUT, F_OK) != -1)
  {
    remove(FILENAME_OUTPUT);
  }
  int fp_outfile = open(FILENAME_OUTPUT, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
  dup2(fp_outfile, STDOUT_FILENO); //TODO why are the puts/printf only printing to file, not terminal?

  print_formatted_output(ban_dat);

  free(ban_dat->balances);
  free(ban_dat->transaction_string_lengths);
  free(ban_dat);
  close(fp_outfile);

  return 0;
}
