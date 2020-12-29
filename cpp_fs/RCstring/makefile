CC=g++
CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g
DEPS = RCString_adv.hpp 
OBJ = RCString_adv.o RCString_test.o 
.PHONY = run cgdb vlg clean ar gd

a.out: $(DEPS)
	g++ -ansi -pedantic-errors -Wall -Wextra -g RCString_adv.cpp RCString_test.cpp

run: a.out
	./a.out

cgdb: a.out
	cgdb a.out

vlg: a.out
	valgrind --leak-check=yes --track-origins=yes ./a.out

gd:
	g++ -ansi -pedantic-errors -Wall -Wextra -g $(OBJ) $(DEPS) 

ar: 
	ar rcs RCString_adv.ar $(OBJ) $(DEPS)

clean: 
	rm -f *.o *.out *.gch