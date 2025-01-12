#pragma once

#include "math.hpp"

#include <optional>
#include <tuple>

namespace wk::Geometry
{
	// https://flassari.is/2008/11/line-line-intersection-in-cplusplus
	template<typename T>
	std::optional<Point_t<T>> line_intersect(const Line_t<T> l1, const Line_t<T> l2)
	{
		float x1 = l1.start.x, x2 = l1.end.x, x3 = l2.start.x, x4 = l2.end.x;
		float y1 = l1.start.y, y2 = l1.end.y, y3 = l2.start.y, y4 = l2.end.y;

		float determinant = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

		if (determinant == 0)
		{
			return std::nullopt;
		};

		// Get the x and y
		float pre = (x1 * y2 - y1 * x2), post = (x3 * y4 - y3 * x4);
		float x = (pre * (x3 - x4) - (x1 - x2) * post) / determinant;
		float y = (pre * (y3 - y4) - (y1 - y2) * post) / determinant;

		// Check if the x and y coordinates are within both lines
		if (x < std::min(x1, x2) || x > std::max(x1, x2) ||
			x < std::min(x3, x4) || x > std::max(x3, x4))
		{
			return std::nullopt;
		};

		if (y < std::min(y1, y2) || y > std::max(y1, y2) ||
			y < std::min(y3, y4) || y > std::max(y3, y4))
		{
			return std::nullopt;
		};

		return PointF(x, y);
	}

	template<typename T>
	std::optional<std::tuple<size_t, size_t, Point_t<T>>> line_intersect(const std::vector<Point>& polygon, const Line_t<T> line)
	{
		size_t length = polygon.size();
		for (size_t i = 0; length > i; i++)
		{
			size_t p1_idx = i;
			size_t p2_idx = (i + 1) % length;

			const Point& p1 = polygon[p1_idx];
			const Point& p2 = polygon[p2_idx];

			LineF candidate(
				LineF::ValueT((float)p1.x, (float)p1.y),
				LineF::ValueT((float)p2.x, (float)p2.y)
			);

			auto result = line_intersect(candidate, line);
			if (result.has_value())
			{
				return std::make_tuple(p1_idx, p2_idx, result.value());
			}
		}

		return std::nullopt;
	}
}