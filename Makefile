MYSQLFLAGS = $$(mysql_config --cflags --libs)
CC = g++


Main:Main.cpp ./target/Server.o ./target/Log.o  	
	$(CC)  $< $(MYSQLFLAGS) -o $@ -std=c++11 > log 2>&1


./target/Server.o:
	$(CC)  ./src/Server.cpp -c -o ./target/Server.o -std=c++11 

./target/Log.o:
	$(CC)  ./src/Log.cpp -c -o ./target/Log.o -std=c++11 


.PHONY:clean all
clean:    
	-rm Main ./target/*.o
