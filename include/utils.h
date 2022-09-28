#pragma once

namespace utils
{
	template<typename T>
	T clamp(T value, T min, T max)
	{
		T res = value < min ? min : value;
		return res > max ? max : res;
	}

	template<typename T>
	T lerp(T a, T b, T t)
	{
		return a + t * (b - a);
	}

	template<typename T>
	T getMax(T a, T b)
	{
		return a < b ? b : a;
	}
	template<typename T>
	T getMin(T a, T b)
	{
		return a < b ? a : b;
	}

	inline float sign(float a)
	{
		return a < 0.f ? -1.f : 1.f;
	}
	inline double sign(double a)
	{
		return a < 0 ? -1 : 1;
	}
};