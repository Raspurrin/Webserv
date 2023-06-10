#pragma once
#include <exception>

enum Error {
	Bad_Request,
	Forbidden,
	Not_Found,
	Internal_Error
};

class ErrC: public std::exception {
private:
	Error err;

public:
	ErrC(Error);

	Error getError() const;
};