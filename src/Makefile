CC = g++
CFLAGS = -Wall
LIBS = -lboost_program_options

BINARY = Bot
OBJDIR = ../objs
DEPDIR = deps


SRCS = main.cpp\
	IRCBot.cpp\
	Socket.cpp\
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
$(DEPDIR)/%.d: %.cpp
	@mkdir -p $(DEPDIR)
	@echo Generating $@
	@$(CC) -MM $< -MT $(OBJDIR)/$(<:.cpp=.o) -MF $@

include $(SRCS:%.cpp=$(DEPDIR)/%.d)

