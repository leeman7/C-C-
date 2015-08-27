list:
	echo "------------LISTING FILES------------"
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

