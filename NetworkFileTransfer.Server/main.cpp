#include <stdio.h>
#include "Server.h"

int main()
{
	Server server("127.0.0.1", "33444", 10);
	server.run();

	system("pause");
	return 0;
}