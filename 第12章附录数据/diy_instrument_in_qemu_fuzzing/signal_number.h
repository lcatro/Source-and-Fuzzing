
#ifndef __SIGNAL_NUMBER_H__
#define __SIGNAL_NUMBER_H__

#define SIGRTMIN                    34

//#define SIGNAL_INVALID              SIGRTMIN + 10
#define SIGNAL_CREATE_FUZZER_TARGET (SIGRTMIN + 10)
#define SIGNAL_FUZZ_ONCE            (SIGRTMIN + 11)

#define TRAP_SIGNAL (SIGNAL_CREATE_FUZZER_TARGET | SIGNAL_FUZZ_ONCE)

#endif



