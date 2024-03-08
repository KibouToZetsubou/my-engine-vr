#pragma once

#include <iostream>

#ifdef _WINDOWS
#ifdef SIMPLEDYNLIB_EXPORTS
#define LIB_API __declspec(dllexport)
#else
#define LIB_API __declspec(dllimport)
#endif
#else
#define LIB_API
#endif

#define _DEBUG

#ifdef _DEBUG
    #define DEBUG( message ) \
        std::cout << "[DEBUG] " << message << std::endl;
#else
    #define DEBUG( message )
#endif

#define WARNING( message ) \
    std::cout << "[WARNING] " << message << std::endl;

#define ERROR( message ) \
    std::cerr << "[ERROR] " << message << std::endl;
