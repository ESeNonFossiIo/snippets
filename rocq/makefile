########
# ROCQ #
########

WDIR=./
DOCDIR=./html

ROCQ=rocq compile
ROCQDOC=rocq doc

ROCQFLAGS=
ROCQDOCFLAGS=--html --utf8 --interpolate --no-lib-name

V_EXT=v

V_SRC=$(wildcard *.$(V_EXT))
ALL_V=$(patsubst %.$(V_EXT),%.vo,$(V_SRC))

.PHONY: all doc clean

all: $(ALL_V)

%.vo:	%.$(V_EXT)
	$(ROCQ) $(ROCQFLAGS) $<

#######
# Doc #
#######
# Renders the (** ... **) comments to $(DOCDIR)/*.html

doc: $(V_SRC)
	mkdir -p $(DOCDIR)
	$(ROCQDOC) $(ROCQDOCFLAGS) -d $(DOCDIR) $(V_SRC)

#########
# Clean #
#########

clean:
	rm -rf *.vo *.vok *.vos *.glob .*.aux .coq-native .lia.cache $(DOCDIR)
