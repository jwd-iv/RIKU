#pragma once

#ifdef _WIN32
#  ifdef RIKU_SHAREDLIB
#    ifdef RIKU_BUILD
#      define RIKU_API __declspec(dllexport)
#      define RIKU_GENAPI __declspec(dllexport)
#    else
#      define RIKU_API __declspec(dllimport)
#      define RIKU_GENAPI __declspec(dllexport)
#    endif
#  else
#    define RIKU_API 
#    define RIKU_GENAPI 
#  endif
#else
#  define RIKU_API 
#  define RIKU_GENAPI 
#endif

#ifndef RIKU_MAX_VAR_SIZE
#  define RIKU_MAX_VAR_SIZE sizeof(long long unsigned)
#endif
