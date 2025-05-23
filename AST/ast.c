#include "ast.h"

/**
 * ast_new - allocates and initialises an AST.
 * @ast: the AST to initialise with.
 *
 * Return: pointer to the AST, NULL on failure.
 */
AST *ast_new(AST ast)
{
	AST *ptr = malloc(sizeof(*ptr));

	if (ptr)
		*ptr = ast;

	return (ptr);
}

/**
 * ast_free - deallocates memory of an ast.
 * @ptr: pointer to the ast.
 */
void ast_free(AST *ptr)
{
	AST ast = {0};

	if (!ptr)
		return;

	ast = *ptr;
	switch (ast.id)
	{
	case AST_NUMBER:
	{
		struct AST_NUMBER data = ast.op.AST_NUMBER;

		(void)data;
		break;
	}
	case AST_ADD:
	{
		struct AST_ADD data = ast.op.AST_ADD;

		ast_free(data.left);
		ast_free(data.right);
		break;
	}
	case AST_MUL:
	{
		struct AST_MUL data = ast.op.AST_MUL;

		ast_free(data.left);
		ast_free(data.right);
		break;
	}
	}

	free(ptr);
}

/**
 * ast_print - prints an AST as an expression.
 * @ptr: pointer to the AST to print.
 */
void ast_print(AST *ptr)
{
	AST ast = {0};

	if (!ptr)
		return;

	ast = *ptr;
	switch (ast.id)
	{
	case AST_NUMBER:
	{
		struct AST_NUMBER data = ast.op.AST_NUMBER;

		printf("%d", data.number);
		return;
	}
	case AST_ADD:
	{
		struct AST_ADD data = ast.op.AST_ADD;

		printf("(");
		ast_print(data.left);
		printf(" + ");
		ast_print(data.right);
		printf(")");
		return;
	}
	case AST_MUL:
	{
		struct AST_MUL data = ast.op.AST_MUL;

		printf("(");
		ast_print(data.left);
		printf(" * ");
		ast_print(data.right);
		printf(")");
		return;
	}
	}
}

/**
 * main - Entry point.
 *
 * Return: 0.
 */
int main(void)
{
	/* 4 + 2 * 10 + 3 * (5 + 1) */
	AST *term = AST_NEW(
		AST_ADD,
		AST_NEW(AST_NUMBER, 4),
		AST_NEW(
			AST_ADD,
			AST_NEW(AST_MUL, AST_NEW(AST_NUMBER, 2), AST_NEW(AST_NUMBER, 10)),
			AST_NEW(
				AST_MUL,
				AST_NEW(AST_NUMBER, 3),
				AST_NEW(AST_ADD, AST_NEW(AST_NUMBER, 5), AST_NEW(AST_NUMBER, 1)))));

	ast_print(term);
	printf("\n");
	ast_free(term);

	term = ast_new((AST){0});

	ast_print(term);
	printf("\n");
	ast_free(term);
	return (0);
}
