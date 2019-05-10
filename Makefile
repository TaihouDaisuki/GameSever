MYSQLFLAGS = $$(mysql_config --cflags --libs)
CC = g++


Main:Main.cpp  ./target/Server.o ./target/Log.o  ./target/Mysql.o 
	$(CC)  $< ./target/Server.o ./target/Log.o  ./target/Mysql.o  $(MYSQLFLAGS) -o $@ -std=c++11 > make.log 2>&1


./target/Server.o: ./include/Server.h
	$(CC)  ./src/Server.cpp -c -o ./target/Server.o -std=c++11 

./target/Log.o:./include/Log.h
	$(CC)  ./src/Log.cpp -c -o ./target/Log.o -std=c++11 


./target/Mysql.o:./include/Mysql.h
	$(CC)  ./src/Mysql.cpp -c $(MYSQLFLAGS)  -o ./target/Mysql.o -std=c++11 


.PHONY:clean all
clean:    
	-rm Main ./target/*.o
