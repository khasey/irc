#include "../../inc/Channel.hpp"

//	---	---	---	Construcor - Destructor --- --- ---
Channel::Channel(std::string name) : name(name), password(""), currentUsers(0){
}
Channel::~Channel() {}
Channel::Channel (Channel const &rhs){
	*this = rhs;
}

//	---	---	---	Operator --- --- ---

Channel & Channel::operator=(Channel const &rhs){
	name = rhs.getName();
	password = rhs.getPassword();
	currentUsers = rhs.getCurrentUsers();
	return *this;
}
//	---	---	---	Setters --- --- ---

void	Channel::setName(std::string v){
	name = v;
}
void	Channel::setPassword(std::string v){
	password = v;
}
void	Channel::setCurrentUsers(unsigned int v){
	currentUsers = v;
}

// --- --- --- Getters --- --- ---

std::string				Channel::getName() const{
	return name;
}
std::string				Channel::getPassword() const{
	return password;
}
unsigned int			Channel::getCurrentUsers() const{
	return currentUsers;
}
