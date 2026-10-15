LEAN := lean
FILE := basics.lean

.PHONY: all check run clean help

all: check run

check:
	@echo "Checking Lean 4 syntax..."
	$(LEAN) $(FILE)

run:
	@echo "Running Lean 4 program..."
	$(LEAN) --run $(FILE)

clean:
	@echo "Cleaning build artifacts..."
	rm -rf .lake lake-packages *.olean

help:
	@echo "Lean 4 Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  make check   - Type-check the Lean file"
	@echo "  make run     - Run the Lean program"
	@echo "  make all     - Check and run (default)"
	@echo "  make clean   - Remove build artifacts"
	@echo "  make help    - Show this help message"
