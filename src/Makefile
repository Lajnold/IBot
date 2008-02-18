CC = g++
CFLAGS = -Wall
LIBS = -lboost_program_options

BINARY = Bot
OBJDIR = ../objs
DEPDIR = deps

SRCS = main.cpp\
	BotOptions.cpp\
	IRCBot.cpp\
	Socket.cpp\
	CommandHandler.cpp\
	Message.cpp\
	utils.cpp\
	Time.cpp\
	UserStats.cpp\
	DUMIIFinger.cpp\
	Say.cpp\
	OPify.cpp

OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)

$(BINARY): $(OBJS)
	@echo Linking $(BINARY)
	@$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Dependency handling
$(DEPDIR)/%.d: %.cpp
	@mkdir -p $(DEPDIR)
	@echo Generating $@
	@$(CC) -MM $< -MT $(OBJDIR)/$(<:.cpp=.o) -MF $@

include $(SRCS:%.cpp=$(DEPDIR)/%.d)

