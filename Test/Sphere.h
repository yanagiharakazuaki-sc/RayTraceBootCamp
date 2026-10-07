#pragma once

#include "Hittable.h"

class Sphere : public Hittable
{
public:
	Sphere(const Point3& center, double radius, shared_ptr<Material> mat) : center(center), radius(std::fmax(0, radius)), mat(mat)
	{
	}

	bool Hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
		Vector3 oc = center - r.origin();
		auto a = r.direction().LengthSquared();
		auto h = Dot(r.direction(), oc);
		auto c = oc.LengthSquared() - radius * radius;

		auto discriminant = h * h - a * c;
		if (discriminant < 0)
		{
			return false;
		}

		auto sqrtd = std::sqrt(discriminant);

		auto root = (h - sqrtd) / a;
		if (!ray_t.Surrounds(root))
		{
			root = (h + sqrtd) / a;
			if (!ray_t.Surrounds(root))
			{
				return false;
			}
		}

		rec.t = root;
		rec.p = r.At(rec.t);
		rec.normal = (rec.p - center) / radius;
		Vector3 outwardNormal = (rec.p - center) / radius;
		rec.SetFaceNormal(r, outwardNormal);
		rec.mat = mat;

		return true;
	}


private:
	Point3 center;
	double radius;
	shared_ptr<Material> mat;
};
