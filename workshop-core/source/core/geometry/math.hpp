#pragma once

#include "core/math/point.h"
#include "core/math/line.h"
#include "core/math/triangle.h"

#include <vector>
#include <cmath>

namespace wk::Geometry
{
	template<typename T1, typename T2>
	float dist(const Point_t<T1>& p1, const Point_t<T2>& p2) {
		return (float)std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
	}

	template<typename T>
	float line_angle(const Line_t<T>& line)
	{
		return (float)std::atan2(line.end.y - line.start.y, line.end.x - line.start.x);
	}

	template<typename T>
	Triangle_t<T> build_triangle(const Line_t<T>& bisector, float angle, const T& length)
	{
		T half_length = (T)((float)length / 2);

		const Point_t<T>& midpoint = bisector.end;

		T x1 = (T)(midpoint.x + half_length * std::cos(angle));
		T y1 = (T)(midpoint.y + half_length * std::sin(angle));

		T x2 = (T)(midpoint.x - half_length * std::cos(angle));
		T y2 = (T)(midpoint.y - half_length * std::sin(angle));

		return Triangle_t<T>(
			bisector.start,
			Point_t<T>(x1, y1),
			Point_t<T>(x2, y2)
		);
	}

	template<typename T>
	bool point_inside_polygon(const std::vector<Point_t<T>> polygon, Point_t<T> point)
	{
		bool result = false;

		for (size_t i = 0, j = polygon.size() - 1; i < polygon.size(); j = i++)
		{
			const auto& p1 = polygon[i];
			const auto& p2 = polygon[j];

			bool intersect = ((p1.y > point.y) != (p2.y > point.y)) && (point.x < (p2.x - p1.x) * (point.y - p1.y) / (p2.y - p1.y) + p1.x);
			if (intersect)
			{
				result = !result;
			}
		}

		return result;
	}
}