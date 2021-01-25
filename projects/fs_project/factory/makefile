CC=g++
CPPFLAGS=-I ../../../cpp_fs/utils -ansi -pedantic-errors -Wall -Wextra -g 
DEPS = factory.hpp future_task.hpp 
OBJ = factory_impl.hpp factory_test.o 

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

clean: 
	rm -f *.o *.out *.gch