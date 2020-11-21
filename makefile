CC = gcc
H = io.h types.h
C = main.c io.c
EXEC = a6

build: ${C} ${H}
	${CC} ${C} -o ${EXEC}

test:
	@echo "Running a6 on assignment_6_input_file.txt"
	./${EXEC} assignment_6_input_file.txt
	diff assignment_6_output_file.txt expected_output.txt
	@echo "Passed!"

clean:
	@echo "Cleaning up..."
	rm -f ${EXEC} assignment_6_output_file.txt

run:
	make clean
	make
	make test

debug:
	make clean
	make
	./${EXEC} assignment_6_input_file.txt
	cat assignment_6_output_file.txt

test2:
	./${EXEC} input2.txt
	diff assignment_6_output_file.txt expected_output2.txt
	@echo "Passed!"

robust:
	make clean
	make
	make test
	make test2
