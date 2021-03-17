CC=g++
CPPFLAGS= -ansi -Wall -L/home/may/git/projects/master/monitor_dir/ -Wl,-rpath=/home/may/git/projects/master/monitor_dir/ -ldl -lboost_thread -lboost_chrono -lpthread -lboost_system -lboost_filesystem -g -I ../utils/ -I ../include/ -rdynamic
DEPS = master.hpp
OBJ = master_test.o master.o ../raid/raid.o ../minion_manager/minion_manager.o ../socket/socket.o ../udpsocket/client_udp_socket.o ../protocol_translator/protocol_translator.o ../app_protocol/app_protocol.o ../minion_data/minion_data.o ../framework/framework.o ../logger/logger.o ../reactor/reactor.o ../mediator/mediator.o ../thread_pool/thread_pool_impl.o ../dir_monitor_dllloader/dir_monitor_impl.o ../nbd_proxy/nbd_proxy.o 
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
	$(CC) $(CPPFLAGS) -I /home/may/git/projects/include/ -I /home/may/git/projects/utils/ -fPIC -shared -o libreadcmd.so  ../commands/read_command.cpp ../udpsocket/client_udp_socket.cpp

write_cmd:
	$(CC) $(CPPFLAGS) -I /home/may/git/projects/include/ -I /home/may/git/projects/utils/ -fPIC -shared -o libwritecmd.so  ../commands/write_command.cpp ../udpsocket/client_udp_socket.cpp

minion1:
	$(CC) $(CPPFLAGS) minion_test.cpp ../udpsocket/server_udp_socket.cpp ../socket/socket.cpp ../app_protocol/app_protocol.cpp  ../protocol_translator/protocol_translator.cpp ../logger/logger.cpp -lboost_thread -lboost_system -lboost_filesystem -o minion1.out

minion2:
	$(CC) $(CPPFLAGS) minion_test.cpp ../udpsocket/server_udp_socket.cpp ../socket/socket.cpp ../app_protocol/app_protocol.cpp  ../protocol_translator/protocol_translator.cpp ../logger/logger.cpp -lboost_thread -lboost_system -lboost_filesystem -o minion2.out

gc:
	g++ -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 $(OBJ) 

ar: 
	ar rcs vsa.ar $(OBJ) $(DEPS)

sl:
	

clean: 
	rm -f *.o *.out *.gch