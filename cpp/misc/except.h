#ifndef __EXCEPT_H__
#define __EXCEPT_H__
#include <stdexcept>

struct NotFoundException: public std::exception {  // publicly inherited!!!
	const char* what() const noexcept override {
		return "Element not found";
	}
};

struct InvalidRangeException : public std::exception {  // publicly inherited!!!
	const char* what() const noexcept override {
		return "Given range is invalid";
	}
};

struct InvalidLeafException : public std::exception {  // publicly inherited!!!
	const char* what() const noexcept override {
		return "Wrong usage of leaf and parent nodes";
	}
};

#endif
