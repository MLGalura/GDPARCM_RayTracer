#include "rtweekend.h"
#include <vector>

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

#include "RayTraceThread.h"
#include "RTImage.h"

#include "opencv2/opencv.hpp"
#include "Windows.h"

void multiCoreWindowRT(hittable_list world, camera cam, int imageWidth, int raySamplesPerPixel, int bounces, int numCores)
{
    const float aspectRatio = 3.0 / 2.0f;
    const int imageHeight = static_cast<int>(imageWidth, aspectRatio);

    std::vector<RayTraceThread*> rtList;

    const int widthWindow = rint(imageWidth / numCores);
    const int heightWindow = imageHeight;

    cam.initialize();

    // Number of Cores
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    const int logicalCores = sysInfo.dwNumberOfProcessors;
    const int nThreads = logicalCores * 2;

    const int rowRatio = imageHeight / nThreads + 1;

    for (int i = 0; i < numCores; i++) {
        RayTraceThread* rtCompute = new RayTraceThread(i, i * rowRatio, (i + 1) * rowRatio);
        rtCompute->setAttributes(&cam, world, bounces, raySamplesPerPixel, imageWidth, imageHeight);

        rtList.push_back(rtCompute);
        rtCompute->start();
    }


    for (int i = 0; i < rtList.size(); i++) {
        if (rtList[i]->isRunning) {
            i = 0;
        }
    }

    std::string renderData = "";
    for (int i = rtList.size() - 1; i >= 0; i--) {
        renderData.append(rtList[i]->data);
    }

    std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";
    std::cout << renderData;

    cv::Mat ppmImage = cv::imread("C:/Users/Marc Galura/source/repos/GDPARCM_RayTracer/x64/Debug/image.ppm", cv::IMREAD_UNCHANGED);

    if (ppmImage.empty())
        std::cerr << "Can't open or find the image." << std::endl;
    
    else {
        cv::imwrite("finalImage.png", ppmImage);
        cv::imshow("Sample", ppmImage);
    }
}

int main() {
    const float aspectRatio = 3.0f / 2.0f;
    const int imgWidth = 1440;
    const int imgHeight = static_cast<int>(imgWidth / aspectRatio);
    const int raySamplesPerPixel = 50;
    const int bounces = 25;
    const int numCores = 24;

    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));
     
    for (int a = -11; a < 11; a++) { 
        for (int b = -11; b < 11; b++) { 
            auto choose_mat = random_double(); 
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double()); 

            if ((center - point3(4, 0.2, 0)).length() > 0.9) { 
                shared_ptr<material> sphere_material; 

                if (choose_mat < 0.8) { 
                    // diffuse
                    auto albedo = color::random() * color::random(); 
                    sphere_material = make_shared<lambertian>(albedo); 
                    world.add(make_shared<sphere>(center, 0.2, sphere_material)); 
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1); 
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material)); 
                }
                else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5); 
                    world.add(make_shared<sphere>(center, 0.2, sphere_material)); 
                }
            }
        }
    }
    

    auto material1 = make_shared<dielectric>(1.00 / 1.50);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.1, 0.4, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    auto material4 = make_shared<lambertian>(color(0.8, 0.1, 0.1));
    world.add(make_shared<sphere>(point3(-8, 1, 0), 1.0, material4));

    camera cam;

    cam.aspect_ratio = 1; //16.0 / 9.0;
    cam.image_width = 512;
    cam.samples_per_pixel = 500;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;

    multiCoreWindowRT(world, cam, imgWidth, raySamplesPerPixel, bounces, numCores);
    //cam.render(world);
}