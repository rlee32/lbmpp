#pragma once

#include <iostream>

template <typename T>
inline void Log(T content)
{
    std::cout << content << std::endl;
}

template <typename T, typename... Ts>
inline void Log(T content, Ts... other)
{
    std::cout << content;
    Log(other...);
}

