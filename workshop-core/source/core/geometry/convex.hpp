#pragma once

#include "math.hpp"

#include <optional>
#include <tuple>

namespace wk::Geometry
{
	enum PolygonType
	{
		Convex,
		Concave,
		Complex
	};

	template<typename T>
	PolygonType get_polygon_type(const std::vector<Point_t<T>>& points)
	{
		if (points.size() < 3)
			throw Exception("Polygon must have at least 3 points");

		bool sign = false;
		bool first = true;
		for (size_t i = 0; points.size() > i; i++)
		{
			Point_t<T> p1 = points[i];
			Point_t<T> p2 = points[(i + 1) % points.size()];
			Point_t<T> p3 = points[(i + 2) % points.size()];
			float cross = cross_product(p1, p2, p3);

			if (first)
			{
				sign = cross >= 0;
				first = false;
			}
			else if (sign != cross > 0)
			{
				return PolygonType::Complex;
			}
		}

		return sign ? PolygonType::Convex : PolygonType::Concave;
	}

	namespace Hull
	{
		template<typename T>
		void quickHull(const std::vector<Point_t<T>>& v, const Point_t<T>& a, const Point_t<T>& b, std::vector<Point_t<T>>& hull)
		{
			if (v.empty()) {
				return;
			}

			auto f = v[get_farthest_point(Line_t<T>(a, b), v)];

			// Collect points to the left of segment (a, f)
			std::vector<Point_t<T>> left;
			for (auto p : v) {
				if (cross_product(a, f, p) > 0) {
					left.push_back(p);
				}
			}
			quickHull(left, a, f, hull);

			// Add f to the hull
			hull.push_back(f);

			// Collect points to the left of segment (f, b)
			std::vector<Point_t<T>> right;
			for (auto p : v) {
				if (cross_product(f, b, p) > 0) {
					right.push_back(p);
				}
			}
			quickHull(right, f, b, hull);
		}

		template<typename T>
		std::vector<Point_t<T>> quick_hull(std::vector<Point_t<T>> points)
		{
			std::vector<Point_t<T>> result;
			if (points.size() < 3) return result;

			// Find the leftmost and rightmost points
			size_t min = 0;
			size_t max = 0;
			for (size_t i = 1; points.size() > i; i++)
			{
				if (points[i].x < points[min].x)
				{
					min = i;
				}
				if (points[i].x > points[max].x)
				{
					max = i;
				}
			}

			// Split the points into two parts
			std::vector<Point_t<T>> left;
			std::vector<Point_t<T>> right;
			for (size_t i = 0; points.size() > i; i++)
			{
				if (cross_product(points[min], points[max], points[i]) > 0)
				{
					left.push_back(points[i]);
				}
				else if (cross_product(points[min], points[max], points[i]) < 0)
				{
					right.push_back(points[i]);
				}
			}

			// Find the hull
			result.push_back(points[min]);
			quickHull(left, points[min], points[max], result);

			result.push_back(points[max]);
			quickHull(right, points[max], points[min], result);

			return result;
		}
	}
}