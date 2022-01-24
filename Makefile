CFLAGS = -std=c99 -Wall -Wextra -static -lm $(CFLAG)
CP ?= cp
TIME ?= time
PREFIX ?= /usr/local

SRCDIR = src
LIBDIR = lib
BINDIR = bin
OBJDIR = obj
TSTDIR = test

.PHONY: all test clean install uninstall

all: ${BINDIR}/praytimes

test: ${BINDIR}/lib-praytimes-test ${BINDIR}/lib-praytimes-math-test
	${TIME} ${BINDIR}/lib-praytimes-math-test; \
	${TIME} ${BINDIR}/lib-praytimes-test

clean:
	${RM} -rf ${OBJDIR}/*

install: ${BINDIR}/praytimes
	${CP} ${BINDIR}/praytimes ${PREFIX}/bin/

uninstall:
	${RM} ${PREFIX}/bin/praytimes

${BINDIR}/praytimes: ${OBJDIR}/praytimes-src.o ${OBJDIR}/praytimes-lib.o
	${CC} -o $@ $^ ${CFLAGS}

${BINDIR}/lib-praytimes-test: ${OBJDIR}/lib_praytimes-test.o ${OBJDIR}/praytimes-lib.o
	${CC} -o $@ $^ ${CFLAGS}

${BINDIR}/lib-praytimes-math-test: ${OBJDIR}/lib_praytimes_math-test.o
	${CC} -o $@ $^ ${CFLAGS}

${OBJDIR}/%-src.o: ${SRCDIR}/%.c
	${CC} -o $@ -c -I${LIBDIR} $< ${CFLAGS}

${OBJDIR}/%-lib.o: ${LIBDIR}/%.c
	${CC} -o $@ -c $< ${CFLAGS}

${OBJDIR}/%-test.o: ${TSTDIR}/%.c
	${CC} -o $@ -c $< -I${LIBDIR} ${CFLAGS}
