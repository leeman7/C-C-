list:
	@echo "------------LISTING FILES------------"
	ls -hal
clean:
	rm synflood
	rm usr-info
	rm polish_calc
	rm gauss_sum
	rm cat
	rm amdahls_law
	rm temp_convert
	rm stat
	rm find
	rm sort
	rm pipe
	rm fork
	rm shhh

master:
	gcc -Wall -g -o synflood synflood.c
	gcc -Wall -g -o usr-info usr-info.c
	# gcc -Wall -g -o stat stat.c
	gcc -Wall -g -o temp_convert temp_convert.c
	gcc -Wall -g -o gauss_sum gauss_sum.c
	gcc -Wall -g -o law amdahls_law.c
	gcc -Wall -g -o find find.c
	gcc -Wall -g -o sort sort.c
	gcc -Wall -g -o pipe pipe.c
	gcc -Wall -g -o fork fork.c
	gcc -Wall -g -o shhh shhh.c

flood:
	gcc -Wall -g -o synflood synflood.c
	./synflood
info:
	gcc -Wall -g -o usr-info usr-info.c
	./usr-info
stat:
	gcc -Wall -g -o stat stat.c
	./stat
temp:
	gcc -Wall -g -o temp_convert temp_convert.c
	./temp_convert
law:
	gcc -Wall -g -o law amdahls_law.c
	./law
sum:
	gcc -Wall -g -o gauss_sum gauss_sum.c
	./gauss_sum
shhh:
	gcc -Wall -g -o shhh shhh.c
	./shhh

