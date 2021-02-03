CC=g++
CPPFLAGS=-I ../utils -ansi -pedantic-errors -Wall -Wextra -lpthread -lboost_thread -lboost_system -lboost_chrono -g 
DEPS = thread_pool.hpp future_task.hpp 
OBJ = thread_pool_impl.o thread_pool_test.o future_task_impl.hpp

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