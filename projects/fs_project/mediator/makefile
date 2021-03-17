CC=g++
CPPFLAGS=-I ../include -ansi -pedantic-errors -Wall -Wextra -lboost_thread -lboost_system -g 
DEPS = mediator.hpp
OBJ = mediator_test.o  mediator_impl.o ../thread_pool/thread_pool_impl.cpp 

.PHONY = run cgdb vlg clean ar gc

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
	ln -s ~/git/projects/fs_project/mediator/mediator.hpp ~/git/projects/fs_project/include/

clean: 
	rm -f *.o *.out *.gch