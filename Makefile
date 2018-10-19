CC=gcc
CXX=g++

CFLAGS=-g -O1 -std=c99 -pthread -lm -Wall -Wextra -Werror
CXXFLAGS=-g -O1 -lm -Wall -Wextra -Werror -std=c++11

all: run_aluno grade 

memvirt.o: memvirt.c

aluno.o: aluno.c

test.o: test.c

aluno: memvirt.o aluno.o 
	$(CC) $(CFLAGS) memvirt.o aluno.o -lm -o aluno

test: memvirt.o test.o
	$(CC) $(CFLAGS) memvirt.o test.o -lm -o test

run_aluno: aluno
	./aluno

grade: test
	./test

clean:
	rm -rf *.o test aluno
