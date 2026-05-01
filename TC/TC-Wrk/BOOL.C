/****************************************************************************
*
* bool V1.3: Read boolean expression(s) and perform some useful operations.
*            See text of help message below for summary details.
*
* (c) Adrian Johnstone 1993. This code may be freely distributed.
*
****************************************************************************/
#include<stdlib.h>
#include<stdio.h>
#include<setjmp.h>
#include<string.h>
#include<stdarg.h>
#include<time.h>

#if defined(TURBOC_V2)
extern unsigned _stklen = 15000;	/* For Turbo C V2.00: increase stack size */
#define CLOCKS_PER_SEC CLK_TCK
#endif

void help(void)
{
 printf
 ("Bool V1.3: Read boolean expression(s) and print canonical sum of products.\n"
  "           Optionally print truth table, print Karnaugh map, minimise\n"
  "           factorise and print statistics (in that order).\n\n"
  "Source syntax:  e0 = e1 {';' e1}.               expression list\n"
  "                e1 = e2 {'^' e2 }.              exclusive OR\n"
  "                e2 = e3 {'+' e3 }.              inclusive OR\n"
  "                e3 = e4 {'.' e4 }.              AND\n"
  "                e4 = '~' e4 | e5.               NOT\n"
  "                e5 = a..z | 1 | 0 | '(' e1 ')'. basic elements\n\n"
  "                Multiple expressions may be entered on one line.\n"
  "                Dot (AND) operators may be omitted.\n"
  "                EOF (ctrl-z, return in MS-DOS) or ctrl-C terminates input.\n"
  "                Syntax errors are reported and the bad expression discarded.\n\n"
  "Valid examples: a.b;  ab; a+b; ; 1; 1^a; ~(a+b)c;\n\n"
  "Tips: Use pipes and redirection, as in 'echo a.b; | bool' or 'bool < filename'.\n"
  "      '(expression1)^(expression2);' returns '0;' if expression1=expression2.\n"
  "      Large functions take an unreasonably long time to evaluate!\n\n"
  "Flags: -t Print truth table\n"
  "       -k Draw Karnaugh map of first four variables.\n"
  "       -m Merge canonical sum of products to prime implicants.\n"
  "       -c Generate cover function and select a minimal cover.\n"
  "       -f Factorise (imperfectly).\n"
  "       -s Report summary statistics.\n"
  "       -v Be verbose and show intermediate working.\n"
 );
}

#define BUFLEN 30000			/* maximum length of an expression in characters */
#define VARIABLES 26			/* maximum number of supported variables (a through z) */

/****************************************************************************
*
* Global data
*
****************************************************************************/
jmp_buf err;					/* GOTO target for error breakout in expression parser */

unsigned long value;			/* present bit patterns */

char buffer[BUFLEN],			/* expression buffer */
*cc=buffer,						/* current character pointer */
 old_conv[VARIABLES],
 conv[VARIABLES] = "                          ";	/* old ids for printing */

struct flagtype
{
 int truth_table: 1,			/* print truth table */
 karnaugh: 1,					/* print Karnaugh map */
 minimise: 1,					/* perform minimisation */
 cover: 1,						/* generate cover function and select minimal cover */
 factor: 1,						/* factorise */
 statistics: 1,					/* print statistics */
 verbose: 1;					/* be verbose */
} flags;

struct prodtype					/* description of a product term */
{
	unsigned long value,		/* true or false bits */
	 inuse;						/* 1=>value used */
	unsigned weight;			/* number of bits set in weight */
	struct prodtype *next,		/* next product term */
	*rest;						/* next bracketed sum of products */
	char id;					/* equivalent identifier */
	char merged;				/* has participated in merge flag */
} *primes[VARIABLES];			/* pointers to up to 26 prime implicants */
/****************************************************************************
*
* Printing routines
*
****************************************************************************/
/* flags.verbose conditional printing routine. verbose_printf() acts as
   wrapper for the standard printf that discards output if flags.verbose is false */
int verbose_printf(char *fmt, ...)
{
	int i;
	va_list ap;					/* argument list walker */

	va_start(ap, fmt);

	if (flags.verbose)			/* no-op if not verbose... */
		i = vprintf(fmt, ap);	/* ... otherwise pass to printf() */

	va_end(ap);

	return (i);					/* for completeness, although not used here */
}

