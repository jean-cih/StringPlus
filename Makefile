GCC = gcc -Wall -Werror -Wextra -std=c11

FILES_TESTS = tests/build_test.c tests/memchr_test.c tests/memcmp_test.c tests/memcpy_test.c tests/memset_test.c tests/strlen_test.c tests/strncat_test.c tests/strchr_test.c tests/strncmp_test.c tests/strncpy_test.c tests/strrchr_test.c tests/strcspn_test.c tests/strstr_test.c tests/strpbrk_test.c tests/strtok_test.c tests/strerror_test.c  tests/test_string.c

FILES_TESTS_ADDIT = tests/to_upper_test.c tests/to_lower_test.c tests/insert_test.c tests/trim_test.c tests/sprintf_test.c tests/sscanf_test.c

FILES_LIB = lib/s21_memchr.c lib/s21_memcmp.c lib/s21_memcpy.c lib/s21_memset.c lib/s21_strlen.c lib/s21_strncat.c lib/s21_strchr.c lib/s21_strncmp.c lib/s21_strncpy.c lib/s21_strrchr.c lib/s21_strstr.c lib/s21_strcspn.c lib/s21_strspn.c lib/s21_strpbrk.c lib/s21_strtok.c lib/s21_strerror.c

FILES_S_ADDIT = lib/s21_to_upper.c lib/s21_to_lower.c lib/s21_insert.c lib/s21_trim.c lib/s21_sprintf.c lib/s21_sprintf_flags.c lib/s21_sscanf.c lib/s21_strcpy.c

GCOV_FLAGS = -fprofile-arcs -ftest-coverage

CHECK_FLAGS = -lcheck -lsubunit -lrt -lpthread -lm -lgcov

all: clean s21_string.a

s21_string.a: s21_string.o
	ar r s21_string.a *.o
	mv s21_string.a lib
	rm -f *.o

s21_string.o:
	@$(GCC) $(GCOV_FLAGS) -c $(FILES_LIB) $(FILES_S_ADDIT)

test: s21_string.a test.o
	@$(GCC) -o test *.o -Llib -l:s21_string.a $(CHECK_FLAGS)
	@./test
	@rm *.o

test.o:
	@$(GCC) -c $(FILES_TESTS) $(FILES_TESTS_ADDIT) 

clean:
	rm -f lib/s21_string.a test
	rm -f *.o *.gcda *.gcno
	rm -rf report

gcov_report:
	rm -rf report
	mkdir report
	@gcovr -r . --html --html-details --html-self-contained -o coverage_report.html
	mv coverage_report* report
	rm -f *.gcda *.gcno

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c lib/*.c tests/*.c

clang:
	cp ../materials/linters/.clang-format ./
	clang-format -n lib/*.c
	clang-format -n tests/*.c
	rm .clang-format
