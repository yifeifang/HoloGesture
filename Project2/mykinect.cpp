#include "mykinect.h"
#include "utils.h"

mykinect::mykinect(k4a_device_configuration_t deviceConfig, k4abt_tracker_configuration_t tracker_config) : _deviceConfig(deviceConfig), _tracker_config(tracker_config)
{
    VERIFY_k4a_result_t(k4a_device_open(0, &_device));
    VERIFY_k4a_result_t(k4a_device_get_calibration(_device, _deviceConfig.depth_mode, _deviceConfig.color_resolution, &_sensor_calibration));
    VERIFY_k4a_result_t(k4abt_tracker_create(&_sensor_calibration, _tracker_config, &_tracker));
}

mykinect::~mykinect()
{
    k4abt_tracker_shutdown(_tracker);
    k4abt_tracker_destroy(_tracker);
    k4a_device_close(_device);
}

void mykinect::start_cameras()
{
    VERIFY_k4a_result_t(k4a_device_start_cameras(_device, &_deviceConfig));
}

void mykinect::stop_cameras()
{
    k4a_device_stop_cameras(_device);
}

k4a_wait_result_t mykinect::get_capture(int32_t timeout_in_ms)
{
    return k4a_device_get_capture(_device, &_sensor_capture, timeout_in_ms);
}

void mykinect::release_capture()
{
    k4a_capture_release(_sensor_capture);
}

k4a_wait_result_t mykinect::enqueue_capture(int32_t timeout_in_ms)
{
    return k4abt_tracker_enqueue_capture(_tracker, _sensor_capture, timeout_in_ms);
}

void mykinect::release_body_frame()
{
    k4abt_frame_release(_body_frame);
}

k4a_wait_result_t mykinect::pop_frame_result(int32_t timeout_in_ms)
{
    return k4abt_tracker_pop_result(_tracker, &_body_frame, timeout_in_ms);
}


