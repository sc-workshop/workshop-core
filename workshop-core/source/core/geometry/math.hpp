#pragma once

#include "core/math/point.h"
#include "core/math/line.h"
#include "core/math/triangle.h"

#include <vector>
#include <cmath>

namespace wk::Geometry
{
	/// <summary>
	/// Z Cross product of two vectors.
	/// </summary>
	/// <typeparam name="T1"></typeparam>
	/// <typeparam name="T2"></typeparam>
	/// <typeparam name="T3"></typeparam>
	/// <param name="p1"></param>
	/// <param name="p2"></param>
	/// <param name="p3"></param>
	/// <returns></returns>
	template<typename T1, typename T2, typename T3>
	float cross_product(const Point_t<T1>& p1, const Point_t<T2>& p2, const Point_t<T3>& p3)
	{
		return (float)(
			(p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x)
		);
	}

	/// <summary>
	/// Calculates the Euclidean distance between two points.
	/// </summary>
	/// <typeparam name="T1"></typeparam>
	/// <typeparam name="T2"></typeparam>
	/// <param name="p1"></param>
	/// <param name="p2"></param>
	/// <returns>Distance between two points</returns>
	template<typename T1, typename T2>
	float dist(const Point_t<T1>& p1, const Point_t<T2>& p2) {
		return (float)std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
	}

	template<typename T1, typename T2>
	float line_length(const Point_t<T1>& a, const Point_t<T2>& b) {
		return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
	}

	template<typename T1, typename T2, typename T3>
	float point_to_line_distance(const Point_t<T1>& a, const Point_t<T2>& b, const Point_t<T3>& p) {
		return (float)std::fabs((b.x - a.x) * (a.y - p.y) - (b.y - a.y) * (a.x - p.x)) / line_length(a, b);
	}

	/// <summary>
	/// Calculates the angle of a line.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="line"></param>
	/// <returns>Angle in radians</returns>
	template<typename T>
	float line_angle(const Line_t<T>& line)
	{
		return (float)std::atan2(line.end.y - line.start.y, line.end.x - line.start.x);
	}

	/// <summary>
	/// Builds triangle by given bisector, angle and length.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="bisector"></param>
	/// <param name="angle">Angle in radians</param>
	/// <param name="length">Triangle length</param>
	/// <returns></returns>
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

	/// <summary>
	/// Calculates if point is inside polygon.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="polygon"></param>
	/// <param name="point"></param>
	/// <returns>True if point is inside polygon</returns>
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

	template<typename T>
	size_t get_farthest_point(const Line_t<T> line, const std::vector<Point_t<T>>& v) {
		size_t idxMax = 0;
		float distMax = point_to_line_distance(line.start, line.end, v[idxMax]);

		for (size_t i = 1; i < v.size(); ++i) {
			float distCurr = point_to_line_distance(line.start, line.end, v[i]);
			if (distCurr > distMax) {
				idxMax = i;
				distMax = distCurr;
			}
		}

		return idxMax;
	}
}