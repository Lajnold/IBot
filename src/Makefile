CC = g++
CFLAGS = -Wall
LIBS = -lboost_program_options

BINARY = Bot
OBJDIR = ../objs


SRCS = main.cpp\
	IRCBot.cpp\
	IRCSocket.cpp\
	UserStats.cpp\
	CommandHandler.cpp\
	Time.cpp\
	utils.cpp\
	DUMIIFinger.cpp

OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)


$(BINARY): $(OBJS)
	@echo Linking $(BINARY)
	@$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@


# Dependency handling
deps/%.d: %.cpp
	@mkdir -p deps
	@echo Generating $@
	@$(CC) -MM $< > $@

include $(SRCS:%.cpp=deps/%.d)

