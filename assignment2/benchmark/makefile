PROG := membench

CC := gcc
CFLAGS := -O4 

.PHONY: default clean

default: $(PROG).ps

$(PROG).ps: $(PROG).gp $(PROG).out
	gnuplot < $(PROG).gp > $(PROG).ps

$(PROG).out: $(PROG)
	./$(PROG) > $(PROG).out

$(PROG): $(PROG).c
	$(CC) $(CFLAGS) -o $(PROG) $(PROG).c

clean:
	rm -f $(PROG) $(PROG).ps $(PROG).out
