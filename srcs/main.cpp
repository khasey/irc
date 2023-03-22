#include "../inc/irc.hpp"

int main(int ac, char **av){
	Socket b;

	if (!init(ac, av, b))
		return (FAIL);
	pollLoop(b, av[2]);
	close(b.getSockFd());
	return 0;
}