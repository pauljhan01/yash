yash: main.o command.o job.o
	gcc -o yash main.o  command.o job.o -lreadline

main.o: main.c
	gcc -c main.c

command.o: command.c
	gcc -c command.c 

job.o: job.c
	gcc -c job.c

clean:
	rm -f yash *.o