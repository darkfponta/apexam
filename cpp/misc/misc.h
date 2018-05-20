#ifndef __MISC_H__
#define __MISC_H__

#include <algorithm>
#include <chrono>
#include <random>
#include <ctime>

/*int randomgen(const int i) {
	return std::rand() / ((RAND_MAX + 1u) / i);
}*/

bool mycomparison(const size_t& a, const size_t& b) {
	return a > b;
}

class RndGen {
	std::default_random_engine _gen;
	uniform_int_distribution<size_t> _dist;
public:
	RndGen(size_t& i) : _gen{ static_cast<size_t>(std::time(nullptr)) }, _dist { 0, i } {};

	size_t operator()() { return _dist(_gen); };
};

#define TIME std::chrono::high_resolution_clock::now()

#endif
