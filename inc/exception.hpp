#ifndef CEXCEPTION_HPP
#define CEXCEPTION_HPP

#include <iostream>
#include <exception>

struct MyException : public std::exception {
	const char * what () const throw () {
		return "Exception catch";
	}
};

#endif