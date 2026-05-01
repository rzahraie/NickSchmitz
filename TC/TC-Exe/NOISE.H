/* noise.h
 * Get environmental noise for RNG
 *
 * The interface is system-independent, but the
 * implementation should be highly system-dependent,
 * to get at as much state as possible.
 */

#include "usuals.h"

typedef long    clock_t;

#define MSDOS
#define _MSDOS_

word32 noise(void);
