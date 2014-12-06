#include <stdio.h>
#include "Client.h"

int main()
{
	Client client("127.0.0.1", "33444");
	client.run();
	
	system("pause");
	return 0;
}