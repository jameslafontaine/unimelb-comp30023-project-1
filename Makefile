CC=gcc
CFLAGS=-Wall
TEST=list_test
OUT1=out1
OUT2=out2
OUT3=out3
OUT4=out4
DLLOBJ=dlinkedlist.o
OBJ1=obj1.o obj2.o obj3.o
OBJ2=$(OBJ1) obj4.o

all: $(OUT1) $(OUT2) $(OUT3) $(OUT4)

$(TEST): list_test.c $(DLLOBJ)
	$(CC) $(CFLAGS) -o $(TEST) $< $(DLLOBJ) -g

$(OUT1): fast.c $(OBJ1)
	$(CC) $(CFLAGS) -o $(OUT1) $< $(OBJ1)
	
$(OUT2): slow.c
	$(CC) $(CFLAGS) -o $(OUT2) $<

$(OUT3): fast-temp.c
	$(CC) $(CFLAGS) -o $(OUT3) $<

$(OUT4): slow-temp.c
	$(CC) $(CFLAGS) -o $(OUT4) $<

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o $(OUT1) $(OUT2) $(OUT3) $(OUT4)

format:
	clang-format -i *.c *.h
