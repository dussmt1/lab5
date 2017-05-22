#ifndef HUGEINT_H

#define HUGEINT_H

#include <vector>
#include <string>
#include <iostream>

class HugeInt
{
	public:
		HugeInt();
		HugeInt(int origin);
		HugeInt(const HugeInt& origin);
		explicit HugeInt(const std::string& origin);
		HugeInt& operator+=(const HugeInt& delta);
		HugeInt& operator-=(const HugeInt& delta);
		HugeInt& operator*=(const HugeInt& times);
		HugeInt& operator/=(const HugeInt& div);
		HugeInt& operator%=(const HugeInt& div);
		HugeInt operator+(const HugeInt& delta) const;
		HugeInt operator-(const HugeInt& delta) const;
		HugeInt operator*(const HugeInt& times) const;
		HugeInt operator/(HugeInt div) const;
		HugeInt operator%(HugeInt div) const;
		HugeInt& operator=(const HugeInt& origin);
		bool operator==(const HugeInt& target) const;
		HugeInt& operator++();
		HugeInt operator++(const int shit);
		HugeInt& operator--();
		HugeInt operator--(const int shit);
		bool operator>(const HugeInt& target) const;
		bool operator>=(const HugeInt& target) const;
		bool operator<(const HugeInt& target) const;
		bool operator<=(const HugeInt& target) const;
		HugeInt operator-() const;
		explicit operator bool() const;
		bool operator !() const;
		std::string toString() const;
	private:
		int sign;
		std::vector<int> ints;
		static int convertion_abs(int x);
		void deltaInts(int pos,int num);
		void setInts(int pos,int num);
		int getInts(int pos) const;
		void trimupInts(int modTop = -1);
};

std::ostream& operator<<(std::ostream& os,const HugeInt& num);
std::istream& operator>>(std::istream& is,HugeInt& num);

#endif
