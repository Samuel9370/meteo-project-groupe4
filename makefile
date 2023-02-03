SRC = $(wildcard *.c)                   
OBJ = $(SRC:.c=.o) 
#make pour compiler puis ./exec 
all : exec

%.o : %.c header.h
		gcc -O2 -c $< -o $@

exec: $(OBJ)
		gcc -O2 $^ -o $@

clean :
	rm -f *.o
	rm -f *.txt
	rm exec

