/* randpool.h */
#include "usuals.h"

/* Set this to whatever you need (must be > 512) */
#define RANDPOOLBITS 3072

#define RAND_MAX 255

void randPoolStir(void);
void randPoolAddBytes(byte const *buf, unsigned len);
void randPoolGetBytes(byte *buf, unsigned len);
byte randPoolGetByte(void);
