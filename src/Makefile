CC = g++
CFLAGS = -Wall
LIBS = -lboost_program_options

BINARY = Bot


SRCS = main.cpp\
	IRCBot.cpp\
	IRCSocket.cpp\
	UserStats.cpp\
	CommandHandler.cpp\
	Time.cpp\
	utils.cpp\
	DUMIIFinger.cpp

OBJS = $(SRCS:.cpp=.o)


$(BINARY): $(OBJS)
	@echo Linking $(BINARY)
	@$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<


# Dependency handling
deps/%.d: %.cpp
	@mkdir -p deps
	@echo Generating $@
	@$(CC) -MM $< > $@

include $(SRCS:%.cpp=deps/%.d)

