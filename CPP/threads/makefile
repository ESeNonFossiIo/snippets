.PHONY: all clean test tsan

CXX ?= g++
CXXFLAGS ?= -std=c++20 -Wall -Wextra -O2
LDLIBS ?= -pthread

SOURCES := $(wildcard *.cpp)
TARGETS := $(SOURCES:.cpp=)

all: $(TARGETS)

%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDLIBS)

test: all
	@for t in $(TARGETS); do \
	  echo "== $$t =="; \
	  ./$$t; \
	  echo; \
	done

tsan: $(SOURCES)
	@for s in $(SOURCES); do \
	  t=$${s%.cpp}; \
	  $(CXX) $(CXXFLAGS) -fsanitize=thread -o $$t.tsan $$s $(LDLIBS); \
	  ./$$t.tsan; \
	done

clean:
	rm -f $(TARGETS) *.tsan
