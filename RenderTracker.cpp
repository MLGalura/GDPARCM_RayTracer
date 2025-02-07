#include "RenderTracker.h"

RenderTracker::RenderTracker(int total_tasks, cv::Mat& buffer, int width, int height, int samples) :
    remaining_tasks(total_tasks), image_buffer(buffer), image_width(width),
    image_height(height), pixel_samples_scale(1.0 / samples) {}

void RenderTracker::onFinishedExecution()
{
    std::lock_guard<std::mutex> lock(mtx);
    remaining_tasks--;

    // If all tasks are done, save the image
    if (remaining_tasks == 0) {
        
        cv::Mat output_image;
        image_buffer.convertTo(output_image, CV_8UC3, 255.0); 
        cv::cvtColor(output_image, output_image, cv::COLOR_RGB2BGR); 

        // Save the image
        cv::imwrite("image.png", output_image);
        std::clog << "\rImage saved as image.png.                 \n";
    } 
}

bool RenderTracker::isDone() const
{
    std::lock_guard<std::mutex> lock(mtx); // idk why this doesn't work
    return remaining_tasks == 0;
}
