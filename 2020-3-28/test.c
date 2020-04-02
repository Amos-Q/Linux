#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>


int main()
{
	while(1){
		//增加一个shell提示
		printf("[qc@minshell]$");
		fflush(stdout);//刷新标准输出缓冲区
		//1.等待标准输入
		char buf[1024] = {0};
		fgets(buf,1023,stdin);
		buf[strlen(buf)-1]= '\0';//buf[...]='\n'
		//1.5解析重定向
		//ls -l > a.txt
		char* ptr = buf;
		int redirect_flag = 0;
		char* redirect_file =NULL;
		while(*ptr != '\0'){
			if(*ptr == '>'){
				redirect_flag =1;//这是清空重定向
				*ptr = '\0';//将>替换为结尾标志
				ptr++;
				if(*ptr == '>'){//这是追加重定向
					redirect_flag=2;
					*ptr = '\0';
					ptr++;
				}
				while(*ptr==' '&& *ptr!='\0')ptr++;//将a.txt之前的空格走完
				redirect_file=ptr;//redirect_file这个指针指向a.txt的位置
				while(*ptr!=' '&& *ptr!='\0')ptr++;//将a.txt字符走完
			}
			ptr++;
		}
		//2.对输入数据进行解析                            
		char *argv[32] = {NULL};                     
		int argc = 0;
		ptr = buf; 
		//[    ls    -a    -l   ] 
		while(*ptr != '\0'){           
			if(*ptr != ' '){                                       
				argv[argc] = ptr;
				argc++;                                          
				while(*ptr != ' '&& *ptr != '\0'){        
					ptr++;
				}
				*ptr = '\0';
			}
			ptr++;
		}
		argv[argc]=NULL;//最后一个参数得下一个位置置NULL
		//3.创建子进程 4.在子进程中程序替换  
		pid_t pid = fork();                                              
		if(pid == 0){  
			if(redirect_flag==1){//清空重定向
				int fd = open(redirect_file,O_WRONLY|O_CREAT|O_TRUNC,0664);
				dup2(fd,1);//将标准输出重定向到redirect_file，原本要打印的数据就会被写入文件
			}else if(redirect_flag==2){//追加重定向
				int fd = open(redirect_file,O_WRONLY|O_CREAT|O_APPEND,0664);
				dup2(fd,1);
			}
			//execvp(char *file,char *argv[])  file--新程序名称
			execvp(argv[0],(char**)argv);//程序替换成功就去运行新程序了，32行以后就不会运行了                                                           
			//能走到33行，那么肯定程序替换失败了
			perror("execvp error");
			exit(0);
		}
		//5.进程等待
		wait(NULL);
	}
	return 0;
}
