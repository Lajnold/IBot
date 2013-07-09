CC = g++
CFLAGS = -Wall -std=gnu++0x -g
LIBS = -lboost_program_options

BINARY = Bot
OBJDIR = objs
DEPDIR = deps

SRCS = main.cpp \
	BotOptions.cpp \
	IRCBot.cpp \
	Socket.cpp \
	CommandHandler.cpp \
	Message.cpp \
	Time.cpp \
	UserStats.cpp \
	DUMIIFinger.cpp \
	Say.cpp \
	OPify.cpp

OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)

$(BINARY): $(OBJS)
	@echo Linking $(BINARY)
	@$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo Cleaning up src/
	rm -rf $(DEPDIR)
	rm -rf $(OBJDIR)
	rm -rf $(BINARY)

# Dependency handling
$(DEPDIR)/%.d: %.cpp
	@mkdir -p $(DEPDIR)
	@echo Generating $@
	@$(CC) -MM $< -MT $(OBJDIR)/$(<:.cpp=.o) -MF $@

include $(SRCS:%.cpp=$(DEPDIR)/%.d)
