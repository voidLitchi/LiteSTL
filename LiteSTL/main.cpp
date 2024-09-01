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
	std::vector<int> stdv = { 1,3,5 };
	vector<int> v1(stdv.begin(), stdv.end());
	vector<int> v2(v1.rbegin(), v1.rend());
	for (auto p = v2.rbegin(); p != v2.rend(); ++p)
	{
		std::cout << *p << std::endl;
	}
#endif // TEST_VECTOR

}