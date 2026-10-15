

########
# JAVA #
########

WDIR=./

JAVAC=javac
JAVA=java

JAVA_EXT=java

JAVA_SRC=$(wildcard *.$(JAVA_EXT))
ALL_JAVA=$(patsubst %.$(JAVA_EXT),%.class,$(JAVA_SRC))

.PHONY: clean all_cuda

all: $(ALL_JAVA)

$(WDIR)/%.class:	%.$(JAVA_EXT)
	$(JAVAC) $<

#########
# Clean #
#########

clean:
	rm -f *.class
