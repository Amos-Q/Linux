#include"session.h"

void begin_session(session_t *sess)
{
	pid_t pid = fork();
	if(pid == -1)
		ERR_EXIT("fork");
	if(pid == 0)
	{
		//ftp ¡¤t????3¨¬
		handle_child(sess);
	}
	else
	{
		//nobody ??3¨¬
		handle_parent(sess);
	}
}