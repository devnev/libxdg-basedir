#include <stdio.h>
#include <string.h>
#include <basedir.h>

void printAndFreeStrings(const char * strings)
{
	const char * ptr = strings;
	if (!strings) return;
	while (*ptr)
	{
		printf("%s\n", ptr);
		ptr += strlen(ptr)+1;
	}
	free(strings);
}

int main(int argc, char* argv[])
{
	const char* result;
	xdgHandle handle = xdgAllocHandle();
	if (!handle) return 1;

	if (argc == 2)
	{
		printf("xdgDataFind:\n");
		printAndFreeStrings(xdgDataFind(argv[1], handle));
		printf("xdgConfigFind:\n");
		printAndFreeStrings(xdgConfigFind(argv[1], handle));
	}
	else if (argc == 3)
	{
		if (strcmp(argv[1], "--data") == 0)
			printAndFreeStrings(xdgDataFind(argv[2], handle));
		else if (strcmp(argv[1], "--config") == 0)
			printAndFreeStrings(xdgConfigFind(argv[2], handle));
		else
			return 2;
	}
	else
		return 2;

	return 0;
}
