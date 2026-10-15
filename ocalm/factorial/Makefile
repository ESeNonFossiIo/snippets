OCAMLOPT := ocamlopt
SRC       := factorial.ml
BIN       := factorial

# Default value for `make run N=...`
N ?= 10

.PHONY: all build run clean

all: build

build: $(BIN)

$(BIN): $(SRC)
	$(OCAMLOPT) -o $(BIN) $(SRC)

run: build
	./$(BIN) $(N)

clean:
	rm -f $(BIN) *.cmi *.cmo *.cmx *.o
