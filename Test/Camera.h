#pragma once

#include "RTWeekEnd.h"
#include "Hittable.h"
#include "Material.h"

class Camera {
public:
	double aspectRatio = 1.0;
	int imageWidth = 100;
	int samplesPerPixel = 100;
	int maxDepth = 50;
	double vFov = 90.0;
	Point3 lookFrom = Point3(0, 0, 0);
	Point3 lookAt = Point3(0, 0, -1);
	Vector3 vUp = Vector3(0, 1, 0);

	double defocusAngle = 0;
	double focusDistance = 10;

	void Render(const Hittable& world)
	{
		Initialize();
		// Render
		std::ofstream file("image.ppm");
		file << "P3\r\n" << imageWidth << ' ' << imageHeight << "\r\n255\r\n";
		for (int j = 0; j < imageHeight; j++)
		{
			std::clog << "\r rScanlines remaining: " << (imageHeight - j) << ' ' << std::flush;
			for (int i = 0; i < imageWidth; i++)
			{
				Color pixelColor(0, 0, 0);
				for (int sample = 0; sample < samplesPerPixel; sample++)
				{
					Ray r = GetRay(i, j);
					pixelColor += RayColor(r, maxDepth,world);
				}
				WriteColor(file, pixelSamplesScale * pixelColor);
			}
		}
		std::clog << "\rDone.                    \n";

		file.close();
	}

private:
	int imageHeight;
	double pixelSamplesScale;
	Point3 center;
	Point3 pixel100Loc;
	Vector3 pixelDeltaU;
	Vector3 pixelDeltaV;
	Vector3 u, v, w;
	Vector3 defocusDiskU;
	Vector3 defocusDiskV;

	void Initialize() {
		imageHeight = std::max(1, int(imageWidth / aspectRatio));

		pixelSamplesScale = 1.0 / samplesPerPixel;

		center = lookFrom;

		auto focalLength = (lookFrom - lookAt).Length();
		auto theta = DegreesToRadians(vFov);
		auto h = std::tan(theta / 2);
		auto viewportHeight = 2.0 * h * focalLength;
		auto viewportWidth = viewportHeight * (double(imageWidth) / imageHeight);

		w = UnitVector(lookFrom - lookAt);
		u = UnitVector(Cross(vUp, w));
		v = Cross(w, u);

		auto viewportU = viewportWidth * u;
		auto viewportV = viewportHeight * -v;

		pixelDeltaU = viewportU / imageWidth;
		pixelDeltaV = viewportV / imageHeight;

		auto viewportUpperLeft = center - (focalLength * w) - viewportU / 2 - viewportV / 2;
		pixel100Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

		auto defocusRadius = focusDistance * std::tan(DegreesToRadians(defocusAngle / 2));
		defocusDiskU = u * defocusRadius;
		defocusDiskV = v * defocusRadius;
	}

	Ray GetRay(int i, int j) const {
		auto offset = SampleSquare();
		auto pixelSample = pixel100Loc
			+ ((i + offset.x()) * pixelDeltaU)
			+ ((j + offset.y()) * pixelDeltaV);
		auto rayOrigin = (defocusAngle <= 0) ? center : DefocusDiskSample();
		auto rayDirection = pixelSample - rayOrigin;

		return Ray(rayOrigin, rayDirection);
	}
	
	Vector3 DefocusDiskSample() const {
		auto p = RandomInUnitDisk();
		return center + p.x() * defocusDiskU + p.y() * defocusDiskV;
	}
	Vector3 SampleSquare() const {
		return Vector3(RandomDoubleNew() - 0.5, RandomDoubleNew() - 0.5, 0);
	}

	Color RayColor(const Ray& r, int depth, const Hittable& world) const
	{
		if (depth <= 0)
			return Color(0, 0, 0);

		HitRecord rec;
		if (world.Hit(r, Interval(0.001, Infinity), rec))
		{
			Ray scattered;
			Color attenuation;
			if (rec.mat->Scatter(r, rec, attenuation, scattered))
			{
				return attenuation * RayColor(scattered, depth - 1, world);
			}
			return Color(0, 0, 0);
		}

		Vector3 unitDirection = UnitVector(r.direction());
		auto a = 0.5 * (unitDirection.y() + 1.0);
		return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
	}
};
