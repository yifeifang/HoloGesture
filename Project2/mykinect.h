#pragma once

#include <k4a/k4a.h>
#include <k4abt.h>                                                                                                

class mykinect
{
    private:
        k4a_device_t _device;
        k4a_device_configuration_t _deviceConfig;
        k4a_calibration_t _sensor_calibration;
        k4abt_tracker_t _tracker;
        k4abt_tracker_configuration_t _tracker_config;
        k4a_capture_t _sensor_capture;
        k4abt_frame_t _body_frame;
    public:
        k4abt_skeleton_t _skeleton;
        mykinect(k4a_device_configuration_t deviceConfig, k4abt_tracker_configuration_t tracker_config);
        ~mykinect();
        void start_cameras();
        void stop_cameras();
        k4a_wait_result_t get_capture(int32_t timeout_in_ms);
        void release_capture();
        k4a_wait_result_t enqueue_capture(int32_t timeout_in_ms);
        k4a_wait_result_t pop_frame_result(int32_t timeout_in_ms);
        bool update_skeleton();
};