all:tcp_cli tcp_thread
tcp_thread:tcp_thread.cc
	g++ -std=c++11 $^ -o $@ -lpthread
tcp_process:tcp_process.cc
	g++ -std=c++11 $^ -o $@
tcp_cli:tcp_cli.cc
	g++ -std=c++11 $^ -o $@
tcp_srv:tcp_srv.cc
	g++ -std=c++11 $^ -o $@
