#pragma once

class Interval
{
public:
	double min, max;

	Interval() : min(+Infinity), max(-Infinity) {}

	Interval(double min, double max) : min(min), max(max) {}

	double Size() const {
		return max - min;
	}

	bool Contains(double x) const {
		return min <= x && x <= max;
	}

	bool Surrounds(double x) const {
		return min < x && x < max;
	}

	double Clamp(double x) const {
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}

	static const Interval Empty, Universe;
};

const Interval Interval::Empty = Interval(+Infinity, -Infinity);
const Interval Interval::Universe = Interval(-Infinity, +Infinity);
