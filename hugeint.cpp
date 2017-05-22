#include "hugeint.h"

#include <stdio.h>

HugeInt::HugeInt()
{
	this->sign = 0;
	this->ints.clear();
}
HugeInt::HugeInt(int origin)
{
	this->ints.clear();
	this->sign = origin?(origin>0?1:-1):0;
	while(origin)
	{
		this->ints.push_back(origin%10000);
		origin/=10000;
	}
}
HugeInt::HugeInt(const HugeInt& origin)
{
	this->sign = origin.sign;
	this->ints = origin.ints;
}
HugeInt::HugeInt(const std::string& source)
{
	int lastLegal = -1;
	int i;
	int tmp=0;
	int number10=1;
	this->ints.clear();
	for(i=source.size()-1;i>=0;--i)
	{
		if(source[i]>='0'&&source[i]<='9')
		{
			tmp += (source[i]-'0')*number10;
			number10*=10;
			if(number10>=10000)
			{
				this->ints.push_back(tmp);
				number10 = 1;
				tmp = 0;
			}
			lastLegal = i;
		}
		else if(source[i] == '-' || source[i] == '+')
		{
			lastLegal = i;
		}
	}
	if(tmp)
		this->ints.push_back(tmp);
	while(!this->ints.empty() && this->ints.back()==0)
		this->ints.pop_back();
	this->sign = this->ints.size()?(source[lastLegal]=='-'?-1:1):0;
}
void HugeInt::deltaInts(int pos,int num)
{
	if(this->ints.size()<=pos)
		this->ints.resize(pos+1,0);
	this->ints[pos]+=num;
}
int HugeInt::getInts(int pos) const
{
	if(this->ints.size()<=pos)
		return 0;
	return this->ints[pos];
}
void HugeInt::setInts(int pos,int num)
{
	if(this->ints.size()<=pos)
		this->ints.resize(pos+1,0);
	this->ints[pos] = num;
}
void HugeInt::trimupInts(int modTop)
{
	if(modTop<0 || modTop>=this->ints.size())
		modTop = this->ints.size()-1;
	int i,tmp;
	for(i=0;i<=modTop;++i)
	{
		if(this->ints[i]>=10000)
		{
			this->deltaInts(i+1,this->ints[i]/10000);
			this->ints[i]%=10000;
		}
		else if(this->ints[i]<0)
		{
			tmp = this->ints[i]/10000+(this->ints[i]%10000?-1:0);
			this->deltaInts(i+1,tmp);
			this->deltaInts(i,10000*(-tmp));
		}
	}
	while(this->getInts(i)>=10000)
	{
		this->deltaInts(i+1,this->ints[i]/10000);
		this->ints[i]%=10000;
		++i;
	}
	while(!this->ints.empty() && this->ints.back()==0)
		this->ints.pop_back();
	if(this->ints.empty())
		this->sign = 0;
}
int HugeInt::convertion_abs(int x)
{
	return x<0?-x:x;
}
HugeInt& HugeInt::operator+=(const HugeInt& delta)
{
	int thisSng, deltaSng;
	if(delta.sign == 0)
		return (*this);
	else if(this->sign == 0)
		return (*this) = delta;
	else if(this->sign == delta.sign)
		thisSng = deltaSng = 1;
	else if(this->sign==1)//negative delta
	{
		if(*this >= -delta)
		{
			thisSng = 1;
			deltaSng = -1;
		}
		else
		{
			this->sign = -1;
			thisSng = -1;deltaSng = 1;
		}
	}
	else // negative this
	{
		if(-(*this)>= delta)
		{
			thisSng = 1;
			deltaSng = -1;
		}
		else
		{
			this->sign = 1;
			thisSng = -1;deltaSng = 1;
		}
	}
	int i=0;
	int sz = delta.ints.size();
	for(;i<sz;++i)
	{
		this->setInts(i,thisSng*this->getInts(i)+deltaSng*delta.getInts(i));
	}
	this->trimupInts(sz);
	return (*this);
}

HugeInt& HugeInt::operator-=(const HugeInt& delta)
{
	return ((*this)+=(-delta));
}

HugeInt& HugeInt::operator*=(const HugeInt& times)
{
	return ((*this) = (*this)*times);
}

HugeInt& HugeInt::operator/=(const HugeInt& div)
{
	return ((*this) = (*this)/div);
}

HugeInt& HugeInt::operator%=(const HugeInt& div)
{
	return ((*this) = (*this)%div);
}

HugeInt HugeInt::operator+(const HugeInt& delta) const
{
	HugeInt rtn(*this);
	return (rtn+=delta);
}

HugeInt HugeInt::operator-(const HugeInt& delta) const
{
	HugeInt rtn(*this);
	return (rtn-=delta);
}

HugeInt HugeInt::operator*(const HugeInt& times) const
{
	if(times.sign==0 || this->sign==0)
		return HugeInt(0);
	HugeInt base = *this;
	HugeInt result(0);
	int i,j,sz1=base.ints.size(),sz2=times.ints.size();
	result.sign = base.sign * times.sign;
	for(i=0;i<sz2;++i)
	{
		for(j=0;j<sz1;++j)
			result.deltaInts(i+j,times.ints[i]*base.ints[j]);
		result.trimupInts();
	}
	return result;
}

