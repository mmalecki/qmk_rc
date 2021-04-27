CC=clang-11
AR=llvm-ar-11

OBJS += qmk_rc.o

TESTS += test/test-parser-data test/test-parser-no-data test/test-parser-two-commands test/test-parser-padding test/test-parser-long-data

# We use this Makefile for tests exclusively, therefore we can compile with
# default commands disabled. Production builds are compiled by the QMK build system.
CFLAGS=-g -I. -DQMK_RC_DISABLE_DEFAULT_COMMANDS

libqmkrc.a: $(OBJS)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test/%: test/%.c libqmkrc.a
	$(CC) $(CFLAGS) -o $@ $^

test: $(TESTS)
	./test/run

clean:
	rm -f libqmkrc.a
	rm -r $(OBJS)
	rm -f $(TESTS)

.PHONY: all test clean
