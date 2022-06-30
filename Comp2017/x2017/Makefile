CC=gcc
CFLAGS=-fsanitize=address -Wvla -Wall -Werror -g -std=gnu11 -lasan

# fill in all your make rules

vm_x2017: vm_x2017
	$(CC) $(CFLAGS) vm_x2017.c -o vm_x2017
	# gcc objdump_x2017.c -o objdump_x2017

objdump_x2017: objdump_x2017
	$(CC) $(CFLAGS) objdump_x2017.c -o objdump_x2017
    # gcc -fsanitize=address -Wvla -Wall -Werror -g -std=gnu11 -lasan $^ objdump_x2017.c -o objdump_x2017
tests:
	echo "tests"

run_tests:
	bash test.sh

clean:
	rm vm_x2017
	rm objdump_x2017

