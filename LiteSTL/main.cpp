#include<iostream>
#include<algorithm>
//#include<xutility>

#include"general_define.h"

#define TEST_VECTOR

#ifdef TEST_VECTOR
#include"vector.h"
#include<vector>
#endif // TEST_VECTOR

int main()
{
#ifdef TEST_VECTOR
	using NAME_GENERAL_NAMESPACE::vector;
	//testing code here
	vector<int> v = { 3,7,2,9 };
	v.push_back(0);
	v.push_back(5);
	v.push_back(10);
	std::sort(v.begin(), v.end());
	//for (auto p = v.begin(); p != v.end(); ++p)std::cout << *p << std::endl;
	for (auto& x : v)std::cout << x << std::endl;
#endif // TEST_VECTOR

}