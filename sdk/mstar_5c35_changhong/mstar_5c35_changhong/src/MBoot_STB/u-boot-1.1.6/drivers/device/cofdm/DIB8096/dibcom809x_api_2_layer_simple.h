#ifndef __DIBCOM_809X_SIMPLE_API__
#define __DIBCOM_809X_SIMPLE_API__

unsigned char dib_init(void);
void dib_tune(unsigned int frequency, unsigned char bandwidth);
unsigned char dib_lockstatus(void);
unsigned char dib_signal_strength(void);
unsigned char dib_signal_quality(void);

#endif