/* print out a singleproduct term */
void print_product(struct prodtype * p)
{
	unsigned long v = p->value,	/* get a copy of the value... */
	 i = p->inuse;				/* ...and the inuse fields. */
	 char c = 'a';				/* initialise variable name */

	if (i == 0)                 /* No bits in use: must be a constant */
	{
		if (v < 2)				/* print constant value of 1 or 0 */
			verbose_printf("%1u", v);
	/* Note that constant values >=2 are reserved for a future implementation
	   of more general parse trees, higher values dnoting operators. */
	}
	else
		while (i > 0)			/* while there are still some bits in use */
		{
			if (i & 1)			/* if lsb inuse */
			{
				if (!(v & 1))	/* is value tur or false? */
					verbose_printf("~");	/* add a bar if variable is negated */
				verbose_printf("%c", conv[c - 'a']);	/* print variable */
				if (i > 1)		/* if there are any bits left... */
					verbose_printf(".");	/* ...print an AND */
			}
			v = v >> 1;			/* shift down local value and inuse words */
			i = i >> 1;
			c++;				/* select next variable */
		}
}
/* walk up an expression printing products and parentheses */
void print_expression(struct prodtype * e)
{
	if (e == NULL)				/* Nothing there */
		verbose_printf("Empty");
	else
		while (e != NULL)		/* while not at end of expression */
		{
			print_product(e);	/* dump this product term */

			if (e->rest != NULL)			/* Is there a bracketed sub-expression? */
			{
				verbose_printf(".(");		/* print opening bracket... */
				print_expression(e->rest);	/* ...recurse */
				verbose_printf(")");	/* print closing parenthsis */
			}
			e = e->next;					/* move to next product */

			if (e != NULL)					/* anything left? */
				verbose_printf(" + ");		/* print OR */
		}
}

/* force an expression to be printed even if non-verbose */
void force_print_expression(struct prodtype * e)
{
	int v=flags.verbose;		/* remember verbose flag */

	flags.verbose=1;			/* force verbosity */
	print_expression(e);		/* call print */
	flags.verbose=v;			/* restore verbosity */
}
/****************************************************************************
*
* Input reading and buffer stuffing routines
*
****************************************************************************/
/* read command line switches and set flags accordingly */
void parse_switches(char **argv)
{
	while (*++argv != NULL)
		switch (*((*argv) + 1))
		{
			case 't':
				flags.truth_table = 1;
				break;
			case 'k':
				flags.karnaugh = 1;
				break;
			case 'm':
				flags.minimise = 1;
				break;
			case 'c':
				flags.cover = 1;
				break;
			case 'f':
				flags.factor = 1;
				break;
			case 's':
				flags.statistics = 1;
				break;
			case 'v':
				flags.verbose = 1;
				break;

			default:
				help();
				exit(1);
        };
}

/* put a character into the buffer, discarding spaces, and stopping if buffer full */
void buf_put(char c)
{
	if (c > ' ' && c <= '~' && cc - buffer < BUFLEN)
		*cc++ = c;
}

/* figure out if a and b are variables or parenthesised expressions. Used to
   decide if an AND operator should be inserted when reading an expression */
int implicit_dot(char a, char b)
{
	return ((a >= 'a' && a <= 'z') || a == '0' || a == '1' || a == ')') &&
	((b >= 'a' && b <= 'z') || b == '0' || b == '1' || b == '(' || b == '~');
}

/* get a new expression from stdin */
void read_expression(void)
{
	char c,
	 last = 0;

	cc = buffer;				/* start filling a new buffer */

	while (!feof(stdin) && (c = getchar()) != ';')	/* read an expression */
	{
		if (implicit_dot(last, c))	/* should we insert an AND operator? */
			buf_put('.');
		buf_put(c);      			/* put the character */
		if (c > ' ' && c <= '~')	/* update last character for implicit_dot() */
			last = c;
	}

	*cc = 0;					/* add a terminating null */

	if (cc - buffer >= BUFLEN)	/* did we overflow the buffer? */
	{
		verbose_printf("Expression too long\n");
		*buffer = 0;			/* kill bad expression: empty expressions are ignored */
	}
}

/* assign variables to bit positions 0, 1... in the order in which they are
   encountered in the input expression, and memorise their names in conv[] */
