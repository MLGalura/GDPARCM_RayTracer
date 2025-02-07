#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

#include "ThreadPool.h"
#include "opencv2/opencv.hpp"

int main() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));
     
    int ballcount = 0;

    for (int a = -10; a < 7; a++) { 
        for (int b = -3; b < 3; b++) { 
            auto choose_mat = random_double(); 
            point3 center(a + 0.3 * random_double(), 0.2, b + 0.3 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.4) { 
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

                ballcount++;
                std::cout << "ball count: " << ballcount << std::endl;
            }
        }
    }
    

    auto material1 = make_shared<dielectric>(1.00 / 1.50);
    auto material2 = make_shared<lambertian>(color(0.1, 0.4, 0.1));
    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    auto material4 = make_shared<lambertian>(color(0.8, 0.1, 0.1));

    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
    world.add(make_shared<sphere>(point3(-8, 1, 0), 1.0, material4));



    camera cam;

    cam.aspect_ratio = 1; //16.0 / 9.0;
    cam.image_width = 1080;
    cam.samples_per_pixel = 200;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle = 1.6;
    cam.focus_dist = 4.0;


    ThreadPool threadPool("RenderPool", std::thread::hardware_concurrency());
    threadPool.startScheduler();

    cam.render(world, &threadPool);

    threadPool.stopScheduler();
    return 0;

    /* TESTER TO CHECK OPENCV
    int width = 256, height = 256;

    // Create an OpenCV Mat filled with a gradient
    cv::Mat image(height, width, CV_8UC3);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            image.at<cv::Vec3b>(y, x) = cv::Vec3b(x % 256, y % 256, 128);
        }
    }

    // Save as a PPM file
    std::ofstream outFile("test_output.ppm");
    if (!outFile) {
        std::cerr << "Error: Could not create the file!" << std::endl;
        return -1;
    }

    // Write PPM header
    outFile << "P3\n" << width << " " << height << "\n255\n";

    // Write pixel data
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            cv::Vec3b color = image.at<cv::Vec3b>(y, x);
            outFile << (int)color[2] << " " << (int)color[1] << " " << (int)color[0] << "\n";
        }
    }

    outFile.close();
    std::cout << "PPM file created: test_output.ppm" << std::endl;

    return 0;
    */
}