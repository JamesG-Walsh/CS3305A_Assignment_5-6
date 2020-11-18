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
  puts("Starting main()");
  if (argc != 2)
  {
  printf("Usage: %s input.txt\n", argv[0]);
  exit(1);
  }

  input_data inp_dat;
  puts("About to call read_input_file()");
  read_input_file(argv[1], &inp_dat); //TODO implement function in io.c
  puts("Back in main from read_input_file()");

  if (access(FILENAME_OUTPUT, F_OK) != -1)
  {
    remove(FILENAME_OUTPUT);
  }
  int fp_outfile = open(FILENAME_OUTPUT, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
  dup2(fp_outfile, STDOUT_FILENO); //TODO why are the puts/printf only printing to file, not terminal?

  puts("techno");
  //printf("electronic music");
  puts("");

  close(fp_outfile);

  return 0;
}
