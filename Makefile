test:test.c
	gcc -c $^ -o $@
.PHONY:clean
clean:
	rm -rf test

