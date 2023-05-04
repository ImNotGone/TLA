
all: clean
	./script/build.sh

test: all
	./script/test.sh

clean:
	rm -f src/frontend/lexical-analysis/flex-scanner.c
	rm -f src/frontend/syntactic-analysis/bison-parser.c
	rm -f src/frontend/syntactic-analysis/bison-parser.h

.PHONY: all test clean
