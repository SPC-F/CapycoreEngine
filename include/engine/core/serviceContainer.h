#pragma once
#include <memory>
#include <optional>
#include <typeindex>
#include <unordered_map>

#include <engine/core/iEngineService.h>

class ServiceContainer final {
public:

    ServiceContainer();

    template<typename T, typename... Args>
        requires EngineService<T>
    T& registerService(Args&&... args) {
        auto service = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *service;
        services_[std::type_index(typeid(T))] = std::move(service);
        return ref;
    }

    template<typename T>
        requires EngineService<T>
    void unregisterService() {
        services_.erase(std::type_index(typeid(T)));
    }

    template<typename T>
    requires EngineService<T>
    std::reference_wrapper<T> getService() const {
        const auto it = services_.find(std::type_index(typeid(T)));
        if (it == services_.end()) {
            throw std::runtime_error("Service not found: " + std::string(typeid(T).name()));
        }
        // So we get an EngineServiceBase, but we know it's actually a T. We do not use dynamic cast because we are not storing subtypes of T
        return std::ref(*static_cast<T*>(it->second.get()));
    }

    template <typename T>
        requires EngineService<T>
    std::optional<std::reference_wrapper<T>> tryGetService() const {
        const auto it = services_.find(std::type_index(typeid(T)));
        if (it == services_.end()) {
            return std::nullopt;
        }

        return std::ref(*static_cast<T*>(it->second.get()));
    }

private:
    std::unordered_map<std::type_index, std::unique_ptr<IEngineService>> services_;
};
