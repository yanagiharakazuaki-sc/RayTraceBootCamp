#pragma once

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <fstream>
#include <random>

// C++ Std Usings
using std::make_shared;
using std::shared_ptr;

// Constants
const double Infinity = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

// UtilityFunctions
inline double DegreesToRadians(double degrees)
{
	return degrees * PI / 180.0;
}

inline double RandomDouble()
{
	return std::rand() / (RAND_MAX + 1.0);
}

inline double RandomDouble(double min, double max)
{
	return min + (max - min) * RandomDouble();
}

inline double RandomDoubleNew()
{
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

// CommonHeaders
#include "Color.h"
#include "Interval.h"
#include "Ray.h"
#include "Vector3.h"
