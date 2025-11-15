#pragma once
#include <format>
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
    T& register_service(Args&&... args) {
        auto service = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *service;
        services_[std::type_index(typeid(T))] = std::move(service);
        return ref;
    }

    template<typename T>
        requires EngineService<T>
    void unregister_service() {
        services_.erase(std::type_index(typeid(T)));
    }

    template<typename T>
    requires EngineService<T>
    std::reference_wrapper<T> get_service() const {
        const auto it = services_.find(std::type_index(typeid(T)));
        if (it == services_.end()) {
            throw std::runtime_error(std::format("Service not found: {}", typeid(T).name()));
        }
        // So we get an EngineServiceBase, but we know it's actually a T. We do not use dynamic cast because we are not storing subtypes of T
        return std::ref(*static_cast<T*>(it->second.get()));
    }

    template <typename T>
        requires EngineService<T>
    std::optional<std::reference_wrapper<T>> try_get_service() const {
        const auto it = services_.find(std::type_index(typeid(T)));
        if (it == services_.end()) {
            return std::nullopt;
        }

        return std::ref(*static_cast<T*>(it->second.get()));
    }

private:
    std::unordered_map<std::type_index, std::unique_ptr<IEngineService>> services_;
};
