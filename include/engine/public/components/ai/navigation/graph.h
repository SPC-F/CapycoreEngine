#pragma once

#include <cfloat>
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

struct NodeRecord {
  GraphPosition pos;
  float g = FLT_MAX;
  float h = 0.f;
  GraphPosition parent;
  bool has_parent = false;

  float f() const { return g + h; }
};
