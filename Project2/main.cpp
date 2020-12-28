#include <stdio.h>
#include <stdlib.h>

#include <k4a/k4a.h>
#include <k4abt.h>
#include <unordered_map>

#include"gesturetree.h"
#include <windows.h>

#define VERIFY(result, error)                                                                            \
    if(result != K4A_RESULT_SUCCEEDED)                                                                   \
    {                                                                                                    \
        printf("%s \n - (File: %s, Function: %s, Line: %d)\n", error, __FILE__, __FUNCTION__, __LINE__); \
        exit(1);                                                                                         \
    }                                                                                                    \

bool process_gesture(unsigned gesture_id)
{
    switch (gesture_id)
    {
        case 1:
        {
            INPUT ip1 = { 0 };
            ip1.type = INPUT_KEYBOARD;
            ip1.ki.wVk = VK_LWIN;
            ip1.ki.dwFlags = 0;
            SendInput(1, &ip1, sizeof(INPUT));

            INPUT ip2 = { 0 };
            ip2.type = INPUT_KEYBOARD;
            ip2.ki.wVk = 0x44;
            ip2.ki.dwFlags = 0;
            SendInput(1, &ip2, sizeof(INPUT));

            ip1.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &ip1, sizeof(INPUT));

            ip2.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &ip2, sizeof(INPUT));
            break;
        }
        case 2:
        {
            INPUT ip1 = { 0 };
            ip1.type = INPUT_KEYBOARD;
            ip1.ki.wVk = VK_LWIN;
            ip1.ki.dwFlags = 0;
            SendInput(1, &ip1, sizeof(INPUT));

            INPUT ip2 = { 0 };
            ip2.type = INPUT_KEYBOARD;
            ip2.ki.wVk = VK_TAB;
            ip2.ki.dwFlags = 0;
            SendInput(1, &ip2, sizeof(INPUT));

            ip1.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &ip1, sizeof(INPUT));

            ip2.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &ip2, sizeof(INPUT));
            break;
        }
        default:
            break;
    }
    return true;
}

