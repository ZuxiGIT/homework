#include "Number.hpp"


Logger log{"dot_test.dot"};

int main()
{
	{
		LogFunc logfunc(__PRETTY_FUNCTION__);
		Number a;
		Number b(6);
	}
	log.close();
}