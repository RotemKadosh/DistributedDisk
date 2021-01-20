CC=g++
CPPFLAGS=-I ../utils -ansi -pedantic-errors -Wall -Wextra -lboost_thread -lboost_system -lboost_chrono -g 
DEPS = waitable_q.hpp waitable_q_impl.hpp
OBJ = waitable_q_test.o 

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