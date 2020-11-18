#pragma once

#ifdef __WINDOWS
	#ifdef _DLL_EXPORTS
	#define DLL_API _declspec(dllexport) 
	#else
	#define DLL_API _declspec(dllimport)
	#endif

#else
	#define DLL_API 
#endif
