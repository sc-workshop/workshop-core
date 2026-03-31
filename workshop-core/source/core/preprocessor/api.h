#pragma once

#if defined(_MSC_VER)
    #define WK_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
    #define WK_INLINE __attribute__((always_inline))
#else
    #define WK_INLINE inline
#endif

#ifdef _WIN32
	#if defined(WK_DLL_EXPORT) && (WK_DLL_EXPORT==1)
		#define WORKSHOP_API __declspec(dllexport)
	#elif defined(WK_DLL_IMPORT) && (WK_DLL_IMPORT==1)
		#define WORKSHOP_API __declspec(dllimport)
	#else
		#define WORKSHOP_API
	#endif
#else
	#define WORKSHOP_API
#endif
