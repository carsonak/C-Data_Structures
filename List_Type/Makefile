include ../Makefile

MATRIX := ../Matrix
CFLAGS += -I ../tau -I $(MATRIX)

$(BINDIR)/test_%: test_%.c %.c linked_node.c $(MATRIX)/2d_array.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BINDIR)/test_%: test_%.c %.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@
