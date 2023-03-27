#include "../../inc/irc.hpp"

static int trimFirstSpace(int fd, std::string &s);

// CMD = OP test <user to promot> <reason>
void op(Server &s, int fd, std::string cmd){

	// If the command is empty, return
	if (cmd.empty())
		return;
	// If the command starts with a space, remove it
	if (trimFirstSpace(fd, cmd))
		return;
	size_t i = 0;
	size_t pos = 0;
	std::string data[3];

	// Split the command in 3 parts: the channel, the user to promot and the reason
	while ((pos = cmd.find(" ")) != std::string::npos){
		data[i] = cmd.substr(0, pos);
		cmd.erase(0, pos + 1);
		i++;
	}
	data[i] = cmd.substr(0, pos);
	std::list<std::string>				chanRights = s.getClients(fd)->getChanRights();
	std::list<std::string>::iterator 	itl;

	// Check if the user is authorized to promot someone
	for (itl = chanRights.begin(); itl != chanRights.end(); ++itl)
		if (*itl == data[1]){
			return;
		}

	// Check if the user is in the channel
	//get the list of channels the client is connected to
	std::list<std::string>::iterator it;
	for (it = s.getClients(fd)->getChanRights().begin(); it != s.getClients(fd)->getChanRights().end(); it++)
	{
		if(s.getClientsUser(data[1]) == NULL)
		{
			send(fd, "Error : This user doesnt exists.\n", strlen("Error : This user doesnt exists.\n"), 0);
			break;
		}
			
	//if the client is connected to the channel that the operator wants to promot the user from
		if (*it == data[0])
		{
			
			//promot the user from the channel
			Client* client;
			client = s.getClientsUser(data[1]);
			client->promot(data[0]);
			
			std::string msg;
			//send a message to the user telling him he has been promoted
			msg = ":you have been promoted to Operator of <";
			msg += data[0];
			msg += "> chan ";
			msg += data[2];
			msg += "\n";
			send(s.getClientsUser(data[1])->getClientSocket(), msg.c_str(), msg.length(), 0);
			break;
		}
	}
}



void ign(Server &s, int fd, std::string n){
	(void)s;
	(void)n;
	send(fd, "PONG 127.0.0.1\n", strlen("PONG 127.0.0.1\n"), 0);
	std::cout << "                     O 					" << std::endl;
    std::cout << "                   _/|)_-O				" << std::endl;
    std::cout << "                  ___|_______				" << std::endl;
    std::cout << "                 (     |   o )			" << std::endl;
    std::cout << "                (      |      )			" << std::endl;
    std::cout << "               #################			" << std::endl;
    std::cout << "              (   _ ( )|        )			" << std::endl;
    std::cout << "             (   ( ) | |         )		" << std::endl;
    std::cout << "            (  (  |_/  |          )		" << std::endl;
    std::cout << "           (____(/|____|___________)		" << std::endl;
    std::cout << "              |   |             |			" << std::endl;
    std::cout << "              |  / )            |			" << std::endl;
    std::cout << "              | /   )           |			" << std::endl;
    std::cout << "              _/    /_					" << std::endl;
	std::cout << "                     						" << std::endl;
	std::cout << "		88888b.  .d88b. 88888b.  .d88b. 	" << std::endl;
	std::cout << "		888  88bd88""88b888  88bd88P 88b	" << std::endl;
	std::cout << "		888  888888  888888  888888  888	" << std::endl;
	std::cout << "		888 d88PY88..88P888  888Y88b 888	" << std::endl;
	std::cout << "		88888P    Y88P  888  888  Y88888 	" << std::endl;
	std::cout << "		888                          888 	" << std::endl;
	std::cout << "		888                     Y8b d88P 	" << std::endl;
	std::cout << "		888                       Y88P   	" << std::endl;
}




// Connection registration

