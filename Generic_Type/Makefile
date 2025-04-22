include ../../Makefile

$(BINDIR)/test_%: test_%.c %.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@
