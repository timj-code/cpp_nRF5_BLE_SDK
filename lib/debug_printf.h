#ifndef DEBUG_PRINTF_H
#define DEBUG_PRINTF_H

#ifdef DEBUG
#define DEBUG_PRT(...)           \
	{                            \
		{                        \
			printf(__VA_ARGS__); \
		}                        \
	}
#else
#define DEBUG_PRT(...) (void)0
#endif

#endif
