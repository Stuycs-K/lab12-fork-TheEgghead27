.PHONY: compile run clean
compile main: main.o
	@gcc -o fork main.o
main.o: main.c
	@gcc -c main.c
run: main
	@./fork $(ARGS)
clean:
	@-rm -rf fork *.o *.txt
