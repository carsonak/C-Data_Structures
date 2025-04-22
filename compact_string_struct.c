#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#define report_size(x) printf(#x ": %zu\n", x)
#define report_ptr(x) printf(#x ": %p\n", (void *)x)

struct String
{
	size_t len;
	char *str;
};

struct String *string_new(const char *str)
{
	size_t len = 1;

	if (str)
	{
		len = strlen(str);
		if (!len)
			len = 1;
	}

	struct String *ns = malloc(sizeof(*ns) + (sizeof(*ns->str) * (len + 1)));

	if (!ns)
		return (NULL);

	*ns = (struct String){.len = len, .str = (char *)ns + sizeof(*ns)};
	if (str)
		strcpy(ns->str, str);

	ns->str[len - 1] = '\0';
	return (ns);
}

void *string_delete(struct String *ns)
{
	if (!ns)
		return (NULL);

	ns->len = 0;
	free(ns);
	return (NULL);
}

void print_str(const char *str)
{
	printf("[%p] -> %c\n", &str[0], *str);
	printf("[%p] -> %s\n", (void *)str, str);
}

int main(void)
{
	const char str[] = "Hello World!";
	struct String *ns = string_new(str);

	if (!ns)
		return (1);

	report_size(sizeof(ns));
	report_size(sizeof(*ns));
	report_size(sizeof(ns->len));
	report_size(offsetof(struct String, len));
	report_size(sizeof(ns->str));
	report_size(offsetof(struct String, str));
	report_ptr(ns);
	report_ptr(&ns->len);
	report_ptr(&ns->str);
	report_ptr(ns->str);
	report_ptr(&(*ns->str));
	print_str(ns->str);

	ns = string_delete(ns);

	return (0);
}
