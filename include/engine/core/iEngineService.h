#pragma once
#include <concepts>

class IEngineService {
public:
    virtual ~IEngineService() = default;
};

template<typename T>
concept EngineService = std::derived_from<T, IEngineService>;