CC=g++
CFLAGS=-Wall
LIBS=-lboost_program_options

SRCS = main.cpp\
	IRCBot.cpp\
	IRCSocket.cpp\
	UserStats.cpp\
	CommandHandler.cpp\
	Time.cpp\
	utils.cpp\
	DUMIIFinger.cpp\

OBJS = $(SRCS:.cpp=.o)

HEADERS=IRCBot.h IRCSocket.h ConnectionError.h CommandHandler.h utils.h UserStats.h Time.h DUMIIFinger.h

../Bot: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