HugeInt HugeInt::operator/(HugeInt div) const
{
	HugeInt result(0);
	if(div.sign==0)
		return 0;//or throw error
	if(this->sign==0)
		return result;
	HugeInt base(*this);
	result.sign = this->sign*div.sign;
	base.sign = div.sign = 1;
	while(base>div)
	{
		base -= div;
		++result;
	}
	return result;
}

HugeInt HugeInt::operator%(HugeInt div) const
{
	if(div.sign==0)
		return 0;//or throw error
	if(this->sign==0)
		return (*this);
	HugeInt base(*this);
	base.sign = div.sign = 1;
	while(base>div)
		base-=div;
	base.sign *= this->sign;
	return base;
}

HugeInt& HugeInt::operator=(const HugeInt& origin)
{
	this->sign = origin.sign;
	this->ints = origin.ints;
	return (*this);
}

bool HugeInt::operator==(const HugeInt& target) const
{
	if(this->sign!=target.sign)
		return false;
	if(this->ints.size()!=target.ints.size())
		return false;
	int sz = this->ints.size();
	for(int i=0;i<sz;++i)
		if(this->ints[i]!=target.ints[i])
			return false;
	return true;
}

HugeInt& HugeInt::operator++()
{
	this->deltaInts(0,1);
	this->trimupInts(0);
	return (*this);
}

HugeInt HugeInt::operator++(const int shit)
{
	HugeInt rtn(*this);
	this->deltaInts(0,1);
	this->trimupInts(0);
	return rtn;
}

HugeInt& HugeInt::operator--()
{
	this->deltaInts(0,-1);
	this->trimupInts(0);
	return (*this);
}

HugeInt HugeInt::operator--(const int shit)
{
	HugeInt rtn(*this);
	this->deltaInts(0,-1);
	this->trimupInts(0);
	return rtn;
}


bool HugeInt::operator>(const HugeInt& target) const
{
	if(this->sign>target.sign)
		return true;
	if(this->ints.size()>target.ints.size())
		return this->sign==1;
	int sz = this->ints.size();
	for(int i=sz-1;i>=0;--i)
	{
		if(this->ints[i]==target.ints[i])
			continue;
		else if(this->ints[i]>target.ints[i])
			return this->sign==1;
		else// if(this->ints[i]<target.ints[i])
			return this->sign==-1;
	}
	return false;
}

bool HugeInt::operator>=(const HugeInt& target) const
{
	if(this->sign>target.sign)
		return true;
	if(this->ints.size()>target.ints.size())
		return this->sign==1;
	int sz = this->ints.size();
	for(int i=sz-1;i>=0;--i)
	{
		if(this->ints[i]==target.ints[i])
			continue;
		else if(this->ints[i]>target.ints[i])
			return this->sign==1;
		else// if(this->ints[i]<target.ints[i])
			return this->sign==-1;
	}
	return true;
}

bool HugeInt::operator<(const HugeInt& target) const
{
	if(this->sign<target.sign)
		return true;
	if(this->ints.size()>target.ints.size())
		return this->sign==-1;
	int sz = this->ints.size();
	for(int i=sz-1;i>=0;--i)
	{
		if(this->ints[i]==target.ints[i])
			continue;
		else if(this->ints[i]>target.ints[i])
			return this->sign==-1;
		else// if(this->ints[i]<target.ints[i])
			return this->sign==1;
	}
	return false;
}


bool HugeInt::operator<=(const HugeInt& target) const
{
	if(this->sign<target.sign)
		return true;
	if(this->ints.size()>target.ints.size())
		return this->sign==-1;
	int sz = this->ints.size();
	for(int i=sz-1;i>=0;--i)
	{
		if(this->ints[i]==target.ints[i])
			continue;
		else if(this->ints[i]>target.ints[i])
			return this->sign==-1;
		else// if(this->ints[i]<target.ints[i])
			return this->sign==1;
	}
	return true;
}

HugeInt HugeInt::operator-() const
{
	HugeInt rtn(*this);
	rtn.sign*=-1;
	return rtn;
}

HugeInt::operator bool() const
{
	return this->sign!=0;
}

bool HugeInt::operator!() const
{
	return this->sign == 0;
}
std::string HugeInt::toString() const
{
	std::string rtn;
	char buf[10]={'\0'};
	if(this->sign==0)
	{
		rtn.assign("0");
		return rtn;
	}
	else if(this->sign<0)
		rtn.assign("-");
	sprintf(buf,"%d",this->ints[this->ints.size()-1]);
	rtn.append(buf);
	for(int i=this->ints.size()-2;i>=0;--i)
	{
		sprintf(buf,"%04d",this->ints[i]);
		rtn.append(buf);
	}
	return rtn;
}

std::ostream& operator<<(std::ostream& os,const HugeInt& num)
{
	return os<<num.toString();
}

std::istream& operator>>(std::istream& is,HugeInt& num)
{
	std::string buf;
	is>>buf;
	num = HugeInt(buf);
	return is;
}
//end of file