unsigned map_expression(char *buffer)
{
	char newid[VARIABLES] = "                          ";	/* id mapping table */
	unsigned high;				/* how many variables are there in this expresion */

	memcpy(old_conv, conv, sizeof(old_conv));	/* remember old */
	memcpy(conv, newid, sizeof(newid));			/* conversion table */

	for (high = 'a'; *buffer != 0; buffer++)	/* scan expression looking for variables */
		if (*buffer >= 'a' && *buffer <= 'z')	/* if we are looking at a variable... */
		{
			if (newid[*buffer - 'a'] == ' ')	/* if this one is unmapped... */
			{
				newid[*buffer - 'a'] = high;	/* create map entry */
				conv[high++ - 'a'] = *buffer;	/* remember old identifier */
			}
			*buffer = newid[*buffer - 'a'];		/* remap identifier */
		}

	return (high - 'a');		/* return number of active variables */
}
/****************************************************************************
*
* Expression calculator
*
****************************************************************************/
/* print out current expression (unless verbose in which case it will already
   have been printed) and then point to current symbol. I always liked the
   rather genteel way the BBC micro BASIC said `mistake' instead of `error'
   so I've used it here */
void error(void)
{
	unsigned long i = cc - buffer;

	if (!flags.verbose)			/* replicate verbose message */
		printf("\nExpanding %s\n", buffer);
	printf("Mistake --");		/* print error message (how genteel) */
	for (i = cc - buffer; i > 0; i--)	/* print a run of - characters */
		printf("-");
	printf("^\n");				/* line advance */
	longjmp(err, 1);			/* GOTO start of parser */
}

char e1(void);					/* forward reference */

char e5(void)					/* handle basic elements */
{
	char c = *cc,				/* remember character from start of parse */
	 r = *cc == '0' ? 0 :		/* constant zero? */
	*cc == '1' ? 1 :			/* constant one? */
	*cc == '(' ? (cc++, e1()) :	/* sub-expression? */
	(*cc >= 'a' && *cc <= 'z') ?		/* variable? */
		1 & (value >> ((*cc) - 'a')) :	/* look up current value */
	(error(),0);				/* bad syntax */

	if (c == '(' && *cc != ')')	/* check correct parenthesis matching */
		error();

	cc++;						/* next character */
	return (r);
}

char e4(void)					/* handle monadic NOT */
{
	if (*cc == '~')				/* NOT operator? */
	{
		cc++;					/* next character */
		return (!e4());			/* call ourselves to handle multiple NOTs */
	}
	else
		return (e5());			/* next priority */
}

char e3(void)					/* handle AND */
{
	char r = e4();				/* parse left hand side */

	while (*cc == '.')			/* AND operator */
	{
		cc++;					/* next character */
		r &= e4();				/* AND in right hand side */
	} return (r);
}

char e2(void)					/* handle EXOR */
{
	char r = e3();				/* parse left hand side */

	while (*cc == '^')			/* EXOR operator */
	{
		cc++;					/* next character */
		r ^= e3();				/* EXOR in right hand side */
	} return (r);
}

char e1(void)					/* handle inclusive OR */
{
	char r = e2();				/* parse left hand side */

	while (*cc == '+')			/* OR operator */
	{
		cc++;					/* next character */
		r |= e2();				/* OR in right hand side */
	} return (r);
}
/****************************************************************************
*
* Routines to build a canonical sum of products structure from text
*
****************************************************************************/
/* return 1 if f is covered by s */
int product_first_in_second(struct prodtype * f, struct prodtype * s)
{
	return (((s->inuse | f->inuse) == s->inuse) &&	/* no 1's in f that are 0 in s */
		(f->value & f->inuse) == (s->value & f->inuse)); /* and values are the same */
}

/* return 1 if a is identical to b */
int product_first_is_second(struct prodtype * a, struct prodtype * b)
{
	return ((a->value & a->inuse) == (b->value & b->inuse) && a->inuse == b->inuse);
}

/* insert a new product term at the head of base, ignoring duplicates */
int insert_product(struct prodtype ** base, unsigned long inuse, unsigned long value)
{
	struct prodtype *q=*base,	/* local temporary for scanning expression */
	*p = malloc(sizeof(*p));	/* make a new node */

	if (p == NULL)				/* Very fatal error */
	{
		printf("Ran out of heap\a\n");
		exit(1);
	}

	memset(p,0,sizeof(struct prodtype));	/* empty the new node */

	p->inuse = inuse;			/* stuff product term */
	p->value = value;

	while (q != NULL && !product_first_is_second(p, q))	/* scan whole list */
		q = q->next;

	if (q == NULL)				/* if not found insert at head of list */
	{
		p->next = *base;
		*base = p;
		return 1;				/* success */
	}
	else
		return 0;				/* failure */
}

