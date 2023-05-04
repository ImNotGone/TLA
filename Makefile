# Check if the filename argument was passed for the run rule
ifeq ($(filter run,$(MAKECMDGOALS)),run)
  ifeq ($(FILE),)
    $(error Please specify a file to run: make run FILE=<file>)
  endif
endif

all: clean
	./script/build.sh

run: $(FILE)
	@if [ ! -f $(FILE) ]; then \
    	echo "$(FILE) is not a file"; exit 1; \
	fi
	./script/start.sh $(FILE)

debug: clean
	./script/build.sh debug

test: all
	./script/test.sh

clean:
	rm -rf bin
	rm -f src/frontend/lexical-analysis/flex-scanner.c
	rm -f src/frontend/syntactic-analysis/bison-parser.c
	rm -f src/frontend/syntactic-analysis/bison-parser.h

.PHONY: all run debug test clean
