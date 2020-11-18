/*	CS3305A       Fall 2020   Assignment 4
		James Walsh   jwalsh57    250481718
*/

#define FILENAME_INPUT "assignment_6_input_file.txt"
#define FILENAME_OUTPUT "assignment_6_output_file.txt"

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "io.h"
#include "types.h"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
  printf("Usage: %s input.txt\n", argv[0]);
  exit(1);
  }

  input_data input;
  read_input_file(argv[1], &input); //TODO implement function in io.c

  if (access(FILENAME_OUTPUT, F_OK) != -1)
  {
    remove(FILENAME_OUTPUT);
  }
  int fp_outfile = open(FILENAME_OUTPUT, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
  dup2(fp_outfile, STDOUT_FILENO); //TODO why are the puts/printf only printing to file, not terminal?

  puts("techno");
  printf("electronic music");
  puts("");

  close(fp_outfile);

  return 0;
}
