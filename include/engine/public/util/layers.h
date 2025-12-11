#pragma once

// We explicitly use an enum here instead of an enum class, because we want the ability to
// automagically convert from enum values to integers.
enum Layers : int {
  Background = 0,
  Foreground = 10,
  Default = Foreground,
  UI = 50,
};