#pragma once

#include <iostream>
#include <string>
#include "MathUtils.h"

#include "color.h"
#include "camera.h"
#include "ray.h"
#include "hittable_list.h"

#include "IETThread.h"
#include "RTImage.h"

class RayTraceThread : public IETThread {
public:
	int ID = 0;

	int Start = 0;
	int End = 0;

	int imageWidth = 0;
	int imageHeight = 0;
	int raySamplesPerPixel = 0;
	int bounces = 0;

	std::string data = "";
	bool isRunning = false;

	hittable_list world;
	camera* cam; 
	RTImage* rtImage;

	RayTraceThread(int id, int start, int end) {
		this->ID = id;
		this->Start = start;
		this->End = end;
	}

	void run() {
		this->isRunning = true;

		//std::cout << "P3\n" << this->imageWidth << ' ' << this->imageHeight << "\n255\n";

		for (int j = End - 1; j >= Start; j--) {
			std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
			for (int i = 0; i < imageWidth; i++) {
				color pixel_color(0, 0, 0);
				for (int s = 0; s < raySamplesPerPixel; ++s) {
					auto u = (i + random_double()) / (imageWidth - 1);
					auto v = (j + random_double()) / (imageHeight - 1);
					ray r = cam->get_ray(u, v);
					pixel_color += ray_color(r, bounces, world);
				}
				//write_color(std::cout, pixel_color, samples_per_pixel);
				writeColor(pixel_color, raySamplesPerPixel);
				// Same function except it appends all computed data into-
				// renderData
			}
		}
		
		this->isRunning = false;
		/*
		for (int j = 0; j < this->imageHeight; j++) {
			std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush; 
			for (int i = 0; i < this->imageWidth; i++) {
				color pixel_color(0, 0, 0);
				for (int sample = 0; sample < raySamplesPerPixel; sample++) {
					//ray r = this->cam->get_ray(i, j);
					auto u = (i + random_double()) / (this->imageWidth - 1);
					auto v = (j + random_double()) / (this->imageHeight - 1);
					ray r = cam->get_ray(u, v);
					pixel_color += ray_color(r, bounces, world);
				}
				write_color(std::cout, (1.0f / raySamplesPerPixel) * pixel_color);
			}
		}
		*/

	}

	color ray_color(const ray& r, int bounces, const hittable& world) const {
        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (bounces <= 0)
            return color(0, 0, 0);

        hit_record rec;

        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, bounces - 1, world);
            return color(0, 0, 0);
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }

	void writeColor(color pixel, int samplesPerPixel) {
		auto r = pixel.x();
		auto g = pixel.y();
		auto b = pixel.z();

		auto scale = 1.0 / samplesPerPixel;
		r = sqrt(scale * r);
		g = sqrt(scale * g);
		b = sqrt(scale * b);

		// Translate the [0,1] component values to the byte range [0,255].
		static const interval intensity(0.000, 0.999);
		int rbyte = int(256 * intensity.clamp(r));
		int gbyte = int(256 * intensity.clamp(g));
		int bbyte = int(256 * intensity.clamp(b));

		data.append(std::to_string(rbyte) + ' ' + std::to_string(gbyte) + ' ' + std::to_string(bbyte) + '\n');
	}

public:
	void setAttributes(camera* cam, hittable_list world, int bounces, int raySamplesPerPixel, int imageWidth, int imageHeight) {
		this->cam = cam;
		this->world = world;
		this->bounces = bounces;
		this->raySamplesPerPixel = raySamplesPerPixel;
		this->imageWidth = imageWidth;
		this->imageHeight = imageHeight;
	}

	void setImage(RTImage* rtImage) {
		this->rtImage = rtImage;
	}
};