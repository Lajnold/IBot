CC=g++
CFLAGS=-Wall
LOADLIBES=-lboost_program_options

SRCS=src/main.cpp src/IRCBot.cpp src/IRCSocket.cpp src/UserStats.cpp src/IRCCommandHandler.cpp src/Time.cpp src/utils.cpp
OBJS=src/main.o src/IRCBot.o src/IRCSocket.o src/UserStats.o src/IRCCommandHandler.o src/Time.o src/utils.o
HEADERS=src/IRCBot.h src/IRCSocket.h src/ConnectionError.h src/IRCCommandHandler.h src/utils.h src/UserStats.h src/Time.h

bot: $(OBJS)
	$(CC) $(CFLAGS) $(SRCS) $(LOADLIBES) -o Bot