void pass(Server &s, int fd, std::string pwd){
	if (trimFirstSpace(fd, pwd))
		return;
	if (s.getClients(fd)->getPass()){
		send(fd, ERR_ALREADYREGISTRED, sizeof(ERR_ALREADYREGISTRED), 0);
		return;
	}
	if (s.getPwd() == pwd){
		send(fd, "SUCCESS\n", strlen("SUCCESS\n"), 0);
		s.getClients(fd)->setPass(1);
	}else
		send(fd, ERR_PASSWDMISMATCH, sizeof(ERR_PASSWDMISMATCH), 0);
}
void nick(Server &s, int fd, std::string nick){
	Client *cl;

	if (trimFirstSpace(fd, nick))
		return;
	cl = s.getClients(fd);
	if (cl)
		cl->setNickname(nick.substr(0, nick.find("\n")));
}
void user(Server &s, int fd, std::string user){
	Client *cl;

	if (trimFirstSpace(fd, user))
		return;
	cl = s.getClients(fd);
	if (cl){
		// if (cl->getRegister()){
		// 	send(fd, ERR_ALREADYREGISTRED, sizeof(ERR_ALREADYREGISTRED), 0);
		// 	return ;
		// }
		if (cl->getNickname().empty()){
			send(fd, ERR_NONICKNAMEGIVEN, sizeof(ERR_NONICKNAMEGIVEN), 0);
			return;
		}
		cl->setUsername(user.substr(0, user.find("\n")));
		welcomeMsg(fd, s, *cl);
		if (!cl->getRegister()){
			cl->setRegister(true);
		}
	}
}

// Channel operations

static void sendMsgChan(std::string const &input, Server &s, int fd, std::string const chan){
	std::list<Client *> *cl = s.getNames(chan);
	std::list<Client *>::iterator it;

	if (!cl)
		return;
	for (it = cl->begin(); it != cl->end(); it++){
		if ((*it)->getClientSocket() == fd)
			continue;
		send((*it)->getClientSocket(), input.c_str(), input.length(), 0);
		send((*it)->getClientSocket(), "\n", 1, 0);
	}
}
void join(Server &s, int fd, std::string channel){
	std::string msg;

	if (trimFirstSpace(fd, channel))
		return;
	s.addChannel(new Channel(channel));
	s.addChannelUser(channel, s.getClients(fd));
	msg = ":local JOIN ";
	msg += channel;
	msg += "\n";
	sendMsgChan(msg, s, fd, channel);
	send(fd, msg.c_str(), msg.length(), 0);

	msg = "331    RPL_NOTOPIC ";
	msg += channel;
	msg += " :No topic is set\r\n";
	send(fd, msg.c_str(), msg.length(), 0);
	msg = " ";
	msg += channel;
	names(s, fd, msg);
}
void part(Server &s, int fd, std::string channel){
	if (trimFirstSpace(fd, channel))
		return;
	s.rmChannelUser(channel, s.getClients(fd));
}
void names(Server &s, int fd, std::string channel){
	std::list<Client *> *names;
	std::list<Client *>::iterator it;
	std::string msg;

	if (trimFirstSpace(fd, channel))
		return;
	names = s.getNames(channel);
	if (!names){
		send(fd, ERR_NOSUCHSERVER, sizeof(ERR_NOSUCHSERVER), 0);
		return;
	}
	if ((*names).size()){
		msg = ":local 353    RPL_NAMREPLY (\"=\")";
		msg += channel;
		msg += " :";
		for (it = names->begin(); it != names->end(); it++){
			msg += " ";
			msg += (*it)->getNickname();
		}
		msg += "\r\n";
		send(fd, msg.c_str(), msg.length(), 0);
	}
	msg = ":local 366    RPL_ENDOFNAMES ";
	msg += channel;
	msg += " :End of NAMES list\r\n";
	send(fd, msg.c_str(), msg.length(), 0);
}
void list(Server &s, int fd, std::string channel){
	std::map<Channel *, std::list<Client *> *> channelList;
	std::map<Channel *, std::list<Client *> *>::iterator it;
	std::string msg;

	channelList = s.getChanList();
	if (!channelList.empty()){
		msg = ":local 322    RPL_LIST";
		msg += channel;
		msg += " :";
		for (it = channelList.begin(); it != channelList.end(); ++it){
			msg += " ";
			msg += it->first->getName();
		}
		msg += "\r\n";
		send(fd, msg.c_str(), msg.length(), 0);
	}

	msg = ":local 323    RPL_LISTEND ";
	msg += " :End of LIST\r\n";
	send(fd, msg.c_str(), msg.length(), 0);
	(void)channel;
}
void kick(Server &s, int fd, std::string input){
	// Check if the user is not authorized to kick someone
	if (trimFirstSpace(fd, input) || s.getClients(fd)->getChanRights().empty())
		return;

	size_t i = 0;
	size_t pos = 0;
	std::string data[3];

	// Split the command in 3 parts: the channel, the user to kick and the reason
	while ((pos = input.find(" ")) != std::string::npos){
		data[i] = input.substr(0, pos);
		input.erase(0, pos + 1);
		i++;
	}
	data[i] = input.substr(0, pos);
	std::list<std::string>				chanRights = s.getClients(fd)->getChanRights();
	std::list<std::string>::iterator 	itl;

	// Check if the user is authorized to kick someone
	for (itl = chanRights.begin(); itl != chanRights.end(); ++itl)
		if (*itl == data[1]){
			return;
		}
	std::list<std::string>::iterator it;

	// Check if the user is in the channel
	//get the list of channels the client is connected to
for (it = s.getClients(fd)->getChanRights().begin(); it != s.getClients(fd)->getChanRights().end(); it++)
{
	if(s.getClientsUser(data[1]) == NULL)
	{
		send(fd, "Error : This user doesnt exists.\n", strlen("Error : This user doesnt exists.\n"), 0);
		break;
	}
	//if the client is connected to the channel that the operator wants to kick the user from
	if (*it == data[0])
	{
		//remove the user from the channel
		s.rmChannelUser(data[0], s.getClientsUser(data[1]));
		std::string msg;
		//send a message to the user telling him he has been kicked
		msg = ":you have been kicked from <";
		msg += data[0];
		msg += "> chan ";
		msg += data[2];
		msg += "\n";
		send(s.getClientsUser(data[1])->getClientSocket(), msg.c_str(), msg.length(), 0);
		break;
	}
}
}
//Sending messages

