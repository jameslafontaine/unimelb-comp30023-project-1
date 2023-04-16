CC=gcc
CFLAGS=-Wall -g 
EXE=allocate
ALLOC_DEBUG=allocate-debug
DLL_O=d_linked_list.o
PUTILS_O=process_utils.o
PROC_MAN_O=process_manager.o 
INPUT_O=input_parser.o
OUTPUT_O=output.o
T4_O=task4.o

all: $(EXE)


$(EXE): src/main.c $(PROC_MAN_O) $(T4_O) $(INPUT_O) $(OUTPUT_O) $(PUTILS_O) $(DLL_O) 
	$(CC) $(CFLAGS) -o $(EXE) $^ -lm

%.o: src/%.c src/%.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o $(EXE)

format:
	clang-format -i *.c *.h
