/* randtest.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>	/* For rand(), srand() and RAND_MAX */

#include "randpool.h"
#include "noise.h"

/*
 * This function returns random numbers chosen uniformly from 0..range-1.
 * (If range is more than RAND_MAX+1, here will be a divide by zero error.)
 * The usual rand()%range does not return a uniform number, even assuming
 * that rand()'s return value is uniform.
 *
 * The technique is to divide by div = floor((RAND_MAX+1)/range), which will
 * usually produce a value uniformly distributed between 0 and range-1, but
 * will sometimes (probability (RAND_MAX+1)%range / (RAND_MAX+1)) produce
 * the value range.  In that case, retry.
 *
 * This example uses rand() because it's just for a cute visual effect,
 * not something critical.  Simple PRNGs are useful sometimes!
 */
static unsigned
randRange(unsigned range)
{
	unsigned result, div = ((unsigned)RAND_MAX+1)/range;

	while ((result = rand()/div) == range)
		/* retry */ ;

	return result;
}

/* Cute Wargames-like random effect thrown in for fun */
static void
funnyprint(char const *string)
{
	static const char alphabet[] =
"ABCDEFGHIJKLMNOPWRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
	char c, flag[80] = {0};	/* 80 is maximum line length */
	unsigned i, tumbling = 0, len = strlen(string);

	/* We don't need good random numbers, so just use a good seed */
	/* rand() varies between systems, so ignore byte-order issues */
	randPoolGetBytes((byte *)i, sizeof(i));
	srand(i);

	/* Truncate longer strings (unless you have a better idea) */
	if (len > sizeof(flag))
		len = sizeof(flag);

	/* Count letters that we can tumble (letters in the alphabet) */
	for (i = 0; i < len; i++) {
		if (strchr(alphabet, string[i])) {
			flag[i] = 1;	/* Increase this for more tumbling */
			tumbling++;
		}
	}

	/* Print until all characters are stable. */
	do {
		putchar('\r');
		for (i = 0; i < len; i++) {
			if (flag[i]) {
				c = alphabet[randRange(sizeof(alphabet)-1)];
				if (c == string[i] && --flag[i] == 0)
					tumbling--;
			} else {
				c = string[i];
			}
			putchar(c);
		}
		fflush(stdout);
	} while (tumbling);

	putchar('\n');
}

/*
 * Gather entropy from keyboard timing.  This is currently MS-DOS specific,
 * as it uses the getch() function to read a signle keystroke without echo.
 *
 * This number of bits gathered is computed in 1/16 of a bit units.
 */
#include <conio.h>	/* For getch() */

#define FRACBITS 4

static void
randAccum(int bits)
{
	word32 delta;
	int c, oldc = 0, olderc = 0;

	if (bits > RANDPOOLBITS)
		bits = RANDPOOLBITS;
	bits <<= FRACBITS;

	puts("We are generating some truly random bits by timing your\n"
	     "keystrokes. Please type until the counter reaches 0.\n");

	while (bits > 0) {
		printf("\r%4d ", (bits-1 >> FRACBITS) + 1);

		c = getch();
		delta = noise()/6;	/* Add time of keystroke */
		if (c == 0)
			c = 0x100 + getch();	/* Handle function keys */
		randPoolAddBytes((byte const *)&c, sizeof(c));

		/* Normal typing has double letters, but discard triples */
		if (c == oldc && c == olderc)
			continue;
		olderc = oldc;
		oldc = c;

		if (delta) {	/* Subtract log2(delta) from bits */
			/* Integer bits first, normalizing */
			bits -= 31<<FRACBITS;
			while (delta < 1ul<<31) {
				bits += 1<<FRACBITS;
				delta <<= 1;
			}

			/* Fractional bits, using integer log algorithm */
			for (c = 1 << FRACBITS-1; c; c >>= 1) {
				delta >>= 16;
				delta *= delta;
				if (delta >= 1ul<<31)
					bits -= c;
				else
					delta <<= 1;
			}
		}
	}
	puts("\r    0 Thank you, that's enough.");
}

/*
 * A little program to test the correct operation of the randpool.c
 * functions.  Requires noise.[ch], randpool.[ch], md5.[ch] and usuals.h.
 *
 * When invoked with the argument "foo", it should start with:
 *
 * Adding "foo\0" to pool.
 *
 * Pseudo-random bytes:
 * 4c 9d 41 ba 44 41 63 a1 db 1c ab 3f 52 a1 a2 84 c3 e5 dc bc 57 4c d9 f3 38
 * d7 45 50 f9 94 36 96 a3 df 90 ff 23 e5 ec 3c 76 1f ce 1c bc d6 79 8b 5e e7
 * aa 97 16 c0 50 c6 95 0b c1 62 42 e5 5b 8f d7 bd d7 70 1f c6 60 6a 5f f3 74
 * 8d 35 ad 51 5a 4a 0c 02 cd d5 36 7e d4 c2 d9 f0 d3 49 ed 2d fa 4e 2b 70 3f
 *
 * The trailing null is included in the bytes added to ensure that the
 * argument lists "foo" and "f o o" are considered distinct.
 */
int
main(int argc, char **argv)
{
	int i;

	while (--argc) {
		printf("Adding \"%s\\0\" to the pool.\n", *++argv);
		randPoolAddBytes((byte const *)*argv, strlen(*argv)+1);
	}

	puts("\nPseudo-random bytes:");
	i = 100;
	while (i--)
		printf("%02x%c", randPoolGetByte(), i % 25 ? ' ' : '\n');

	putchar('\n');
/*	funnyprint("This will be deterministic on a given system.");/**/
	putchar('\n');

	noise();	/* Establish a baseline for the deltas */
	randAccum(800);	/* 800 random bits = 100 random bytes */

	puts("\nTruly random bytes:");
	i = 100;
	while (i--)
		printf("%02x%c", randPoolGetByte(), i % 25 ? ' ' : '\n');

	putchar('\n');
/*	funnyprint("This will be unpredictable.");/**/

	return 0;
}
