CC=gcc -std=c11 -D_GNU_SOURCE
CFLAGS=-Wall -Wextra -Werror
GCOV_LIBS=--coverage
BUILD_PATH=./
SOURCES=s21_decimal.c
H=s21_decimal.h
LIBO=s21_decimal.o
LIBA=s21_decimal.a
EXE=test.out

ifeq ($(OS), Darwin)
	TST_LIBS := -lcheck -lm
	LEAKS := leaks -atExit --  
else
	TST_LIBS := -lcheck -lsubunit -lrt -lm -lpthread -g
	LEAKS := valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --trace-children=yes --track-fds=yes  --log-file=valgrind.log
endif

all: s21_decimal.a test gcov_report

clean:
	@rm -rf *.a && rm -rf *.o && rm -rf *.cfg
	@rm -rf *.info && rm -rf *.gcda && rm -rf *.gcno &&  rm -rf *.gcov
	@rm -rf report/ && rm -rf *.out && rm -rf *.log

test: s21_decimal.a
	@$(CC) $(CFLAGS) test.c -L. $(LIBA) $(TST_LIBS) -o $(BUILD_PATH)$(EXE)
	@$(BUILD_PATH)$(EXE)

rebuild: clean all

s21_decimal.a: clean
	@$(CC) $(CFLAGS) -c $(SOURCES) -o $(BUILD_PATH)$(LIBO)
	@ar rcs $(LIBA) $(LIBO)
	@ranlib $(LIBA)

gcov_report: s21_decimal.a
	$(CC) $(CFLAGS) $(GCOV_LIBS) test.c $(SOURCES) $(LIBA) -L. $(LIBA) $(TST_LIBS) -o $(BUILD_PATH)$(EXE)
	$(BUILD_PATH)$(EXE)
	lcov -t "test" -c -d $(BUILD_PATH) --output-file $(BUILD_PATH)coverage.info
	genhtml $(BUILD_PATH)coverage.info --output-directory $(BUILD_PATH)report/

check:
	@$(LEAKS) ./test.out
	# cppcheck --enable=all --bug-hunting --std=c11 --suppress=missingIncludeSystem *.h *.c 
	@cppcheck --enable=all --std=c11 --suppress=missingIncludeSystem *.h *.c
	@cp ../materials/linters/CPPLINT.cfg .
	@python3 ../materials/linters/cpplint.py --extension=c *.c *.h