void privmsg(Server &s, int fd, std::string targetAndText){
	std::string target;
	std::string textToSend;
	std::string msg;
	Client *c;
	std::list<Client *> *cl;

	if (trimFirstSpace(fd, targetAndText))
		return;
	try{
		target = targetAndText.substr(0, targetAndText.find(" "));
		textToSend = targetAndText.substr(target.length() + 1);
	}catch (std::exception &e){
		std::cout << e.what() << std::endl;
		return;
	}
	c = s.getClients(target);
	cl = s.getNames(target);
	if (c || cl){
		if (cl && !c){
			if (!s.clientOnChan(s.getClients(fd)->getUsername(), target)){
				send(fd, ERR_CANNOTSENDTOCHAN, sizeof(ERR_CANNOTSENDTOCHAN), 0);
				return;
			}
		}
		if (textToSend.empty()){
			send(fd, ERR_NOTEXTTOSEND, sizeof(ERR_NOTEXTTOSEND), 0);
			return;
		}
		std::string nick = s.getClients(fd)->getNickname();
		msg = ":";
		msg += s.getClients(fd)->getID();
		msg += " PRIVMSG ";
		msg += nick;
		msg += " :";
		msg += textToSend;
		msg += "\n";
		if (c)
			send(c->getClientSocket(), msg.c_str(), msg.length(), 0);
		else
			sendMsgChan(msg, s, fd, target);
	}else{
		send(fd, ERR_NORECIPIENT, sizeof(ERR_NORECIPIENT), 0);
		return;
	}
}
void notice(Server &s, int fd, std::string targetAndText){
	std::string target;
	std::string textToSend;
	std::string msg;
	Client *c;
	std::list<Client *> *cl;

	if (trimFirstSpace(fd, targetAndText))
		return;
	try{
		target = targetAndText.substr(0, targetAndText.find(" "));
		textToSend = targetAndText.substr(target.length() + 1);
	}catch (std::exception &e){
		std::cout << e.what() << std::endl;
		return;
	}
	c = s.getClients(target);
	cl = s.getNames(target);
	if (c || cl){
		if (cl && !c){
			if (!s.clientOnChan(s.getClients(fd)->getUsername(), target)){
				return;
			}
		}
		if (textToSend.empty()){
			return;
		}
		std::string nick = s.getClients(fd)->getNickname();
		msg = ":";
		msg += s.getClients(fd)->getID();
		msg += "\e[31m NOTICE by \e[0m ";
		msg += nick;
		msg += " :";
		msg += textToSend;
		msg += "\n";
		if (c)
			send(c->getClientSocket(), msg.c_str(), msg.length(), 0);
		else
			sendMsgChan(msg, s, fd, target);
	}else{
		return;
	}
}


static int trimFirstSpace(int fd, std::string &s){
	try{
		s = s.substr(1);
	}catch (std::exception const &e){
		send(fd, ERR_NEEDMOREPARAMS, sizeof(ERR_NEEDMOREPARAMS), 0);
		return 1;
	}
	if (s.empty()){
		send(fd, ERR_NEEDMOREPARAMS, sizeof(ERR_NEEDMOREPARAMS), 0);
		return 1;
	}
	return 0;
}