#pragma once

#ifdef _WIN32
	#if defined(SC_DLL_EXPORT) && (SC_DLL_EXPORT==1)
		#define SUPERCELL_API __declspec(dllexport)
	#elif defined(SC_DLL_IMPORT) && (SC_DLL_IMPORT==1)
		#define SUPERCELL_API __declspec(dllimport)
	#else
		#define SUPERCELL_API
	#endif
#else
	#define SUPERCELL_API
#endif
