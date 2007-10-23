CC=g++
CFLAGS=-Wall
LIBS=-lboost_program_options

SRCS = src/main.cpp\
	src/IRCBot.cpp\
	src/IRCSocket.cpp\
	src/UserStats.cpp\
	src/CommandHandler.cpp\
	src/Time.cpp src/utils.cpp\
	src/DUMIIFinger.cpp\

OBJS = $(SRCS:.cpp=.o)

HEADERS=src/IRCBot.h src/IRCSocket.h src/ConnectionError.h src/CommandHandler.h src/utils.h src/UserStats.h src/Time.h src/DUMIIFinger.h

Bot: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

