#pragma once

// include needed for header
class Server;
#include <string>

enum e_messages{
	E_PASS,
	E_NICK,
	E_USER,
	E_JOIN,
	E_NAMES,
	E_LIST,
	E_PRIVMSG,
	E_NOTICE,
	E_PING,
	E_KICK,
	E_PART,
};

void	ign(Server &s, int z, std::string n); // ignore cmd

// Connection registration
void	pass(Server &s, int fd, std::string pwd);
void	nick(Server &s, int fd, std::string nick);
void	user(Server &s, int fd, std::string user);
	// Server message
	// Operator message
	// Quit message
	// Server Quit message

// Channel operations
void	join(Server &s, int fd, std::string channel);
void	part(Server &s, int fd, std::string channel);
	//Mode message
	//   Channel modes
	//   User modes
	// Topic messag
void	names(Server &s, int fd, std::string channel);
void	list(Server &s, int fd, std::string channel);
	// Invite message
void	kick(Server &s, int fd, std::string channelAndUser);

//Server queries and command
	//Version message
	//Stats message
	//Links message
	//Time message
	//Connect message
	//Trace message
	//Admin message
	//Info messag

//Sending messages
void	privmsg(Server &s, int fd, std::string targetAndText);
void	notice(Server &s, int fd, std::string targetAndText);

//User-based queries
	//Who query
	//Whois query
	//Whowas message

//Miscellaneous messages
	//Kill message
	//Ping message
	//Pong message
	//Error message
