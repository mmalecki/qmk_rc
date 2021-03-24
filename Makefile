CC=clang-11
AR=llvm-ar-11

OBJS += qmk_rc.o

TESTS += test/test-parser-data test/test-parser-no-data

CFLAGS=-g -I.

libqmkrc.a: $(OBJS)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test/%: test/%.c libqmkrc.a
	$(CC) $(CFLAGS) -o $@ $?

test: $(TESTS)
	./test/run

clean:
	rm -f libqmkrc.a
	rm -r $(OBJS)
	rm -f $(TESTS)

.PHONY: all test clean
