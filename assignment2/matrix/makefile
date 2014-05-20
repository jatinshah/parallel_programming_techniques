######################################################################
#
# 'gmake'        : create program $(PROG)
# 'gmake clean'  : clean directory
# 'gmake submit' : submit all sources to TA via email
#
######################################################################

# list your source/assembly files in SRCS
SRCS := matmul.c    

CFLAGS := -O3

######################################################################
# NO CHANGES BELOW THIS LINE

PROG := matmul
DRIVER := driver
MATMULCC := gcc $(CFLAGS)
DRIVERCC := $(CC) -O3

.PHONY: default clean submit

default: $(PROG)

$(PROG): $(SRCS) $(DRIVER).o
	$(MATMULCC) -o $@ $^

$(DRIVER).o: $(DRIVER).c
	$(DRIVERCC) -o $@ -c $^

clean:
	rm -f *~ *.o $(PROG) 
