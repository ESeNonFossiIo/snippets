.PHONY: all clean test

CXX ?= g++
CXXFLAGS ?= -std=c++20 -Wall -Wextra -O2

SOURCES := $(wildcard *.cpp)
TARGETS := $(SOURCES:.cpp=)

all: $(TARGETS)

%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: all
	@for t in $(TARGETS); do \
	  echo "== $$t =="; \
	  ./$$t; \
	  echo; \
	done

clean:
	rm -f $(TARGETS)
