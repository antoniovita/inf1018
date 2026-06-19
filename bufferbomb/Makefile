####################################
# CS:APP Buffer Lab
# Makefile that builds a buffer bomb
####################################

####################################### 
# No need to change anything below here
#######################################
CC = gcc
CFLAGS = -Wall -O0
UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

# Make the stack executable when supported by the toolchain/arch.
# - Linux (GNU as): use --execstack
# - macOS x86_64: use the linker flag
# - macOS arm64: executable stacks are not supported, so leave empty
ifeq ($(UNAME_S),Darwin)
  ifeq ($(UNAME_M),x86_64)
    STACKEXEC = -Wl,-allow_stack_execute
  else
    STACKEXEC =
  endif
else
  STACKEXEC = -Wa,--execstack
endif

# Build the buffer bomb
bufbomb: bufbomb.c buf.c hex2raw
	$(CC) $(CFLAGS) -no-pie -fno-stack-protector $(STACKEXEC) -o bufbomb bufbomb.c buf.c 

# Build hex2raw utility
hex2raw: hex2raw.c
	$(CC) $(CFLAGS) -o hex2raw hex2raw.c

# Use this when you want to recompile
clean:
	-rm -f *.o bufbomb hex2raw *~