/* Grind through all 2**n possible minterms and build a CSOP expression.
   If truth_table is 1, then print out a truth table as we go */
struct prodtype *expand_expression(char *buffer, unsigned expr_variables,
								   int truth_table, unsigned long *minterms)
{
	struct prodtype *expression = NULL;	/* new expression */
	unsigned hi;

	*minterms=0;

	if (expr_variables == 0)				/* must be a constant expression */
	{
		insert_product(&expression, 0, (cc = buffer, e1()));	/* insert constant value */
		return (expression);	/* and we're done */
	}

	if (truth_table)			/* print a truth table as we go if requested */
	{
		for (hi = 0; hi < expr_variables; hi++)	/* print out the variables in use */
			printf("%c ", conv[hi]);

		printf("\n");
	}

	/* now go through all 2**expr_variables-1 combinations */
	for (value = 0; value <= (1 << expr_variables) - 1; value++)
	{
		if (truth_table)		/* do the truth table as we go */
		{
			unsigned long val = value,	/* local copy of value word */
			 hi = expr_variables;

			while (hi-- > 0)	/* while there are still some variables to go ... */
			{
				printf("%1u ", val & 1);
				val = val >> 1;	/* shift down val */
			}
		}

		if (cc = buffer, e1())	/* evaluate expression */
		{
			insert_product(&expression, (1 << expr_variables) - 1, value);

			(*minterms)++;			/* increment minterm count */
			if (truth_table)	/* do truth table as we go */
				printf("  1\n");
		}
		else
		{
			if (truth_table)
				printf("  0\n");
		}
	}

	if (expression == NULL)		/* this function evaluates to FALSE */
		insert_product(&expression, 0, 0);	/* insert a constat zero */

	return (expression);
}
/****************************************************************************
*
* Karnaugh map printing routines
*
****************************************************************************/
void print_karnaugh(void)
{
	unsigned x,					/* x-coordinate */
	 y;							/* y-coordinate */

	printf("    %c %c\n", conv[2], conv[2]);	/* top variable */

	for (y = 0; y < 4; y++)		/* vertical scan */
	{
		if (y > 0 && y < 3)		/* left variable */
			printf("%c", conv[0]);
		else
			printf(" ");

		for (x = 0; x < 4; x++)	/* horizontal scan */
		{
			value = (y > 1 ? y ^ 1 : y) + (x > 1 ? x ^ 1 : x) * 4;
			printf(" %u", (cc = buffer, e1()));	/* evaluate */
		}

		if (y > 1)				/* right variable */
			printf(" %c", conv[1]);
		else
			printf(" ");

		printf("\n");
	}
	printf("      %c %c\n", conv[3], conv[3]);	/* bottom variable */
}
/****************************************************************************
*
* Quine-McCluskey minimisation routines
*
****************************************************************************/
/* count the number of true bits in a product term */
int true_bits(struct prodtype *p)
{
	unsigned long v=p->value,	/* local copy of values */
	 u=p->inuse;				/* local copies of inuse */
	int count=0;				/* bit counter */

	while (u!=0)				/* while there are still bits in use... */
	{
		if (v&1 && u&1) 		/* if true bit in use */
			count++;			/* bump count */
		u=u>>1;					/* shift down inuse bits */
		v=v>>1;					/* shift down value bits */
	}

	return(count);
}

