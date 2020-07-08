#include <stdio.h>
#include <unistd.h>
int main()
{
	char tmp[11]={0};
	int i;
	for(i=0;i<10;i++)
	{
		tmp[i]='-';
		printf("\r%s",tmp);
		fflush(stdout);
		usleep(1000000);
	}
	return 0;
}
