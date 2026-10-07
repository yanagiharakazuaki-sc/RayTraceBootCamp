#pragma once

#include "Hittable.h"

class Material {
public:
	virtual ~Material() = default;

	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const 
	{
		return false;
	}
};


class Lambertian : public Material
{
public:
	Lambertian(const Color& albedo) : albedo(albedo) {}

	bool Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered)
		const override {
		auto scatterDirection = rec.normal + RandomUnitVector();

		// 法線とランダムベクトルが反対を向いている場合、scatterDirectionがゼロベクトルになる可能性があるので、その場合は法線を使用する
		if (scatterDirection.NearZero())
			scatterDirection = rec.normal;

		scattered = Ray(rec.p, scatterDirection);
		attenuation = albedo;
		
		return true;
	}

private:
	Color albedo;
};

class Metal : public Material
{
public:
	Metal(const Color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

	bool Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
	{
		auto reflected = Reflect(r_in.direction(), rec.normal);
		reflected = UnitVector(reflected) + (fuzz * RandomUnitVector());
		scattered = Ray(rec.p, reflected);
		attenuation = albedo;

		return (Dot(scattered.direction(), rec.normal) > 0);
	}

private:
	Color albedo;
	double fuzz;
};

class Dielectric : public Material
{
public:
	Dielectric(double refractionIndex) : refractionIndex(refractionIndex) {}

	bool Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
	{
		attenuation = Color(1.0, 1.0, 1.0);
		double ri = rec.frontFace ? (1.0 / refractionIndex) : refractionIndex;

		Vector3 unitDirection = UnitVector(r_in.direction());
		double cosTheta = std::fmin(Dot(-unitDirection, rec.normal), 1.0);
		double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

		bool cannotRefract = ri * sinTheta > 1.0;
		Vector3 direction;

		if (cannotRefract ||Reflectance(cosTheta,ri) > RandomDouble())
			direction = Reflect(unitDirection, rec.normal);
		else
			direction = Refract(unitDirection, rec.normal, ri);

		scattered = Ray(rec.p, direction);
		return true;
	}

private:
	double refractionIndex;

	static double Reflectance(double cosine, double refractionIndex)
	{
		auto r0 = (1 - refractionIndex) / (1 + refractionIndex);
		r0 = r0 * r0;
		return r0 + (1 - r0) * std::pow((1 - cosine), 5);
	}
};
