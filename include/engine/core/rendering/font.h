#pragma once

#include <string>
#include <memory>

class TTF_Font; // NOLINT
class TTF_CloseFont; // NOLINT

class Font final {
public: 
    [[nodiscard]] std::string get_name() const;
    [[nodiscard]] int8_t get_size() const;
    [[nodiscard]] TTF_Font& get_ttf_font() const;

private:
    int8_t size_;
    std::string name_;
    std::unique_ptr<TTF_Font, void(*)(TTF_Font*)> font_;

    friend class AssetService;

    explicit Font(std::string name, int8_t size, TTF_Font* font);
};