CC=gcc
CFLAGS=-O0 -Werror=vla -std=gnu11 -g -fsanitize=address -pthread -lrt -lm
PERFFLAGS=-O0 -march=native -Werror=vla -std=gnu11 -pthread -lrt -lm
TESTFLAGS=-O0 -Werror=vla -std=gnu11 -g -fprofile-arcs -ftest-coverage -fsanitize=address -pthread -lrt -lm
RUNFLAGS=-O0 -lbtreestore -g -fsanitize=address -fprofile-arcs -ftest-coverage -pthread -lm 
NAME=btreestore
OBJECT=lib$(NAME).o
LIBRARY=lib$(NAME).a

correctness: btreestore.c
	$(CC) -c $(CFLAGS) $^ -o $(OBJECT)
	ar rcs $(LIBRARY) $(OBJECT)

performance: btreestore.c
	$(CC) -c $(PERFFLAGS) $^ -o $(OBJECT)
	ar rcs $(LIBRARY) $(OBJECT)

tests: btreestore.c
	$(CC) -c $(TESTFLAGS) $^ -o $(OBJECT)
	ar rcs $(LIBRARY) $(OBJECT)

run_tests: btreestore.c
	$(CC) -c $(RUNFLAGS) $^ -o $(OBJECT)
	bash test.sh

# run_tests: btreestore.c
# 	$(CC) -c $(TESTFLAGS) $^ -o $(OBJECT)
# 	ar rcs $(LIBRARY) $(OBJECT)
# 	bash test.sh


