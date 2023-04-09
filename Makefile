CC=gcc
CFLAGS=-Wall -g 
ALLOC=allocate
ALLOC_DEBUG=allocate-debug
DLL_O=d_linked_list.o
PUTILS_O=process_utils.o
PROC_MAN_O=process_manager.o 
INPUT_O=input_parser.o
OUTPUT_O=output.o

all: $(ALLOC)


$(ALLOC): main.c $(PROC_MAN_O) $(INPUT_O) $(OUTPUT_O) $(PUTILS_O) $(DLL_O)
	$(CC) $(CFLAGS) -o $(ALLOC) $^ -lm

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o $(ALLOC)

format:
	clang-format -i *.c *.h
