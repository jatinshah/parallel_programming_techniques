######################################################################
#
# 'gmake'        : create program $(PROG)
# 'gmake clean'  : clean directory
# 'gmake submit' : submit all sources to TA via email
#
######################################################################

# list your source/assembly files in SRCS
SRCS := matmul1.c    

CFLAGS := -O4 

GCOV := -fprofile-arcs -ftest-coverage
######################################################################
# NO CHANGES BELOW THIS LINE

PROG := matmul
DRIVER := driver
MATMULCC := gcc $(CFLAGS) $(GCOV)
DRIVERCC := $(CC) -O4 $(GCOV)

.PHONY: default clean submit

default: $(PROG)

$(PROG): $(SRCS) $(DRIVER).o
	$(MATMULCC) -o $@ $^

$(DRIVER).o: $(DRIVER).c
	$(DRIVERCC) -o $@ -c $^

clean:
	rm -f *~ *.o $(PROG) 
