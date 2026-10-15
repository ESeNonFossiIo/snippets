DIR=./

#Compiler
CC=icc
CUDA=nvcc

#Extensions
CC_EXT=c
CUDA_EXT=nvcc

CUDA_SRC=$(wildcard *.$(CUDA_EXT))
ALL_CUDA=$(patsubst %.$(CUDA_EXT),%.x$(CUDA_EXT),$(CUDA_SRC))
C_SRC=$(wildcard *.$(CC_EXT))
ALL_C=$(patsubst %.$(CC_EXT),%.x$(CC_EXT),$(CC_SRC))

.PHONY: clean all_cuda

########
# CUDA #
########
all_cuda: $(ALL_CUDA)

$(DIR)/%.x$(CUDA_EXT):	%.cu
	$(CUDA) $< -o $@

#####
# C #
#####
all_c: $(ALL_C)

$(IDIR)/%.x$(CC_EXT):	%.$(CC_EXT)
	$(CC) $(MKL) $(AVX) -o $@ $<

$(IDIR)/%.d$(CC_EXT):	%.$(CC_EXT)
	$(CC) -D$(DEBUG) $(MKL) $(AVX) -o $@ $<

#########
# Clean #
#########

clean:
	rm -f *.x$(CUDA_EXT) *.x$(CC_EXT)
