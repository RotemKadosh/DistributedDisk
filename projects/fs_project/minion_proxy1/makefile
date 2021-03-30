CC=g++
CPPFLAGS= -ansi -Wall -L./monitor_dir -Wl,-rpath=./monitor_dir -ldl -lboost_thread -lboost_chrono -lpthread -lboost_system -lboost_filesystem -g -I ../utils/ -I ../include/ -rdynamic
RASP= ~/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc 
RASP_FLAGS= -ansi -Wall -L./monitor_dir -Wl,-rpath=./monitor_dir -I ../utils/ -I ../include/ -I /home/rotemkadosh27/boost_1_47_0/ -ldl -lboost_thread -lboost_chrono -lpthread -lboost_system -lboost_filesystem -g -rdynamic
DEPS = mimion_proxy.hpp minion_read_cmd.hpp minion_write_cmd.hpp
OBJ = minion_exe.o minion_proxy.o ../storage/storage.o ../socket/socket.o ../udpsocket/server_udp_socket.o ../protocol_translator/protocol_translator.o ../app_protocol/app_protocol.o ../minion_data/minion_data.o ../framework/framework.o ../logger/logger.o ../reactor/reactor.o ../mediator/mediator.o ../thread_pool/thread_pool_impl.o ../dir_monitor_dllloader/dir_monitor_impl.o ../config_manager/config_manager.o
COBJ = minion_exe.o minion_proxy.cpp ../storage/storage.cpp ../socket/socket.cpp ../udpsocket/server_udp_socket.cpp ../protocol_translator/protocol_translator.cpp ../app_protocol/app_protocol.cpp ../minion_data/minion_data.cpp ../framework/framework.cpp ../logger/logger.cpp ../reactor/reactor.cpp ../mediator/mediator.cpp ../thread_pool/thread_pool_impl.cpp ../dir_monitor_dllloader/dir_monitor_impl.cpp ../config_manager/config_manager.cpp
.PHONY = run cgdb vlg clean ar gc

%.o: %.cpp $(DEPS) 
	$(CC) -c -o $@ $< $(CPPFLAGS)

a.out: $(OBJ)
	$(CC) -o $@ $^ $(CPPFLAGS)

run: a.out
	./a.out

cgdb: a.out
	cgdb a.out

vlg: a.out
	valgrind --leak-check=yes --track-origins=yes ./a.out

read_cmd:
	$(CC) $(CPPFLAGS) -I ../include -I ../utils -fPIC -shared -o libreadcmd.so  ../minion_cmd/minion_read_cmd.hpp ../minion_cmd/minion_read_cmd.cpp

write_cmd:
	$(CC) $(CPPFLAGS) -I ../include -I ../fs_project/utils -fPIC -shared -o libwritecmd.so  ../minion_cmd/minion_write_cmd.hpp ../minion_cmd/minion_write_cmd.cpp

rp: 
	$(RASP)$(RASP_FLAGS) $(COBJ) -o rsp_min1.out

rp_run: rp	
	sudo scp ./rsp_min1.out pi@192.168.1.107:~/project/miniom1/

gc:
	g++ -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 $(OBJ) 

ar: 
	ar rcs vsa.ar $(OBJ) $(DEPS)

sl:
	

clean: 
	rm -f *.o *.out *.gch