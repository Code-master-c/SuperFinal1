#include <vector>
#include <stdint.h>
#include <iostream>
#include <bitset>
#include <sstream>

using Int = uint64_t;
using LInt = std::vector<uint64_t>;

static bool bit_repr = false;

std::ostream& operator<<(std::ostream& out, const LInt& val) {
	LInt v = val;
	while (v.size() && !v.back())
		v.erase(v.end() - 1);

	for (auto it = v.rbegin(); it != v.rend(); ++it) {
		if (bit_repr) {
			out << std::bitset<64>(*it);
			out << "|";
		}
		else {
			out << *it;
			//out << "|";
		}
	}
	return out;
}

static bool debug = true;



uint64_t table[64];

bool get_bit(Int val, uint8_t bit) {
	if (bit >= 64)
		return 0;
	return val & table[bit];
}

bool get_bit(const LInt& val, uint8_t bit) {
	if (bit >= val.size() * 64)
		return 0;
	return val[bit / 64] & table[bit % 64];
}

void set_bit(LInt& val, uint8_t bit, bool value) {
	if (val.size() <= bit / 64)
		val.resize(bit / 64 + 1);
	if (!value)
		val[bit/64] &= ~table[bit % 64];
	else
		val[bit/64] |= table[bit % 64];
}

void simplify(LInt& val) {
	LInt v = val;
	while (v.size() && !v.back())
		v.erase(v.end() - 1);
	val = v;
}

/// ��������� � t ����� value << shift
void add(LInt& t, Int value, uint16_t shift) {
	uint64_t c = 0;
	for (size_t i = 0; i < 64; ++i) {
		int s = get_bit(t, i + shift) + get_bit(value, i) + c;
		set_bit(t, i + shift, s & table[0]);
		c = s >> 1;
	}
	if (c) {
		int i = 64;
		while (c) {
			int s = get_bit(t, i + shift) + c;
			set_bit(t, i + shift, s & table[0]);
			c = s >> 1;
			++i;
		}
	}
}


LInt multiply(const LInt& a, const LInt& b) {
	LInt a_ = a;
	LInt b_ = b;
	simplify(a_);
	simplify(b_);
	LInt res;
	res.resize(a_.size() + b_.size());
	
	for (size_t i = 0; i < b_.size() * 64; ++i) {
		if (get_bit(b_, i)) {
			for (size_t block = 0; block < a_.size(); ++block)
				add(res, a_[block], i + block * 64);
			//std::cout << "res = " << res << std::endl;		
		}
	}
	return res;
}


LInt operator*(const LInt& a, const LInt& b) {
	return multiply(a, b);
}


LInt factorial(size_t n) {
	if (n <= 1) {
		return LInt{ 1 };
	}
	return factorial(n - 1) * LInt { n };
}


int main(int argc, char* argv[]) {
	for (size_t i = 0; i < 64; ++i) {
		table[i] = static_cast<Int>(1) << i;
	}

	if (argc == 2) {
		std::stringstream ss(argv[1]);
		int f;
		ss >> f;
		std::cout << factorial(f) << std::endl;
	}
	else {
		for (size_t i = 0; i <= 50; ++i) {
			std::cout << i << "! = " << factorial(i) << std::endl;
		}
	}

	return 0;
}