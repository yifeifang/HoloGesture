#pragma comment(lib, "k4a.lib")
#include <k4a/k4a.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>

int main()
{
    uint32_t count = k4a_device_get_installed_count();
    if (count == 0)
    {
        printf("No k4a devices attached!\n");
        return 1;
    }

    // Open the first plugged in Kinect device
    k4a_device_t device = NULL;
    if (K4A_FAILED(k4a_device_open(K4A_DEVICE_DEFAULT, &device)))
    {
        printf("Failed to open k4a device!\n");
        return 1;
    }

    // Get the size of the serial number
    size_t serial_size = 0;
    k4a_device_get_serialnum(device, NULL, &serial_size);

    // Allocate memory for the serial, then acquire it
    char* serial = (char*)(malloc(serial_size));
    k4a_device_get_serialnum(device, serial, &serial_size);
    printf("Opened device: %s\n", serial);
    free(serial);

    // Configure a stream of 4096x3072 BRGA color data at 15 frames per second
    k4a_device_configuration_t config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
    config.camera_fps = K4A_FRAMES_PER_SECOND_15;
    config.color_format = K4A_IMAGE_FORMAT_COLOR_MJPG;
    config.color_resolution = K4A_COLOR_RESOLUTION_2160P;
    config.depth_mode = K4A_DEPTH_MODE_WFOV_2X2BINNED;


    // Start the camera with the given configuration
    if (K4A_FAILED(k4a_device_start_cameras(device, &config)))
    {
        printf("Failed to start cameras!\n");
        k4a_device_close(device);
        return 1;
    }

    // Camera capture and application specific code would go here

    k4a_capture_t capture = NULL;

    switch (k4a_device_get_capture(device, &capture, 10000))
    {
    case K4A_WAIT_RESULT_SUCCEEDED:
        break;
    case K4A_WAIT_RESULT_TIMEOUT:
        printf("Timed out waiting for a capture\n");
        break;
    case K4A_WAIT_RESULT_FAILED:
        printf("Failed to read a capture\n");
        break;
    }

    //k4a_image_t colorImage = k4a_capture_get_color_image(capture); // get image metadata
    //if (colorImage != NULL)
    //{
    //    // you can check the format with this function
    //    k4a_image_format_t format = k4a_image_get_format(colorImage); // K4A_IMAGE_FORMAT_COLOR_BGRA32 

    //    // get raw buffer
    //    uint8_t* buffer = k4a_image_get_buffer(colorImage);

    //    // convert the raw buffer to cv::Mat
    //    int rows = k4a_image_get_height_pixels(colorImage);
    //    int cols = k4a_image_get_width_pixels(colorImage);
    //    cv::Mat colorMat(rows, cols, CV_8UC4, (void*)buffer, cv::Mat::AUTO_STEP);
    //    cv::namedWindow("test", cv::WINDOW_NORMAL);
    //    cv::imshow("test", colorMat);
    //    cv::resizeWindow("test", 1280, 720);
    //    cv::waitKey(0);
    //    // ...

    //    k4a_image_release(colorImage);
    //}

    k4a_image_t image = k4a_capture_get_depth_image(capture);
    if (image != NULL)
    {
        printf(" | Depth16 res:%4dx%4d stride:%5d\n",
            k4a_image_get_height_pixels(image),
            k4a_image_get_width_pixels(image),
            k4a_image_get_stride_bytes(image));

        uint16_t* imagebuffer = (uint16_t *)(void *)k4a_image_get_buffer(image);
        k4a_image_format_t myformat = k4a_image_get_format(image);
        // y * width (first pixel on that row) + x offset
        printf("image pixel 0,0 has depth %d \n", imagebuffer[256 * 512 + 256]);
        // Release the image
        k4a_image_release(image);
    }

    // Release the capture
    k4a_capture_release(capture);

    // Shut down the camera when finished with application logic

    k4a_device_stop_cameras(device);
    k4a_device_close(device);

    //keybd_event(0x12, 0, 0, 0);
    //keybd_event(0x09, 0, 0, 0);
    //keybd_event(0x12, 0, KEYEVENTF_KEYUP, 0);
    //keybd_event(0x09, 0, KEYEVENTF_KEYUP, 0);

    return 0;
}
