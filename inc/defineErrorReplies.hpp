#ifndef DEFINEERRORREPLIES_HPP
#define DEFINEERRORREPLIES_HPP

#define	ERR_NOSUCHSERVER		"402    ERR_NOSUCHSERVER : <server> :No such server\r\n"
#define	ERR_NOSUCHCHANNEL		"403    ERR_NOSUCHCHANNEL : <channel> :No such channel\r\n"
#define	ERR_CANNOTSENDTOCHAN	"404    ERR_CANNOTSENDTOCHAN : <channel> :Cannot send to channel\r\n"
#define	ERR_TOOMANYCHANNELS		"405    ERR_TOOMANYCHANNELS : :You have joined too many channels\r\n"
#define	ERR_NORECIPIENT			"411    ERR_NORECIPIENT :No recipient given \r\n"
#define	ERR_NOTEXTTOSEND		"412    ERR_NOTEXTTOSEND :No text to send\r\n"
#define	ERR_UNKNOWNCOMMAND		"421    ERR_UNKNOWNCOMMAND :cmd :Unknown command\r\n"
#define	ERR_NONICKNAMEGIVEN		"431    ERR_NONICKNAMEGIVEN :No nickname given\r\n"
#define	ERR_NOTREGISTERED		"451    ERR_NOTREGISTERED :You have not registered\r\n"
#define	ERR_NEEDMOREPARAMS		"461    ERR_NEEDMOREPARAMS :Not enough parameters\r\n"
#define	ERR_ALREADYREGISTRED	"462    ERR_ALREADYREGISTRED :Unauthorized command (already registered)\r\n"
#define	ERR_PASSWDMISMATCH		"464    ERR_PASSWDMISMATCH :Wrong password\r\n"

#endif
