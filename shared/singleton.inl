#include "singleton.hpp"

template <typename T>
T& Singleton<T>::GetInstance()
{
    static T instance;
    return instance;
};