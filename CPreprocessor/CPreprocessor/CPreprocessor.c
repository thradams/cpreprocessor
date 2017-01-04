

#include <stdio.h>
#include "Scanner\Scanner.h"


int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("Prints preprocessed file to console");
		return 1;
	}

	PrintPreprocessedToConsole(argv[1]);

	return 0;
}

