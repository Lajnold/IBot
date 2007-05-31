
all: src/main.cpp src/IRCBot.h src/IRCBot.cpp src/IRCSocket.h src/IRCSocket.cpp src/ConnectionError.h src/UserStats.h src/UserStats.cpp src/IRCCommandHandler.h src/IRCCommandHandler.cpp src/Time.h src/Time.cpp src/utils.h src/utils.cpp
	g++ src/main.cpp src/IRCBot.cpp src/IRCSocket.cpp src/IRCCommandHandler.cpp src/UserStats.cpp src/Time.cpp src/utils.cpp -o IRCBot