/* merge cannonical sum of products form to a list of prime implicants */
struct prodtype *prime_expression(struct prodtype * expression, unsigned expr_variables,
								  unsigned long *merges)
{
	struct prodtype *prime = NULL,	/* new expression for prime implicants */
	*merge_list[2][VARIABLES];	/* arrays of lists sorted by number of active variables */

	int temp,
	 cl = 0;					/* current list index */
	char not_done;				/* all done flag */

	*merges = 0;
	memset(merge_list, 0, sizeof(merge_list));	/* set all sub-lists to NULL */

	/* first split expression into list of sorted sub-expressions */
	while (expression != NULL)	/* scan entire list adding product terms to merge list */
	{
		insert_product(&merge_list[cl][true_bits(expression)],
					   expression->inuse, expression->value);
		expression = expression->next;
	}

	do							/* iterate over list until no merges occur */
	{
		struct prodtype *lo;

		not_done = 0;

		/* now print out sub-expression list */
		verbose_printf("\nMerging:\n");
		for (temp = 0; temp <= expr_variables; temp++)
		{
			verbose_printf("[%2i] ", temp);
			print_expression(merge_list[cl][temp]);
			verbose_printf("\n");
		}

		/* and prime list */
		verbose_printf("Primes: ");
		print_expression(prime);
		verbose_printf("\n");

		/* Scan entire list checking row n against row n+1 */
		for (temp = 0; temp < expr_variables; temp++)
		{
			lo = merge_list[cl][temp];	/* point at start of list */

			merge_list[1 - cl][temp] = NULL;	/* clear new merge list */
			while (lo != NULL)	/* scan entire low list */
			{
				struct prodtype *hi = merge_list[cl][temp + 1];	/* point at next list */

				while (hi != NULL)	/* scan entire high list until a merge occurs */
				{
					unsigned long m = lo->inuse & (lo->value ^ hi->value);	/* difference bits */

					if (lo->inuse == hi->inuse && m != 0)	/* if candidate for merge */
					{
						unsigned long new_inuse = lo->inuse & ~m;	/* mask difference */

						/* now find out if there is only one difference bit */
						while ((m & 1) == 0)	/* until a difference bit appears */
							m = m >> 1;	/* shift zeroes out */
						if ((m >> 1) == 0)	/* then there was only one set */
						{
							not_done = 1;	/* still some product terms to process */
							print_product(lo);	/* give some diagnostics */
							verbose_printf(" merged with ");
							print_product(hi);
							verbose_printf(" to form ");
							/* insert new term. If merging to constant, insert 1 */
							insert_product(&merge_list[1 - cl][temp], new_inuse,
										   new_inuse == 0 ? 1 : lo->value);
							print_product(merge_list[1 - cl][temp]);
							verbose_printf("\n");
							(*merges)++;	/* bump merge counter */
							lo->merged = 1;	/* mark these terms as merged */
							hi->merged = 1;
						}

					}
					hi = hi->next;
				}

				if (!lo->merged)/* no merges occurred, so must be prime */
				{
					insert_product(&prime, lo->inuse, lo->value);
					print_product(prime);
					verbose_printf(" is prime\n");
				}

				lo = lo->next;
			}
		}
		/* move the contents of the top list to prime */

		lo = merge_list[cl][expr_variables];

		while (lo != NULL)
		{
			if (!lo->merged)	/* no merges occurred, so must be prime */
			{
				insert_product(&prime, lo->inuse, lo->value);
				print_product(prime);
				verbose_printf(" is prime\n");
			}

			lo = lo->next;
		}

		/* and set top list of new list to NULL */
		merge_list[1 - cl][expr_variables] = NULL;

		cl = 1 - cl;			/* switch lists */
	}
	while (not_done);

	return (prime);
}
/****************************************************************************
*
* Routines to generate cover function
*
****************************************************************************/
/* Give each prime implicant a unique variable name. Return false if >26 primes*/
int label_primes(struct prodtype * p, unsigned long *prime_implicants)
{
	unsigned count;
	unsigned long temp;

	*prime_implicants=0;

	for (count = 0; p != NULL; count++, p = p->next) /* scan expression */
	{
		(*prime_implicants)++;	/* count terms */
		p->id = 'z' - count;	/* give each prime a letter */
		if (count > VARIABLES)	/* give up at 26 */
			return(0);

		primes[count] = p;	/* remember prime implicant */
		temp = p->inuse;		/* count number of inuse bits */
		p->weight = 0;
		while (temp != 0)
		{
			if (temp & 1)
				p->weight++;	/* bump weight for each bit set */
			temp = temp >> 1;	/* shift down */
		}
	}
	return(1);
}

