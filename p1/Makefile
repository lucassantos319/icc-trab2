DEPS = utils.h

CC = gcc

CFLAGS = -Wall -DLIKWID_PERFMON -I${LIKWID_INCLUDE} -O3 -mavx -march=native 

LFLAGS = -L/usr/local/lib -lmatheval -L${LIKWID_LIB} -llikwid

all: newton

newton: main.o Rosenbrock.o functions.o utils.o 
	$(CC) -o newtonPC main.o Rosenbrock.o functions.o  utils.o $(LFLAGS)

functions.c: functions.h
utils.c: utils.h
Rosenbrock.c : Rosenbrock.h

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

clean:
	-rm -f *~ *\# \#* *.o newtonPC

purge: clean
	-rm -f newtonPC

