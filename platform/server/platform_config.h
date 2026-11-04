
#if defined(__linux__) || defined(__APPLE__)
#include <alloca.h>
#endif
#if defined(__FreeBSD__) || defined(__OpenBSD__)
#include <stdlib.h>
#define PTHREAD_BSD_SET_NAME
#endif
#ifdef __APPLE__
#define PTHREAD_RENAME_SELF
#endif
