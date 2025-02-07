#pragma once

#include <opencv2/opencv.hpp>
#include <mutex>

#include "IExecutionEvent.h"

class RenderTracker : public IExecutionEvent
{
private:
    int remaining_tasks; 
    cv::Mat& image_buffer; 
    int image_width;
    int image_height;
    double pixel_samples_scale;
    mutable std::mutex mtx;

public:
    RenderTracker(int total_tasks, cv::Mat& buffer, int width, int height, int samples);
    void onFinishedExecution() override;

    bool isDone() const;
};

