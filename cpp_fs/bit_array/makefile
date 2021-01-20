CC=g++
CPPFLAGS=-I ../utils -ansi -pedantic-errors -Wall -Wextra -g
DEPS = bit_array.hpp bit_array_impl.hpp
OBJ = bit_array_test.o 

.PHONY = run cgdb vlg clean ar sl gc

%.o: %.c $(DEPS) 
	$(CC) -c -o $@ $< $(CPPFLAGS)

a.out: $(OBJ)
	$(CC) -o $@ $^ $(CPPFLAGS) 

run: a.out
	./a.out

cgdb: a.out
	cgdb a.out

vlg: a.out
	valgrind --leak-check=yes --track-origins=yes ./a.out

gc:
	g++ -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 $(OBJ) $(DEPS) 

ar: 
	ar rcs vsa.ar $(OBJ) $(DEPS)

sl:
	ln -s ~/git/system_programming/vsa/vsa.h ~/git/system_programming/include

clean: 
	rm -f *.o *.out *.gch