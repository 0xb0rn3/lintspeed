# LINTSPEED v2.0 Makefile
# Professional build system with optimization profiles

CC = gcc
TARGET = lintspeed-core
SRC = core.c

# Build profiles
COMMON_FLAGS = -Wall -Wextra -pedantic -pthread
LIBS = -lncurses -lpthread -lm -lrt

# Release build (maximum optimization)
RELEASE_FLAGS = $(COMMON_FLAGS) -O3 -march=native -flto -DNDEBUG -s
RELEASE_LIBS = $(LIBS) -flto

# Debug build (with symbols and sanitizers)
DEBUG_FLAGS = $(COMMON_FLAGS) -O0 -g -fsanitize=address -fsanitize=undefined \
              -DDEBUG -fno-omit-frame-pointer
DEBUG_LIBS = $(LIBS) -fsanitize=address -fsanitize=undefined

# Performance build (profiling support)
PROFILE_FLAGS = $(COMMON_FLAGS) -O2 -pg -fprofile-arcs -ftest-coverage
PROFILE_LIBS = $(LIBS) -lgcov

# Static build (portable binary)
STATIC_FLAGS = $(COMMON_FLAGS) -O3 -march=x86-64 -static
STATIC_LIBS = -static -lncurses -ltinfo -lpthread -lm -lrt

# Installation paths
PREFIX ?= /usr/local
BINDIR = $(PREFIX)/bin
MANDIR = $(PREFIX)/share/man/man1

.PHONY: all release debug profile static install uninstall clean help

all: release

# Default release build
release: $(SRC)
	@echo "Building optimized release binary..."
	$(CC) $(RELEASE_FLAGS) -o $(TARGET) $(SRC) $(RELEASE_LIBS)
	@strip $(TARGET)
	@echo "✓ Built $(TARGET) (release)"
	@ls -lh $(TARGET)

# Debug build with sanitizers
debug: $(SRC)
	@echo "Building debug binary with sanitizers..."
	$(CC) $(DEBUG_FLAGS) -o $(TARGET)-debug $(SRC) $(DEBUG_LIBS)
	@echo "✓ Built $(TARGET)-debug"
	@echo "Run with: ASAN_OPTIONS=detect_leaks=1 ./$(TARGET)-debug"

# Profile-guided optimization build
profile: $(SRC)
	@echo "Building with profiling support..."
	$(CC) $(PROFILE_FLAGS) -o $(TARGET)-profile $(SRC) $(PROFILE_LIBS)
	@echo "✓ Built $(TARGET)-profile"
	@echo "Run test workload, then use 'make pgo' to optimize"

# Generate PGO optimized build
pgo: profile
	@echo "Running profiling workload..."
	./$(TARGET)-profile --quick || true
	@echo "Building PGO-optimized binary..."
	$(CC) $(RELEASE_FLAGS) -fprofile-use -o $(TARGET) $(SRC) $(RELEASE_LIBS)
	@strip $(TARGET)
	@rm -f $(TARGET)-profile *.gcda *.gcno
	@echo "✓ Built PGO-optimized $(TARGET)"

# Static binary (portable across Linux distros)
static: $(SRC)
	@echo "Building static binary..."
	$(CC) $(STATIC_FLAGS) -o $(TARGET)-static $(SRC) $(STATIC_LIBS)
	@strip $(TARGET)-static
	@echo "✓ Built $(TARGET)-static (portable)"
	@ls -lh $(TARGET)-static

# Hardened build (security features)
hardened: $(SRC)
	@echo "Building hardened binary..."
	$(CC) $(RELEASE_FLAGS) \
	    -D_FORTIFY_SOURCE=2 \
	    -fstack-protector-strong \
	    -fPIE -pie \
	    -Wl,-z,relro,-z,now \
	    -o $(TARGET)-hardened $(SRC) $(RELEASE_LIBS)
	@strip $(TARGET)-hardened
	@echo "✓ Built $(TARGET)-hardened (security hardened)"

# Size-optimized build
tiny: $(SRC)
	@echo "Building size-optimized binary..."
	$(CC) $(COMMON_FLAGS) -Os -DNDEBUG -o $(TARGET)-tiny $(SRC) $(LIBS)
	@strip --strip-all $(TARGET)-tiny
	@upx --best --lzma $(TARGET)-tiny 2>/dev/null || true
	@echo "✓ Built $(TARGET)-tiny (minimal size)"
	@ls -lh $(TARGET)-tiny

# Install to system
install: release
	@echo "Installing to $(BINDIR)..."
	install -d $(BINDIR)
	install -m 755 $(TARGET) $(BINDIR)/lintspeed
	@if command -v setcap >/dev/null 2>&1; then \
		echo "Granting CAP_NET_RAW capability..."; \
		setcap cap_net_raw+ep $(BINDIR)/lintspeed || true; \
	fi
	@echo "✓ Installed to $(BINDIR)/lintspeed"

# Uninstall from system
uninstall:
	@echo "Removing $(BINDIR)/lintspeed..."
	rm -f $(BINDIR)/lintspeed
	@echo "✓ Uninstalled"

# Clean build artifacts
clean:
	rm -f $(TARGET) $(TARGET)-* *.o *.gcda *.gcno gmon.out *.dSYM
	@echo "✓ Cleaned build artifacts"

# Run static analysis
analyze: $(SRC)
	@echo "Running static analysis..."
	cppcheck --enable=all --inconclusive --std=c11 $(SRC)
	@if command -v clang-tidy >/dev/null 2>&1; then \
		clang-tidy $(SRC) -- $(COMMON_FLAGS); \
	fi

# Run tests
test: debug
	@echo "Running test suite..."
	./$(TARGET)-debug --quick
	@echo "✓ Tests passed"

# Generate compilation database for IDE support
compile_commands.json: $(SRC)
	@echo "Generating compilation database..."
	@echo '[' > compile_commands.json
	@echo '  {' >> compile_commands.json
	@echo '    "directory": "$(PWD)",' >> compile_commands.json
	@echo '    "command": "$(CC) $(RELEASE_FLAGS) -c $(SRC)",' >> compile_commands.json
	@echo '    "file": "$(SRC)"' >> compile_commands.json
	@echo '  }' >> compile_commands.json
	@echo ']' >> compile_commands.json
	@echo "✓ Generated compile_commands.json"

# Help target
help:
	@echo "LINTSPEED v2.0 Build System"
	@echo ""
	@echo "Available targets:"
	@echo "  make              - Build optimized release binary (default)"
	@echo "  make release      - Build optimized release binary"
	@echo "  make debug        - Build debug binary with sanitizers"
	@echo "  make profile      - Build with profiling support"
	@echo "  make pgo          - Build PGO-optimized binary"
	@echo "  make static       - Build portable static binary"
	@echo "  make hardened     - Build security-hardened binary"
	@echo "  make tiny         - Build size-optimized binary"
	@echo "  make install      - Install to system (requires root)"
	@echo "  make uninstall    - Remove from system"
	@echo "  make analyze      - Run static analysis"
	@echo "  make test         - Run test suite"
	@echo "  make clean        - Remove build artifacts"
	@echo ""
	@echo "Variables:"
	@echo "  PREFIX=/opt       - Change installation prefix"
	@echo "  CC=clang          - Use different compiler"
	@echo ""
	@echo "Examples:"
	@echo "  make release                    - Standard build"
	@echo "  make install PREFIX=/opt        - Install to /opt/bin"
	@echo "  make pgo                        - Profile-guided optimization"
	@echo "  CC=clang make hardened          - Hardened build with Clang"
