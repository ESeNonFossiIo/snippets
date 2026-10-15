.PHONY: all test clean

SUBDIRS := atomic concepts

all:
	@for dir in $(SUBDIRS); do \
	  echo "Building $$dir..."; \
	  $(MAKE) -C $$dir all; \
	done

test:
	@for dir in $(SUBDIRS); do \
	  echo "\n=== Testing $$dir ===\n"; \
	  $(MAKE) -C $$dir test; \
	done

clean:
	@for dir in $(SUBDIRS); do \
	  $(MAKE) -C $$dir clean; \
	done
