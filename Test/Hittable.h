#pragma once

class Material;

class HitRecord {
public:
	Point3 p;
	Vector3 normal;
	double t;
	bool frontFace;
	shared_ptr<Material> mat;
	
	void SetFaceNormal(const Ray& r, const Vector3& outwardNormal)
	{
		frontFace = Dot(r.direction(), outwardNormal) < 0;
		normal = frontFace ? outwardNormal : -outwardNormal;
	}
};

class Hittable {
public:
	virtual ~Hittable() = default;
	virtual bool Hit(const Ray& r, Interval ray_t, HitRecord& rec) const = 0;
};
