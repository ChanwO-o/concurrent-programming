#include <stdio.h>

int main(int argc, char * argv[])
{
	int producers;
	int consumers;
	int items;
	int delay;

	if (argc == 5)
	{
		if (atoi(argv[1]) >= 1 && atoi(argv[1]) <= 16)
		{
			producers = atoi(argv[1]);
		}
		if (atoi(argv[2]) >= 1 && atoi(argv[2]) <= 16 &&
			atoi(argv[2]) < (atoi(argv[1]) * atoi(argv[3])))
		{
			consumers = atoi(argv[2]);
		}
		items = atoi(argv[3]);
		delay = atoi(argv[4]);

		printf("%d %d %d %d\n", producers, consumers, items, delay);
	}

    return 0;
}
