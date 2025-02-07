#include "ScanlineRenderer.h"
#include "IExecutionEvent.h"
#include "camera.h"

ScanlineRenderer::ScanlineRenderer(const camera& cam, int j, int width, int samples,
								   const hittable& world, cv::Mat& buffer, IExecutionEvent* execEvent) : cam(cam), world(world), buffer(buffer)
{
	this->j = j;
	this->imageWidth = width;
	this->samplesPerPixel = samples;
	this->pixelSamplesScale = 1.0 / samples;
	this->execEvent = execEvent;
}

ScanlineRenderer::~ScanlineRenderer()
{
	//std::cout << "Destroying scan line renderer. " << std::endl;
}

void ScanlineRenderer::onStartTask()
{
	//IETThread::sleep(300);

	for (int i = 0; i < this->imageWidth; i++) {

		color pixelColor(0.0f, 0.0f, 0.0f);

		for (int sample = 0; sample < this->samplesPerPixel; sample++) {
			ray r = cam.get_ray(i, j); 
			pixelColor += cam.ray_color(r, cam.max_depth, world);
		}

		buffer.at<cv::Vec3f>(j, i) = cv::Vec3f(pixelColor.x(), pixelColor.y(), pixelColor.z()) * pixelSamplesScale;
	}

	this->execEvent->onFinishedExecution();
	std::cout << "Scanline " << j << " completed." << std::endl;
	delete this;
}