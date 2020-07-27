#pragma once

#include <format.h>

#define xassert(expr, msg, ...) \
{ \
	do { \
		if (!(expr)) \
		{ \
			fmt::printf("Assertion failed: %s\n\t" msg "\n\tIn " __FILE__ ":%u\n\n", #expr, __VA_ARGS__, __LINE__); \
			abort(); \
		} \
	} while (0); \
}