#pragma once
#include <iostream>
#include <vector>
#include <string>

// defining a macro, anything after the second space will replace the macro
        // macro              // compiler version

// cout a white message to the console
#define NOVO_MSG_LOG(cn, msg) std::cout << cn << " | " << msg << std::endl;
// cout a red message to the console       // red colour                        // reset to normal
#define NOVO_MSG_ERR(cn, msg) std::cout << "\033[31m" << cn << " | " << msg << "\033[0m" << std::endl;

// type define vector as a TArray and template the typename
// at compile time, the compiler will replace "TArray" with std::vector<typename>
template <typename T> 
using TArray = std::vector<T>;

typedef unsigned int NovoUint;

typedef std::string NovoString;