/* Check implicant coverage and construct prime implicant function */
void build_prime_implicant_function(struct prodtype * expression, struct prodtype * prime)
{
	char start = 1;				/* flag to mark first term */

	cc = buffer;                /* point at start of text buffer */

	verbose_printf("\nMinterm coverage\n");	/* be chatty */

	while (expression != NULL)	/* scan entire expression */
	{
		struct prodtype *p = prime;	/* local copy of prime expression */
		char first = 1;			/* flag to mark first term */

		verbose_printf("Minterm ");	/* print coverage */
		print_product(expression);
		verbose_printf(" is covered by ");

		if (start)				/* place AND operators except before first variable */
			start = 0;
		else
			buf_put('.');

		buf_put('(');			/* opening parenthesis */

		while (p != NULL)		/* scan prime expression */
		{
			if ((p->inuse & p->value) == (p->inuse & expression->value))
			{
				if (first)		/* place OR operators except before first variable */
					first = 0;
				else
				{
					verbose_printf(", ");
					buf_put('+');
				}
				print_product(p);	/* dump out the product term */

				buf_put(p->id);		/* write product term to text buffer */
			}
			p = p->next;
		}

		buf_put(')');

		verbose_printf("\n");
		expression = expression->next;	/* move to next expression minterm */
	}

	*cc = 0;					/* add trailing null to cover function in buffer */
}

/* print list of implicants and weights */
void print_implicants(struct prodtype * p)
{
	verbose_printf("\nImplicant list\n");
	while (p != NULL)			/* scan entire prime implicant list */
	{
		verbose_printf("%c (", p->id);
		print_product(p);
		verbose_printf(") weight %u\n", p->weight);

		p = p->next;
	}
}
/****************************************************************************
*
* Routines to find minimum cover
*
****************************************************************************/
/* caluculate the number of terms in each cover */
void label_prime_implicant_function(struct prodtype * p)
{
	while (p != NULL)
	{                           /* scan entire function */
		unsigned long temp = p->inuse;	/* local copy of inuse bits */
		unsigned count = 0;				/* count of active variables */

		p->weight = 0;			/* clear product weight */

		while (temp != 0)		/* while there are still some variables in use */
		{
			if (temp & 1)		/* if the least significant bit is true... */
				p->weight += primes[count]->weight;	/* add in current weight */
			count++;			/* bump variable count */
			temp = temp >> 1;	/* shift down */
		}

		p = p->next;			/* move to next product term */
	}
}

/* print out all covers and their weights */
struct prodtype *print_covers(struct prodtype * p, unsigned long *covers)
{
	struct prodtype *m = NULL;	/* pointer to minimum cover */
	unsigned minweight = 0xffff;	/* minimum weight set to a large number */

	*covers=0;

	while (p != NULL)	/* scan all products in expanded cover function */
	{
		verbose_printf("Cover ");	/* be chatty */
		print_product(p);
		verbose_printf(" has weight %u\n", p->weight);

		if (p->weight < minweight)	/* look for lowest weight */
		{
			minweight = p->weight;
			m = p;
		}
		p = p->next;			/* move to next product term */

		(*covers)++;			/* bump cover count */
	}
	return (m);					/* return minimum cover */
}

/* print out a cover product term in terms of the original expression */
struct prodtype *cover_as_expression(struct prodtype * c)
{
	struct prodtype *e = NULL;		/* temporary to build cover expression */
	unsigned long value = c->value;	/* local copy of cover value */
	unsigned count = 0;				/* index to primes array */

	while (value != 0)			/* while there are still some variables */
	{
		if (value & 1)			/* if least significant bit is active */
			insert_product(&e, primes[count]->inuse, primes[count]->value);

		count++;           		/* bump index */
		value = value >> 1;		/* shift value down one bit */
	}

	return e;					/* return minimum cover expression */
}
/****************************************************************************
*
* Routines to find an approximate factorisation from a msop
*
****************************************************************************/
/* A rather quick and dirty factorisation routine: look for the most common
   variable in an expression and then move it to the front of the expression.
   Move terms that don't include that variable into a new expression list and
   attach it to the rest field. Then recursively factorise that expression
   and refactorise this expression */
