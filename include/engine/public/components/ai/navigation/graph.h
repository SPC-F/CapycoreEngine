#pragma once

#include <functional>

template <typename T>
struct GraphEdge {
  std::reference_wrapper<T> target;
  std::reference_wrapper<T> source;
  float cost;
};

struct GraphPosition {
  int x, y;

  bool operator==(const GraphPosition& other) const noexcept {
    return x == other.x && y == other.y;
  }
};

struct GraphPositionHash {
  size_t operator()(const GraphPosition& p) const noexcept {
    return (std::hash<int>()(p.x) << 1) ^ std::hash<int>()(p.y);
  }
};
