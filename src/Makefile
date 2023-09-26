CC = gcc -Wall -Werror -Wextra
SRC = another.c arithmetic.c comparison.c converter.c support.c
TEST_LIBS = -lcheck -lm -lpthread
OBJ = $(SRC:%.c=%.o)
GCOVFLAGS = -fprofile-arcs -ftest-coverage
OS = $(shell uname -s)

ifeq ($(OS), Darwin)
	TEST_LIBS=-lcheck
else
	TEST_LIBS=-lcheck -lsubunit -pthread -lrt -lm
endif

all: clean s21_decimal.a test gcov_report

s21_decimal.a: $(OBJ)
	$(CC) -c $(SRC) $(shell pkg-config --libs --cflags check)
	ar rcs s21_decimal.a $(OBJ)

clean:
	rm -rf *.o
	rm -rf gcov_test
	rm -rf test
	rm -rf *.gcda *.gcno *.info
	rm -rf ./test_output/ 
	rm -rf s21_decimal.a
	rm -rf test_leaks
	rm -rf CPPLINT.cfg
	rm -rf RESULT_VALGRIND.txt
	rm -rf test_leaks.dSYM

test: test.c s21_decimal.a
	$(CC) $(CFLAGS) -c test.c -o test.o $(shell pkg-config --libs --cflags check)
	$(CC) test.o s21_decimal.a $(TEST_LIBS) -o test $(shell pkg-config --libs --cflags check)



gcov_report: test
	$(CC) --coverage test.c $(SRC) s21_decimal.a $(TEST_LIBS) -o gcov_test $(shell pkg-config --libs --cflags check)
	chmod +x *
	./gcov_test
	lcov -t "gcov_test" -o gcov_test.info --no-external -c -d .
	genhtml -o report/ gcov_test.info
	open ./report/index.html


check: test
	cppcheck --enable=all --suppress=missingIncludeSystem --inconclusive --check-config *.c *.h
	cp ../materials/linters/CPPLINT.cfg .
	python3 ../materials/linters/cpplint.py --extension=c *.c *.h *.c
	rm -rf CPPLINT.cfg
	make test

ifeq ($(OS), Darwin)
	leaks --atExit -- test
else
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./test
endif

rebuild: clean all
	open ./report/index.html
