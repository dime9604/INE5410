FLAGS=-Wall -Wno-unused-result -std=c11 -O2

CC=gcc

RM=rm -f

EXEC=tsp-seq

all: $(EXEC)

tsp.o: src/tsp.c
	$(CC) $(FLAGS) -c src/tsp.c

$(EXEC): tsp.o
	$(CC) $(FLAGS) tsp.o -o $(EXEC)
	$(RM) tsp.o

run:
	./$(EXEC)

clean:
	$(RM) tsp.o $(EXEC)
