#pragma once

/**
 * @brief Predefined layers for GameObjects.
 * Layers are used to categorize GameObjects for rendering and collision
 * detection.
 *
 * @note The Default layer is set to Foreground.
 * @note This enum is not an enum class to allow implicit conversion to int.
 */
enum Layers : int {
  Background = 0,
  Foreground = 10,
  Default = Foreground,
  UI = 50,
};