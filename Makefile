.PHONY: all clean install test

MAC_DIR=macosx
POSIX_DIR=posix

OS := $(shell uname -s)

LIBMETRO_AR=libmetro.a

HEADERS := metro.h
SRC_HEADERS := $(addprefix metro/,$(HEADERS))

# Get the list of object files to compile,
# some are platform-agnostic and some are not.

OBJS_GENERIC := event.o mem.o
OBJS_PLATFORM := metro.o thread.o

ifeq ($(OS),Darwin)
PLATFORM_DIR := $(MAC_DIR)
else
PLATFORM_DIR := $(POSIX_DIR)
endif
OBJS_PLATFORM := $(addprefix $(PLATFORM_DIR)/,$(OBJS_PLATFORM))

OBJS := $(OBJS_GENERIC) $(OBJS_PLATFORM)

TEST_PROGS := $(wildcard test/*.c)
TEST_PROGS := $(subst .c,,$(TEST_PROGS))

EXAMPLE_PROGS := $(wildcard examples/*.c)
EXAMPLE_PROGS := $(subst .c,,$(EXAMPLE_PROGS))

ifeq ($(DESTDIR),)
DESTDIR=/usr/local/
endif

LIB_DIR=$(DESTDIR)lib
INCLUDE_DIR=$(DESTDIR)include
SHARED_DIR=.shared

# gcc flags

CPPFLAGS=-I.
CFLAGS=-Wall -g -O2 -pthread
LDFLAGS=-L.
LDLIBS=-lcheck -lmetro

all: $(LIBMETRO_AR)

install: $(LIBMETRO_AR)
	for h in $(SRC_HEADERS); do rm -f $(INCLUDE_DIR)/$$h; done
	for h in $(SRC_HEADERS); do cp $$h $(INCLUDE_DIR); done
	rm -f $(LIB_DIR)/$(LIBMETRO_AR)
	cp $(LIBMETRO_AR) $(LIB_DIR)

uninstall:
	for h in $(SRC_HEADERS); do rm -f $(INCLUDE_DIR)/$$h; done
	rm -f $(LIB_DIR)/$(LIBMETRO_AR)

$(LIBMETRO_AR): $(OBJS)
	ar rcs $(LIBMETRO_AR) $^

clean:
	rm -rf $(LIBMETRO_AR) $(OBJS) *~ $(MAC_DIR)/*~ $(POSIX_DIR)/*~ \
        $(TEST_PROGS) $(EXAMPLE_PROGS) examples/*~ test/*~
	find . -name '*.dSYM' -type d -exec rm -rf '{}' \;

test: $(LIBMETRO_AR) $(TEST_PROGS)
	@for test in $(TEST_PROGS); do $$test; done

examples: $(LIBMETRO_AR) $(EXAMPLE_PROGS)
