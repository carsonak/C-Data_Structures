#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct AST AST;

/**
 * struct AST - Abstract Syntax Tree.
 * @id: type of node.
 * @op: operations.
 */
struct AST
{
	/**
	 * enum expression_type - tags for available operations.
	 * @AST_NUMBER: a number.
	 * @AST_ADD: an add operation.
	 * @AST_MUL: a multiply operation.
	 */
	enum expression_type
	{
		AST_NUMBER,
		AST_ADD,
		AST_MUL,
	} id;

	/**
	 * union operations - union of operators and operands.
	 * @AST_NUMBER: a number.
	 * @AST_ADD: an add operation.
	 * @AST_MUL: a multiply operation.
	 */
	union operations
	{
		/**
		 * struct AST_NUMBER - a number.
		 * @number: the number.
		 */
		struct AST_NUMBER
		{
			int number;
		} AST_NUMBER;

		/**
		 * struct AST_MUL - a multiply operation.
		 * @left: 1st expression.
		 * @right: 2nd expression.
		 */
		struct AST_MUL
		{
			AST *left;
			AST *right;
		} AST_MUL;

		/**
		 * struct AST_ADD - an add operation.
		 * @left: 1st expression.
		 * @right: 2nd expression.
		 */
		struct AST_ADD
		{
			AST *left;
			AST *right;
		} AST_ADD;
	} op;
};

void ast_print(AST *ptr);
void ast_free(AST *ptr);
AST *ast_new(AST ast);

/* Syntactic sugar for allocating a AST. */
#define AST_NEW(tag, ...) ast_new((AST){tag, {.tag = (struct tag){__VA_ARGS__}}})

#endif /* AST_H */