void factorise(struct prodtype ** expr)
{
	static unsigned frequencies[2 * VARIABLES],	/* frequency histogram array */
	 max,						/* maximum frequency seen so far */
	 maxindex,					/* index of maximum frequency seen so far */
	 count;						/* current variable index */
	struct prodtype *e = *expr, /* temporary product term pointer */
	*new_out = NULL,
	*new_in = NULL;

	if (e == NULL)              /* skip empty function */
		return;

	verbose_printf("\nFactorising ");	/* be chatty */
	print_expression(e);
	verbose_printf("\n");
	max = 0;					/* set max to a small value */
	memset(frequencies,0,sizeof(frequencies));	/* clear frequencies */

	while (e != NULL)			/* collect frequencies */
	{
		unsigned long inuse = e->inuse,	/* local copy of inuse bits */
		 value = e->value;				/* local copy of value word */

		count = 0;
		while (inuse != 0)		/* while there are still variables in use */
		{
			if (inuse & 1)		/* if least significant bit is set */
				if (value & 1)	/* true variable? */
					frequencies[count]++;
				else			/* complemented variable */
					frequencies[count + VARIABLES]++;
			inuse = inuse >> 1;	/* shift down inuse word */
			value = value >> 1;	/* shift down value word */
			count++;			/* bump current variable */
		}
		e = e->next;			/* select next product term */
	}
	/* scan histogram to find most frequent entry */
	for (count = 0; count < 2 * VARIABLES; count++)
	{
		if (frequencies[count] > 0)	/* print some diagnostics */
			verbose_printf("Variable %c%c has frequency %u\n",
						   count < VARIABLES ? ' ' : '~', 'a' + (count % VARIABLES),
						   frequencies[count]
						  );
		if (frequencies[count] > max)	/* is this a new maximum? */
		{
			max = frequencies[count];	/* remember new maximum... */
			maxindex = count;			/* ...and new maximum variable */
		}
	}

	/* now move most common variable outside expression */
	/* Make a product containing only the most common variable to the head of the new expression. */
	if (max > 1)				/* don't factorise single terms! */
	{
		insert_product(&new_out, 1 << (maxindex % VARIABLES),
					   maxindex < VARIABLES ? 0xFFFF : 0);

		verbose_printf("Moving ");	/* some chat */
		print_product(new_out);
		verbose_printf(" outside expression\n");

		/* Move along old next chain. If a product contains the common variable
		   then delete the the variable and the add the result to the new->rest
		   chain, otherwise add the product to the new->next chain */
		e = *expr;
		while (e != NULL)			/* scan entire expression */
		{
			struct prodtype *p = e;

			e = e->next;
			/* does this product contain the maximum variable? */
			if (product_first_in_second(new_out, p))
			{
				p->inuse = p->inuse & ~new_out->inuse;	/* kill variable */

				if (p->inuse != 0)	/* anything left in this product term? */
				{
					p->next = new_in;	/* add to new within expression */
					new_in = p;
				}
			}
			else					/* maximum variable not in this product */
			{
				p->next = new_out->next;	/* add to new rest-of expression */
				new_out->next = p;
			}
		}
		new_out->rest = new_in;		/* point rest-of at within */
		factorise(&(new_out->next));	/* recursively factorise */
		factorise(&(new_out->rest));	/* recursively factorise */

		*expr = new_out;
	}
}
/****************************************************************************
*
* Mainline
*
****************************************************************************/
void main(int argc, char *argv[])
{
	struct prodtype *expression,	/* input expression */
	*prime,							/* prime implicant function */
	*cover,							/* cover function */
	*mincover;						/* minimum cover as expresion */

	unsigned long expr_minterms,	/* number of minterms in original expression */
	 cover_minterms,				/* number of minterms in cover expression */
	 expr_merges,   				/* number of merges of original expression */
	 cover_merges,  				/* number of merges of cover expression */
	 expr_primes,   				/* number of prime implicants in original expression */
	 covers;		  				/* number of prime implicants in cover expression */

	unsigned expr_variables,		/* number of active variables */
	 cover_variables;				/* number of cover variables (= #prime implicants) */

	clock_t start_time,				/* time this run started */
	 temp_time,						/* a temporary */
	 expr_minterm_time,				/* time taken to expand original expression */
	 expr_merge_time,				/* time taken to merge original expression */
	 p_func_time,					/* time taken to generate p_func */
	 cover_minterm_time,            /* time taken to expand cover expression */
	 cover_merge_time,              /* time taken to merge cover expression */
	 factorisation_time;			/* time taken to factorise result */

	*cc = 0;						/* put a null at the beginning of the buffer */
	parse_switches(argv);			/* get user parameters */
	switch (setjmp(err))
		;							/* establish error handler */

	while (!feof(stdin))			/* while more input available */
	{
		read_expression();			/* pull in the next expression */
		start_time=clock();			/* intialise CPU time counter */

		if (*buffer == 0)
			continue;				/* throw away a null expression */

		verbose_printf("\nExpanding %s\n", buffer);	/* be chatty */

		if (cc = buffer, e1(), *cc != 0)
			error();				/* preliminary run to check for errors */

		/* change variables to 'a'..<'a'+expr_variables> */
		expr_variables = map_expression(buffer);

		/* generate canonical sum of products */
		temp_time=clock();
		expression = expand_expression(buffer, expr_variables, flags.truth_table, &expr_minterms);
		expr_minterm_time=clock()-temp_time;

		verbose_printf("\nCanonical sum of products is ");
		print_expression(expression);
		verbose_printf(";\n");

		if (flags.karnaugh)			/* do karnaugh map */
			print_karnaugh();

		if (flags.minimise)
		{
			temp_time=clock();
			prime = prime_expression(expression, expr_variables, &expr_merges);
			expr_merge_time=clock()-temp_time;

			if (flags.cover && label_primes(prime, &expr_primes))
			{
				temp_time=clock();
				build_prime_implicant_function(expression, prime);
				p_func_time=clock()-temp_time;
				print_implicants(prime);				/* and print */
				verbose_printf("\nPrime implicant function is %s\n", buffer);
				cover_variables = map_expression(buffer);		/* remap variables */
				temp_time=clock();
				cover = expand_expression(buffer, cover_variables, 0, &cover_minterms);
				cover_minterm_time=clock()-temp_time;

				verbose_printf("\nExpanded prime implicant function is ");
				print_expression(cover);
				verbose_printf(";\n");

				temp_time=clock();
				cover = prime_expression(cover, cover_variables, &cover_merges);
				cover_merge_time=clock()-temp_time;

				verbose_printf("\nReduced prime implicant function is ");
				print_expression(cover);
				verbose_printf(";\n\n");

				label_prime_implicant_function(cover);	/* label each one */
				mincover = print_covers(cover, &covers);	/* find minimum cover */
				verbose_printf("\nA minimal cover is ");
				print_product(mincover);

				memcpy(conv, old_conv, sizeof(old_conv));	/* restore conversion */
				expression = cover_as_expression(mincover);	/* print out in original form */
				verbose_printf(" which corresponds to ");
				print_expression(expression);
				verbose_printf(";\n");
			}
			else expression=prime;
        }

		if (flags.factor)
		{
			temp_time=clock();
			factorise(&expression);		/* factorise */
			factorisation_time=clock()-temp_time;
			verbose_printf("\nFactorised to ");
			print_expression(expression);
			verbose_printf(";\n");
		}

		verbose_printf("\nFinal result ");	/* printf final result */

		force_print_expression(expression);
		printf(";\n");

		if (flags.statistics)			/* dump statistics out */
		{
			printf("\n%lu minterm%c", expr_minterms, expr_minterms==1?' ':'s');
			if (flags.minimise)
			{
				printf(" %lu merge%c", expr_merges, expr_merges==1?' ':'s');
				if (flags.cover)
				{
					printf(" %lu prime%c", expr_primes, expr_primes==1?' ':'s');
					printf(" %lu p-minterm%c", cover_minterms, cover_minterms==1?' ':'s');
					printf(" %lu p-merge%c", cover_merges, cover_merges==1?' ':'s');
					printf(" %lu cover%c", covers, covers==1?' ':'s');
				}
			}
			printf("\n");

#define real(a) ( (double) (a) / (double) CLOCKS_PER_SEC )

			printf("\nOriginal function: expansion %.2lfs", real(expr_minterm_time));
			if (flags.minimise)
			{
				printf(", merging %.2lfs.\n", real(expr_merge_time));
				if (flags.cover)
				{
					printf("Prime implicant function: generation %.2lfs, ", real(p_func_time));
					printf("expansion %.2lfs, ", real(cover_minterm_time));
					printf("mergeing %.2lfs.", real(cover_merge_time));
				}
			}
			else
				printf(".");

			if (flags.factor)
				printf("\nFactorisation %.3lfs.", factorisation_time);
			printf("\n");

			printf("\nbool used %.3lf CPU seconds\n\n", real(clock()-start_time));
		}
	}
}
