BINDIR := bin
CC := clang

CPPFLAGS :=
C_STANDARD := --std=c17
WFLAGS := -Wall -Wextra -Wformat -Wformat=2 -pedantic -Werror
OPTIMISATION := -Og
DEBUG := -g3 -fno-omit-frame-pointer
SANITIZER := -fsanitize=address -fsanitize=undefined -fno-common
CFLAGS = $(C_STANDARD) $(CPPFLAGS) $(WFLAGS) $(OPTIMISATION) $(DEBUG) $(SANITIZER)


$(BINDIR)/%: %.c | $(BINDIR)
	$(CC) $(CFLAGS) $< -o $@

$(BINDIR):
	mkdir -p $@

.PHONY: clean
clean:
	@$(RM) -vrd $(BINDIR)
