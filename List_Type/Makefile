include ../Makefile

CFLAGS += -I ../tau

$(BINDIR)/test_%: test_%.c %.c doubly_linked_list.c singly_linked_list.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@
