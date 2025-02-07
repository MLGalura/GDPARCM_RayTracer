#pragma once
#include "IETThread.h"
#include "IWorkerAction.h"

#include "hittable.h"
#include "opencv2/opencv.hpp"

class camera;
class IExecutionEvent;
class ScanlineRenderer : public IWorkerAction
{
public:
	ScanlineRenderer(const camera& cam, int j, int width, int samples,
		const hittable& world, cv::Mat& buffer, IExecutionEvent* execEvent);
	~ScanlineRenderer();

private:
	void onStartTask() override;

	const camera& cam;
	int j;
	int imageWidth;
	int samplesPerPixel;
	double pixelSamplesScale;
	const hittable& world; 
	cv::Mat& buffer;
	IExecutionEvent* execEvent;
};