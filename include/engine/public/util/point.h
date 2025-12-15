#pragma once
class Point {
 public:
  Point();
  Point(float x, float y);
  float x;
  float y;

  Point distance_to(const Point& other) const;
};