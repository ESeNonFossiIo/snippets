.PHONY: compile


objects := $(patsubst %.hs,%,$(wildcard *.hs))

$(objects): 
	ghc -o $(objects) $(objects).hs
