CC=gcc
CFLAGS=-Wall
TEST=list_test
INPUT=input_parser.o
MAIN=main
PUTILS=process_utils.o
OUT1=out1
OUT2=out2
DLLOBJ=dlinkedlist.o
OBJ1=obj1.o obj2.o obj3.o
OBJ2=$(OBJ1) obj4.o

all: $(OUT1) $(OUT2) 

$(TEST): list_test.c $(DLLOBJ)
	$(CC) $(CFLAGS) -o $(TEST) $< $(DLLOBJ) -g

$(MAIN): main.c $(PUTILS) $(INPUT) 
	$(CC) $(CFLAGS) -o $(MAIN) $< -g $(PUTILS) $(INPUT) 

$(OUT1): fast.c $(OBJ1)
	$(CC) $(CFLAGS) -o $(OUT1) $< $(OBJ1)
	
$(OUT2): slow.c
	$(CC) $(CFLAGS) -o $(OUT2) $<


%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o $(OUT1) $(OUT2)

format:
	clang-format -i *.c *.h
