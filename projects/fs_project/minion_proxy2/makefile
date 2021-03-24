CC=g++
CPPFLAGS= -ansi -Wall -L/home/rotemkadosh27/git/projects/fs_project/minion_proxy1/monitor_dir -Wl,-rpath=/home/rotemkadosh27/git/projects/fs_project/minion_proxy1/monitor_dir -ldl -lboost_thread -lboost_chrono -lpthread -lboost_system -lboost_filesystem -g -I ../utils/ -I ../include/ -rdynamic
DEPS = mimion_proxy.hpp
OBJ = minion_exe.o minion_proxy.o ../storage/storage.o ../socket/socket.o ../udpsocket/server_udp_socket.o ../protocol_translator/protocol_translator.o ../app_protocol/app_protocol.o ../minion_data/minion_data.o ../framework/framework.o ../logger/logger.o ../reactor/reactor.o ../mediator/mediator.o ../thread_pool/thread_pool_impl.o ../dir_monitor_dllloader/dir_monitor_impl.o ../config_manager/config_manager.o
.PHONY = run cgdb vlg clean ar gc

%.o: %.cpp $(DEPS) 
	$(CC) -c -o $@ $< $(CPPFLAGS)

a.out: $(OBJ)
	$(CC) -o $@ $^ $(CPPFLAGS)

run: a.out
	./a.out

cgdb: a.outT
	cgdb a.out

vlg: a.out
	valgrind --leak-check=yes --track-origins=yes ./a.out

read_cmd:
	$(CC) $(CPPFLAGS) -I /home/rotemkadosh27/git/projects/fs_project/include -I /home/rotemkadosh27/git/projects/fs_project/utils -fPIC -shared -o libreadcmd.so  ../minion_cmd/minion_read_cmd.hpp ../minion_cmd/minion_read_cmd.cpp

write_cmd:
	$(CC) $(CPPFLAGS) -I /home/rotemkadosh27/git/projects/fs_project/include -I /home/rotemkadosh27/git/projects/fs_project/utils -fPIC -shared -o libwritecmd.so  ../minion_cmd/minion_write_cmd.hpp ../minion_cmd/minion_write_cmd.cpp

gc:
	g++ -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 $(OBJ) 

ar: 
	ar rcs vsa.ar $(OBJ) $(DEPS)

sl:
	

clean: 
	rm -f *.o *.out *.gch