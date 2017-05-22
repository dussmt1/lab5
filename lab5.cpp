#include <iostream>
#include "hugeint.h"

int main()
{
	//std::cout<<"start"<<std::endl;
	HugeInt x;
	//std::cout<<"empty constructed"<<std::endl;
	HugeInt y(28825252);
	//std::cout<<"int constructed"<<std::endl;
	HugeInt z("314159265358979353846");
	//std::cout<<"string constructed"<<std::endl;
	
	std::cin>>x;
	HugeInt result = x+y;
	std::cout<<x<<"+"<<y<<"="<<result<<std::endl;
	
	result = z-x;
	std::cout<<result<<std::endl;

	return 0;
}
