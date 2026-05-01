/* noise.c
 * Get environmental noise.
 * This is adapted from code in the Pretty Good Privacy (PGP) package.
 * Written by Colin Plumb.
 */

#include <time.h>
#include "usuals.h"
#include "randpool.h"
#include "noise.h"

#if defined(MSDOS) || defined(__MSDOS__)	/* Use 1.19 MHz PC timer */

#include <dos.h>	/* for enable() and disable() */
#include <conio.h>	/* for inp() and outp() */

/*
 * This code gets as much information as possible out of 8253/8254 timer 0,
 * which ticks every .84 microseconds.  There are three cases:
 * 1) Original 8253.  15 bits available, as the low bit is unused.
 * 2) 8254, in mode 3.  The 16th bit is available from the status register.
 * 3) 8254, in mode 2.  All 16 bits of the counters are available.
 *    (This is not documented anywhere, but I've seen it!)
 *
 * This code repeatedly tries to latch the status (ignored by an 8253) and
 * sees if it looks like xx1101x0.  If not, it's definitely not an 8254.
 * Repeat this a few times to make sure it is an 8254.
 */
static int
has8254(void)
{
	int i, s1, s2;

	for (i = 0; i < 5; i++) {
		disable();
		outp(0x43, 0xe2);	/* Latch status for timer 0 */
		s1 = inp(0x40);		/* If 8253, read timer low byte */
		outp(0x43, 0xe2);	/* Latch status for timer 0 */
		s2 = inp(0x40);		/* If 8253, read timer high byte */
		enable();
		if ((s1 & 0x3d) != 0x34 || (s2 & 0x3d) != 0x34)
			return 0;	/* Ignoring status latch; 8253 */
	}
	return 1;	/* Status reads as expected; 8254 */
}

/* TODO: It would be better to capture this data in a keyboard ISR */
static unsigned
read8254(void)
{
	unsigned status, count;

	disable();
	outp(0x43, 0xc2);	/* Latch status and count for timer 0 */
	status = inp(0x40);
	count = inp(0x40);
	count |= inp(0x40) << 8;
	enable();
	/* The timer is usually in mode 3, but some motherboards use mode 2. */
	if (status & 2)
		count = count>>1 | (status & 0x80)<<8;

	return count;
}

static unsigned
read8253(void)
{
	unsigned count;

	disable();
	outp(0x43, 0x00);	/* Latch count for timer 0 */
	count = (inp(0x40) & 0xff);
	count |= (inp(0x40) & 0xff) << 8;
	enable();

	return count >> 1;
}
#endif /* MSDOS || __MSDOS__ */


#ifdef UNIX

#include <sys/types.h>
#include <sys/time.h>	/* For gettimeofday() */
#include <sys/times.h>	/* for times() */
#include <stdlib.h>	/* For qsort() */
#define N 15	/* Number of deltas to try (at least 5, preferably odd) */

/* Function needed for qsort() */
static int
noiseCompare(void const *p1, void const *p2)
{	return *(int const *)p1 - *(int const *)p2;	}

/*
 * Find the resolution of the gettimeofday() clock by sampling successive
 * values until a tick boundary, at which point the delta is entered into
 * a table.  An average near the median of the table is taken and returned
 * as the system tick size to eliminate outliers due to descheduling (high)
 * or tv0 not being the "zero" time in a given tick (low).
 *
 * Some trickery is needed to defeat the habit systems have of always
 * incrementing the microseconds field so that no two calls return the
 * same value.  Thus, a "tick boundary" is assumed when successive calls
 * return a difference of > 2us.  This catches cases where at most one other
 * task reads the clock between successive reads by this task.  More tasks
 * in between are rare enough that they'll get cut off by the median filter.
 *
 * When a tick boundary is found, the *first* time read during the previous
 * tick (tv0) is subtracted from the new time to get microseconds per tick.
 *
 * Suns have a 1 us timer, and as of SunOS 4.1, they return that timer, but
 * there is ~50 us of system-call overhead to get it, so this overestimates
 * the tick size considerably.  On SunOS 5.x/Solaris, the overhead has been
 * cut to about 2.5 us, so the measured time alternates between 2 and 3 us.
 * Some better algorithms will be required for future machines that really
 * do achieve 1 us granularity.
 *
 * The alternative to all this is to model the intervals and compute the
 * entropy directly.  A model considering the previous interval only should
 * be adequate for keystrokes.
 */
static unsigned
noiseTickSize(void)
{
	int i = 0, j = 0, d[N];
	struct timeval tv0, tv1, tv2;

	gettimeofday(&tv0, (struct timezone *)0);
	tv1 = tv0;
	do {
		gettimeofday(&tv2, (struct timezone *)0);
		if (tv2.tv_usec > tv1.tv_usec+2) {
			d[i++] = tv2.tv_usec - tv0.tv_usec +
				1000000 * (tv2.tv_sec - tv0.tv_sec);
			tv0 = tv2;
			j = 0;
		} else if (++j > 10000)	/* Always getting <= 2 us, */
			return 2;	/* so assume 2us ticks */
		tv1 = tv2;
	} while (i < N);

	/* Return average of middle 5 values (rounding up) */
	qsort(d, N, sizeof(d[0]), noiseCompare);
	return (d[N/2-2]+d[N/2-1]+d[N/2]+d[N/2+1]+d[N/2+2]+4)/5;
}

#endif /* UNIX */


/*
 * Add as much time-dependent random noise to the randPool as possible.
 * This involves reading the most accurate system clocks available.
 *
 * Returns the number of ticks that have passed since the last call,
 * for entropy estimation purposes.
 */
word32
noise(void)
{
	static word32 lastcounter;
	word32 delta;
	time_t tnow;
/*nas->
	clock_t cnow;
/*nas*/

#if defined(MSDOS) || defined(__MSDOS__)
	static unsigned deltamask = 0;
	unsigned t;

	if (deltamask == 0)
		deltamask = has8254() ? 0xffff : 0x7fff;
	t = (deltamask & 0x8000) ? read8254() : read8253();
	randPoolAddBytes((byte const *)&t, sizeof(t));
	delta = deltamask & (t - (unsigned)lastcounter);
	lastcounter = t;
#elif defined(VMS)
	word32 t[2];

	SYS$GETTIM(t);	/* VMS hardware clock increments by 100000 per tick */
	randPoolAddBytes((byte const *)t, sizeof(t));
	delta = (t[0]-lastcounter)/100000;
	lastcounter = t[0];
#elif defined(UNIX)
	static unsigned ticksize = 0;
	struct timeval tv;
	struct tms tms;

	gettimeofday(&tv, (struct timezone *)0);
	randPoolAddBytes((byte const *)&tv, sizeof(tv));
	cnow = times(&tms);
	randPoolAddBytes((byte const *)&tms, sizeof(tms));
	randPoolAddBytes((byte const *)&cnow, sizeof(cnow));

	/* This may wrap, but it's unsigned, so that's okay */
	tv.tv_usec += tv.tv_sec * 1000000;
	if (!ticksize)
		ticksize = noiseTickSize();
	delta = (tv.tv_usec-lastcounter)/ticksize;
	lastcounter = tv.tv_usec;
#else
#error Unknown operating system
#endif
/*nas->
	cnow = clock();
	randPoolAddBytes((byte const *)&cnow, sizeof(cnow));
/*nas*/
	tnow = time((time_t *)0);		/* Read slowest clock last */
	randPoolAddBytes((byte const *)&tnow, sizeof(tnow));

	return delta;
}