int main()
{
    std::unordered_map<k4abt_joint_id_t, k4abt_joint_t> joint_map;

    positionNode * root = new positionNode(0, false, K4ABT_JOINT_HAND_RIGHT, k4a_float3_t{0,0,0}, 200);
    positionNode * front = new positionNode(1, false, K4ABT_JOINT_HAND_RIGHT, k4a_float3_t{ -250,-80,385 }, 200);
    orientationNode* test = new orientationNode(3, false, K4ABT_JOINT_HAND_RIGHT, k4a_quaternion_t{ 0,1,2,3 }, 200);
    positionNode * back = new positionNode(2, true, K4ABT_JOINT_HAND_RIGHT, k4a_float3_t{ -200,150,800 }, 200);

    back->set_gesture(1);

    front->add_child(test);

    test->add_child(back);

    root->add_child(front);

    gestureTree mytree(root, &joint_map);

    int timeout = 0;

    k4a_device_t device = NULL;
    VERIFY(k4a_device_open(0, &device), "Open K4A Device failed!");

    // Start camera. Make sure depth camera is enabled.
    k4a_device_configuration_t deviceConfig = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
    deviceConfig.depth_mode = K4A_DEPTH_MODE_WFOV_2X2BINNED;
    deviceConfig.color_resolution = K4A_COLOR_RESOLUTION_OFF;
    VERIFY(k4a_device_start_cameras(device, &deviceConfig), "Start K4A cameras failed!");

    k4a_calibration_t sensor_calibration;
    VERIFY(k4a_device_get_calibration(device, deviceConfig.depth_mode, deviceConfig.color_resolution, &sensor_calibration),
        "Get depth camera calibration failed!");

    k4abt_tracker_t tracker = NULL;
    k4abt_tracker_configuration_t tracker_config = K4ABT_TRACKER_CONFIG_DEFAULT;
    VERIFY(k4abt_tracker_create(&sensor_calibration, tracker_config, &tracker), "Body tracker initialization failed!");

    while(true)
    {
        if (GetAsyncKeyState(VK_ESCAPE))
        {
            printf("ESC pressed, terminate\n");
            break;
        }
        k4a_capture_t sensor_capture;
        k4a_wait_result_t get_capture_result = k4a_device_get_capture(device, &sensor_capture, K4A_WAIT_INFINITE);
        if (get_capture_result == K4A_WAIT_RESULT_SUCCEEDED)
        {
            k4a_wait_result_t queue_capture_result = k4abt_tracker_enqueue_capture(tracker, sensor_capture, K4A_WAIT_INFINITE);
            k4a_capture_release(sensor_capture); // Remember to release the sensor capture once you finish using it
            if (queue_capture_result == K4A_WAIT_RESULT_TIMEOUT)
            {
                // It should never hit timeout when K4A_WAIT_INFINITE is set.
                printf("Error! Add capture to tracker process queue timeout!\n");
                break;
            }
            else if (queue_capture_result == K4A_WAIT_RESULT_FAILED)
            {
                printf("Error! Add capture to tracker process queue failed!\n");
                break;
            }

            k4abt_frame_t body_frame = NULL;
            k4a_wait_result_t pop_frame_result = k4abt_tracker_pop_result(tracker, &body_frame, K4A_WAIT_INFINITE);
            if (pop_frame_result == K4A_WAIT_RESULT_SUCCEEDED)
            {
                // Successfully popped the body tracking result. Start your processing

                k4abt_skeleton_t my_skeleton;
                k4abt_frame_get_body_skeleton(body_frame, 0, &my_skeleton);
                if (my_skeleton.joints[K4ABT_JOINT_HAND_RIGHT].confidence_level >= K4ABT_JOINT_CONFIDENCE_MEDIUM)
                {
                    joint_map[K4ABT_JOINT_HAND_RIGHT] = my_skeleton.joints[K4ABT_JOINT_HAND_RIGHT];
                    joint_map[K4ABT_JOINT_HAND_LEFT] = my_skeleton.joints[K4ABT_JOINT_HAND_LEFT];

                    //float readx = my_skeleton.joints[K4ABT_JOINT_HAND_RIGHT].position.xyz.x;
                    //float ready = my_skeleton.joints[K4ABT_JOINT_HAND_RIGHT].position.xyz.y;
                    //float readz = my_skeleton.joints[K4ABT_JOINT_HAND_RIGHT].position.xyz.z;
                    //printf("X = %f, Y = %f, Z = %f\n", readx, ready, readz);
                    //int gesture = mytree.traverse(readx, ready, readz);
                    int gesture = mytree.traverse_map();
                    if (gesture)
                    {
                        process_gesture(gesture);
                        mytree.set_state(root);
                        Sleep(1000);
                    }
                    else if (gesture == -1)     // timeout
                    {
                        mytree.set_state(root);
                    }
                    /*if (state->match(readx, ready, readz, 200))
                    {
                        if (state->leaf)
                        {
                            printf("Detect state ID = %d\n", state->id);
                            process_gesture(state->gesture_id);
                            timeout = 0;
                            state = &root;
                        }
                        else
                        {
                            printf("Detect state ID = %d\n", state->id);
                            state = &(state->children[0]);
                        }
                    }
                    else if(timeout >= 75)
                    {
                        timeout = 0;
                        state = &root;
                        printf("Gesture timeout\n");
                    }
                    else
                    {
                        timeout++;
                    }*/
                    //printf("Right hand distance X: %f,  Y: %f,  Z: %f\n", my_skeleton.joints[K4ABT_JOINT_HAND_RIGHT].position.xyz.x, my_skeleton.joints[K4ABT_JOINT_HAND_RIGHT].position.xyz.y, my_skeleton.joints[K4ABT_JOINT_HAND_RIGHT].position.xyz.z);
                }

                k4abt_frame_release(body_frame); // Remember to release the body frame once you finish using it
            }
            else if (pop_frame_result == K4A_WAIT_RESULT_TIMEOUT)
            {
                //  It should never hit timeout when K4A_WAIT_INFINITE is set.
                printf("Error! Pop body frame result timeout!\n");
                break;
            }
            else
            {
                printf("Pop body frame result failed!\n");
                break;
            }
        }
        else if (get_capture_result == K4A_WAIT_RESULT_TIMEOUT)
        {
            // It should never hit time out when K4A_WAIT_INFINITE is set.
            printf("Error! Get depth frame time out!\n");
            break;
        }
        else
        {
            printf("Get depth capture returned error: %d\n", get_capture_result);
            break;
        }

    } 
    printf("Finished body tracking processing!\n");

    delete root;
    delete front;
    delete back;

    k4abt_tracker_shutdown(tracker);
    k4abt_tracker_destroy(tracker);
    k4a_device_stop_cameras(device);
    k4a_device_close(device);

    return 0;
}