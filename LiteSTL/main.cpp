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
#endif // TEST_VECTOR

}