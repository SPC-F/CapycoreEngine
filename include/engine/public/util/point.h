#pragma once

/**
 * @brief Represents a point in 2D space.
 */
class Point {
 public:
  Point();
  Point(float x, float y);
  float x;
  float y;

  /**
   * @brief Calculates the distance to another point.
   * @param other The other point to calculate the distance to.
   * @return The distance as a Point object.
   */
  Point distance_to(const Point& other) const;
};