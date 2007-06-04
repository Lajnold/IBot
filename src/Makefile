CC=g++
CFLAGS=-Wall
LOADLIBES=-lboost_program_options-gcc

SRCS=main.cpp IRCBot.cpp IRCSocket.cpp UserStats.cpp IRCCommandHandler.cpp Time.cpp utils.cpp
OBJS=main.o IRCBot.o IRCSocket.o UserStats.o IRCCommandHandler.o Time.o utils.o
HEADERS=IRCBot.h IRCSocket.h ConnectionError.h IRCCommandHandler.h utils.h UserStats.h Time.h

bot: $(OBJS)
	$(CC) $(SRCS) $(LOADLIBES) -o ../Amon
