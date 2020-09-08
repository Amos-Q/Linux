#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc,char *argv[],char* env[])
{
	int i = 0;
	for(i = 0;i<argc;i++){
		printf("argv[%d]=[%s]\n",i,argv[i]);
	}
	//for(i=0;env[i]!=NULL;i++){
	//	printf("env[%d]=[%s]\n",i,env[i]);
	//}
	//extern char** environ;
	//for(i=0;environ[i]!=NULL;i++){
	//     printf("environ[%d]=[%s]\n",i,environ[i]);
	//}
	//char* hetenv(char* name) - 通过环境变量名称获取指定环境变量的数据
	char * ptr=getenv("MYENV");
	if(ptr!=NULL)
	{
		printf("%s\n",ptr);
	}else
	{
		printf("no");
	}
	return 0;
}
