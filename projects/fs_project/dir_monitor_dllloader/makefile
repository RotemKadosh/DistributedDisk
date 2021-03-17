CC=g++
CPPFLAGS=-I ../include -ansi -pedantic-errors -Wall -Wextra -ldl -lboost_thread -lboost_system -g 
DEPS = dir_monitor.hpp dir_monitor_impl.hpp ../dispatcher_callback/dispatcher_callback_impl.hpp
OBJ = dir_monitor_test.o 

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
	ln -s ~/git/projects/fs_project/dispatcher_callback/dispatcher_callback.hpp ~/git/projects/fs_project/include/

clean: 
	rm -f *.o *.out *.gch