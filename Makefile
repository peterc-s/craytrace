# the C compiler to use
CC = gcc

# the base compiler flags
CFLAGS ?= -std=c23 -Wall -Wextra -pedantic -Werror -Werror=format-security -fstack-clash-protection -fstack-protector-all -fcf-protection -Wl,-z,relro,-z,now -lm

# special flags for debugging
CFLAGS_DEBUG ?= -g

# special flags for release
CFLAGS_RELEASE ?= -s -O2 -D_FORTIFY_SOURCE=3

# allow `make CFLAGS_EX="<flags>"` to add onto base compiler flags in CFLAGS
CFLAGS += $(CFLAGS_EX)

# the dource code directory
SRCDIR = src

# the build directory (will be made if it doesn't exist)
BUILDDIR = build
DEBUGDIR = $(BUILDDIR)/debug
RELEASEDIR = $(BUILDDIR)/release

# source files
SRC = $(wildcard $(SRCDIR)/*.c)

# object files
OBJ_DEBUG = $(SRC:$(SRCDIR)/%.c=$(DEBUGDIR)/%.o)
OBJ_RELEASE = $(SRC:$(SRCDIR)/%.c=$(RELEASEDIR)/%.o)

# the install directory
INSTALLDIR = /usr/sbin

# target name
NAME = c-raytrace
NAME_DEBUG = $(NAME)-debug

# debugger to use when doing `make debugger`
DEBUGGER ?= gdb

release: CFLAGS += $(CFLAGS_RELEASE)
release: $(NAME)

all: release debug

debug: CFLAGS += $(CFLAGS_DEBUG)
debug: $(NAME_DEBUG)

$(NAME): $(OBJ_RELEASE)
	@echo "Building release binary $@..."
	@$(CC) $(CFLAGS) $^ -o $@

$(NAME_DEBUG): $(OBJ_DEBUG)
	@echo "Building debug binary $@..."
	@$(CC) $(CFLAGS) $^ -o $@

$(DEBUGDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Building $@..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(RELEASEDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Building $@..."
	@$(CC) $(CFLAGS) -c $< -o $@

debug-run: debug
	./$(NAME_DEBUG) $(ARGS)

release-run: release
	./$(NAME) $(ARGS)

run: release-run

debugger: debug
	$(DEBUGGER) ./$(NAME_DEBUG)

image: image.ppm
	@timg image.ppm -U -C

image.ppm: $(NAME)
	@echo "Rendering image..."
	./$(NAME) > image.ppm

clean:
	@echo "Removing build directory..."
	@-rm -r $(BUILDDIR)
	@echo "Removing output binaries..."
	@-rm $(NAME) $(NAME_DEBUG)
	@echo "Removing output image..."
	@-rm image.ppm

install: release
	sudo cp $(NAME) $(INSTALLDIR)/$(NAME)

uninstall:
	sudo rm $(INSTALLDIR)/$(NAME)

.PHONY: all debug release clean debug-run release-run run debugger install uninstall image
