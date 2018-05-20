#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <ctime>
#include "bst.h"
#include "misc.h"
#include <random>

#define MIN_EXP 8
#define MAX_EXP 24

#define TESTS 100

#define tpair std::pair<size_t,size_t>
#define ttree Bst<size_t,size_t>

int main() {

	std::ofstream out;
	out.open("times.txt");

	out << "exp;prepost;depth;avgdepth";
	for (size_t c = 1; c <= TESTS; ++c)
		out << ";" << "val" << c;

	out << endl;

	for (size_t n = MIN_EXP; n <= MAX_EXP; ++n) { // exponent: 2^n = tree size

		size_t siz = 1 << n;

		cout << "Using size " << siz << endl;

		RndGen rndgen{siz};  // seed is set internally

		vector<tpair> v;
		v.reserve(siz);
		for (size_t i = 0; i < siz; ++i)
			v.emplace_back(make_pair(i, i));
		shuffle(v.begin(), v.end(),
			std::default_random_engine(static_cast<size_t>(std::chrono::system_clock::now().time_since_epoch().count())));

		ttree t{ v }; // our tree

		cout << "Effective size " << t.size() << endl;

		vector<size_t> vals;
		vals.reserve(TESTS);

		for (size_t c = 0; c < TESTS; ++c) { // 100 trials for each exponent
			vals.emplace_back(rndgen());
		}

		cout << "before balance: ";
		out << n << ";" << 0 << ";" << t.depth() << ";" << t.avgdepth();

		auto loopstart = TIME;

		for (size_t c = 0; c < TESTS; ++c) {
			auto start = TIME;
			/*auto curval =*/ t.find(vals.at(c));
			auto end = TIME;
			out << ";" << chrono::duration_cast<chrono::nanoseconds>(end-start).count() ;
		}
		out << endl;

		auto loopend = TIME;
		cout << "elapsed time " << chrono::duration_cast<chrono::milliseconds>(loopend - loopstart).count() << "ms" << endl;

		t.balance();

		cout << "after balance: ";
		out << n << ";" << 1 << ";" << t.depth() << ";" << t.avgdepth();

		loopstart = TIME;

		for (size_t c = 0; c < TESTS; ++c) {
			auto start = TIME;
			/*auto curval =*/ t.find(vals.at(c));
			auto end = TIME;
			out << ";" << chrono::duration_cast<chrono::nanoseconds>(end - start).count() ;
		}
		out << endl;

		loopend = TIME;
		cout << "elapsed time " << chrono::duration_cast<chrono::milliseconds>(loopend - loopstart).count() << "ms" << endl;

	}
	out.flush();
	out.close();
	return 0;
}