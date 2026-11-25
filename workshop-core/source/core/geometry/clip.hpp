#pragma once

#include "core/math/point.h"

#include <vector>

namespace wk::Geometry
{
	// Clip a convex polygon by the half-plane opposite `cut_corner`.
	//
	// `line_p1`/`line_p2` define the cut boundary. The side containing
	// `cut_corner` is discarded. Vertex order follows Sutherland-Hodgman, so
	// the input winding is preserved.
	template<typename T>
	std::vector<Point_t<T>> clip_polygon_by_line(
		const std::vector<Point_t<T>>& polygon,
		const Point_t<T>& line_p1,
		const Point_t<T>& line_p2,
		const Point_t<T>& cut_corner
	)
	{
		std::vector<Point_t<T>> result;
		if (polygon.empty()) return result;

		const T edge_x = line_p2.x - line_p1.x;
		const T edge_y = line_p2.y - line_p1.y;
		const T corner_side = edge_x * (cut_corner.y - line_p1.y) - edge_y * (cut_corner.x - line_p1.x);

		result.reserve(polygon.size() + 1);

		const size_t count = polygon.size();
		for (size_t i = 0; count > i; i++)
		{
			const Point_t<T>& start = polygon[i];
			const Point_t<T>& end = polygon[(i + 1) % count];

			const T start_side = edge_x * (start.y - line_p1.y) - edge_y * (start.x - line_p1.x);
			const T end_side = edge_x * (end.y - line_p1.y) - edge_y * (end.x - line_p1.x);

			const bool start_inside = start_side * corner_side <= 0;
			const bool end_inside = end_side * corner_side <= 0;

			if (start_inside && end_inside)
			{
				result.push_back(end);
				continue;
			}

			if (start_inside == end_inside)
			{
				continue;
			}

			const T denominator = start_side - end_side;
			const T t = (T)0 == denominator
				? (T)0
				: start_side / denominator;

			result.emplace_back(
				start.x + (end.x - start.x) * t,
				start.y + (end.y - start.y) * t
			);

			if (end_inside)
			{
				result.push_back(end);
			}
		}

		return result;
	}
}
