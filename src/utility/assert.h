#ifndef TFF_UTILITY_ASSERT_H_
#define TFF_UTILITY_ASSERT_H_

#include "../config.h"
#include "exception.h"

#define TFF_STRINGIZE_(X) #X
#define TFF_STRINGIZE(X) TFF_STRINGIZE_(X)

// to be called like TFF_GET_NARG_MACRO_2(__VA_ARGS__, MACRO2, MACRO1, IGNORE)
// if __VA_ARGS__ contains 1 item, resolves into MACRO1
// if __VA_ARGS__ contains 2 items, resolves into MACRO2
#define TFF_GET_NARG_MACRO_2(_1, _2, NAME, ...) NAME

#ifdef _MSC_VER
	#define TFF_ASSUME(__invariant__) __assume(__invariant__)
	#define TFF_LIKELY(__condition__) (__condition__)
	#define TFF_UNLIKELY(__condition__) (__condition__)
#else
	#define TFF_ASSUME(__invariant__) __builtin_assume(__invariant__)
	#define TFF_LIKELY(__condition__) __builtin_expect(!!(__condition__), 1)
	#define TFF_UNLIKELY(__condition__) __builtin_expect(!!(__condition__), 0)
#endif


#if TFF_DEBUG_BUILD
	#define TFF_ASSERT_CRIT_MSG_(__condition__, __msg__) \
		if(! TFF_UNLIKELY(static_cast<bool>(__condition__))) \
			throw ::tff::failed_assertion(__msg__ " at " __FILE__ ":" TFF_STRINGIZE(__LINE__))
	#define TFF_ASSERT_MSG_(__condition__, __msg__) \
		if(! TFF_UNLIKELY(static_cast<bool>(__condition__))) \
			throw ::tff::failed_assertion(__msg__ " at " __FILE__ ":" TFF_STRINGIZE(__LINE__))
#else
	#define TFF_ASSERT_CRIT_MSG_(__condition__, __msg__) \
		(void)0
	#define TFF_ASSERT_MSG_(__condition__, __msg__) \
		if(! TFF_UNLIKELY(static_cast<bool>(__condition__))) \
			throw ::tff::failed_assertion(__msg__ " at " __FILE__ ":" TFF_STRINGIZE(__LINE__))
#endif


#define TFF_ASSERT_(__condition__) TFF_ASSERT_MSG_(__condition__, "`" #__condition__ "`")
#define TFF_ASSERT_CRIT_(__condition__) TFF_ASSERT_CRIT_MSG_(__condition__, "`" #__condition__ "`")

#ifdef _MSC_VER
	// workaround for MSVC: http://stackoverflow.com/a/5134656/4108376
	#define TFF_EXPAND_(x) x
	#define TFF_ASSERT(...) TFF_EXPAND_( TFF_GET_NARG_MACRO_2(__VA_ARGS__, TFF_ASSERT_MSG_, TFF_ASSERT_, IGNORE)(__VA_ARGS__) )
	#define TFF_ASSERT_CRIT(...) TFF_EXPAND_( TFF_GET_NARG_MACRO_2(__VA_ARGS__, TFF_ASSERT_CRIT_MSG_, TFF_ASSERT_CRIT_, IGNORE)(__VA_ARGS__) )
#else
	#define TFF_ASSERT(...) TFF_GET_NARG_MACRO_2(__VA_ARGS__, TFF_ASSERT_MSG_, TFF_ASSERT_, IGNORE)(__VA_ARGS__)
	#define TFF_ASSERT_CRIT(...) TFF_GET_NARG_MACRO_2(__VA_ARGS__, TFF_ASSERT_CRIT_MSG_, TFF_ASSERT_CRIT_, IGNORE)(__VA_ARGS__)
#endif

#define Assert TFF_ASSERT
#define Assert_crit TFF_ASSERT_CRIT


#endif
