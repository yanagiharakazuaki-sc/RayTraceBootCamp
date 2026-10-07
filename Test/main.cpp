#include "RTWeekEnd.h"

#include "Camera.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Material.h"
#include "Sphere.h"



int main()
{
	// World
	HittableList world;

	auto groundMaterial = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
	world.Add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, groundMaterial));

	for(int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			auto chooseMat = RandomDoubleNew();
			Point3 center(a + 0.9 * RandomDoubleNew(), 0.2, b + 0.9 * RandomDoubleNew());
			if ((center - Point3(4, 0.2, 0)).Length() > 0.9)
			{
				shared_ptr<Material> sphereMaterial;
				if (chooseMat < 0.8)
				{
					// diffuse
					auto albedo = Color::Random() * Color::Random();
					sphereMaterial = make_shared<Lambertian>(albedo);
					world.Add(make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
				else if (chooseMat < 0.95)
				{
					// metal
					auto albedo = Color::Random(0.5, 1);
					auto fuzz = RandomDouble(0, 0.5);
					sphereMaterial = make_shared<Metal>(albedo, fuzz);
					world.Add(make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
				else
				{
					// glass
					sphereMaterial = make_shared<Dielectric>(1.5);
					world.Add(make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
			}
		}
	}

	auto material1 = make_shared<Dielectric>(1.5);
	world.Add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
	world.Add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));


	auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
	world.Add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));
/*
	auto materialGround = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
	auto materialCenter = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
	auto materialMetal  = make_shared<Metal>(Color(0.8, 0.6, 0.2),1.0);
	auto materialGlass = make_shared<Dielectric>(1.5);
	auto materialBubble = make_shared<Dielectric>(1.00/ 1.33);
	world.Add(make_shared<Sphere>(Point3(0, -100.5, -1), 100.0, materialGround));
	world.Add(make_shared<Sphere>(Point3(0, 0, -1.2), 0.5, materialCenter));
	world.Add(make_shared<Sphere>(Point3(-1, 0, -1), 0.5, materialGlass));
	world.Add(make_shared<Sphere>(Point3(-1, 0, -1), 0.4, materialBubble));
	world.Add(make_shared<Sphere>(Point3(1, 0, -1), 0.5, materialMetal));
	*/
	Camera cam;

	cam.aspectRatio = 16.0 / 9.0;
	cam.imageWidth = 1200;
	cam.samplesPerPixel = 10;
	cam.maxDepth = 50;

	cam.vFov = 20.0;
	cam.lookFrom = Point3(13, 2, 3);
	cam.lookAt = Point3(0, 0, 0);
	cam.vUp = Vector3(0, 1, 0);

	cam.defocusAngle = 0.6;
	cam.focusDistance = 10.0;

	cam.Render(world);
}
