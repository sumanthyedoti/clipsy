all:
	echo "build | run | clean"

CC: cc

build:
	$(CC) -std=c99 -Wall main.c modules/mpc.c -ledit -lm -o clispy

run:
	./clispy

clean:
	$(RM) clispy
