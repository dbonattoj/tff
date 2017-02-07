#ifndef TFF_OS_H_
#define TFF_OS_H_

// macros for operating system
#if defined(__APPLE__)
	#define TFF_OS_DARWIN
#elif defined(_WIN32)
	#define TFF_OS_WINDOWS
#elif defined(__linux__)
	#define TFF_OS_LINUX
#else
	#error Unknown operating system
#endif

// macros for compiler
#if defined(__clang__)
	#define TFF_COMPILER_CLANG
#elif defined(__GNUC__)
	#define TFF_COMPILER_GCC
#elif defined(_MSC_VER)
	#define TFF_COMPILER_MSVC
#endif

#endif