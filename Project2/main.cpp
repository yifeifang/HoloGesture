#include <stdio.h>
#include <stdlib.h>

#include <k4a/k4a.h>
#include <k4abt.h>
#include <unordered_map>

#include"gesturetree.h"
#include"mykinect.h"
#include <windows.h>

#define VERIFY(result, error)                                                                            \
    if(result != K4A_RESULT_SUCCEEDED)                                                                   \
    {                                                                                                    \
        printf("%s \n - (File: %s, Function: %s, Line: %d)\n", error, __FILE__, __FUNCTION__, __LINE__); \
        exit(1);                                                                                         \
    }                                                                                                    \

bool process_gesture(unsigned gesture_id, mykinect & device)
{
    printf("process_gesture\n");
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
        case 3:
        {
            INPUT ip1 = { 0 };
            ip1.type = INPUT_KEYBOARD;
            ip1.ki.wVk = VK_VOLUME_DOWN;
            ip1.ki.dwFlags = 0;

            INPUT ip2 = { 0 };
            ip2.type = INPUT_KEYBOARD;
            ip2.ki.wVk = VK_VOLUME_UP;
            ip2.ki.dwFlags = 0;

            ip1.ki.dwFlags = 0;
            SendInput(1, &ip1, sizeof(INPUT));
            ip1.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &ip1, sizeof(INPUT));

            ip2.ki.dwFlags = 0;
            SendInput(1, &ip2, sizeof(INPUT));
            ip2.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &ip2, sizeof(INPUT));

            k4a_float3_t init_position = {0};

            // discard first two frame which might be unstable
            device.update_skeleton(100);
            device.update_skeleton(100);

            // Do an average over 5 data points to debouncing
            float avg_x = 0.0f, avg_y = 0.0f, avg_z = 0.0f;
            for (int i = 0; i < 5; i++)
            {
                if (device.update_skeleton(K4A_WAIT_INFINITE))
                {
                    if (device._skeleton.joints[K4ABT_JOINT_HAND_LEFT].confidence_level >= K4ABT_JOINT_CONFIDENCE_MEDIUM)
                    {
                        avg_x += device._skeleton.joints[K4ABT_JOINT_HAND_LEFT].position.xyz.x;
                        avg_y += device._skeleton.joints[K4ABT_JOINT_HAND_LEFT].position.xyz.y;
                        avg_z += device._skeleton.joints[K4ABT_JOINT_HAND_LEFT].position.xyz.z;
                    }
                    else // always average 5 frames
                    {
                        i--;
                    }
                }
            }

            init_position.xyz.x = avg_x / 5;
            init_position.xyz.y = avg_y / 5;
            init_position.xyz.z = avg_z / 5;

            while (true)
            {
                if (device.update_skeleton(0))
                {
                    if (device._skeleton.joints[K4ABT_JOINT_HAND_LEFT].confidence_level >= K4ABT_JOINT_CONFIDENCE_MEDIUM)
                    {
                        if(std::abs(device._skeleton.joints[K4ABT_JOINT_HAND_LEFT].position.xyz.x - init_position.xyz.x) < 80)
                        {
                            printf("init x = %f, current x = %f, subtracted y = %f\n", init_position.xyz.x, device._skeleton.joints[K4ABT_JOINT_HAND_LEFT].position.xyz.x, device._skeleton.joints[K4ABT_JOINT_HAND_LEFT].position.xyz.y - init_position.xyz.y);
                            if ((device._skeleton.joints[K4ABT_JOINT_HAND_LEFT].position.xyz.y - init_position.xyz.y) > 50)
                            {
                                ip1.ki.dwFlags = 0;
                                SendInput(1, &ip1, sizeof(INPUT));
                                ip1.ki.dwFlags = KEYEVENTF_KEYUP;
                                SendInput(1, &ip1, sizeof(INPUT));
                            }
                            else if ((device._skeleton.joints[K4ABT_JOINT_HAND_LEFT].position.xyz.y - init_position.xyz.y) < -50)
                            {
                                ip2.ki.dwFlags = 0;
                                SendInput(1, &ip2, sizeof(INPUT));
                                ip2.ki.dwFlags = KEYEVENTF_KEYUP;
                                SendInput(1, &ip2, sizeof(INPUT));
                            }
                        }
                        else
                        {
                            printf("hand out of range\n");
                            break;
                        }
                    }
                }
                else
                {
                    continue;
                }
            }
            break;
        }
        default:
            break;
    }
    printf("gesture process finished\n");
    return true;
}

int main()
{
    std::unordered_map<k4abt_joint_id_t, k4abt_joint_t> joint_map;

    positionNode * root = new positionNode(0, false, K4ABT_JOINT_HAND_RIGHT, k4a_float3_t{0,0,0}, 200);
    positionNode * front = new positionNode(1, false, K4ABT_JOINT_HAND_RIGHT, k4a_float3_t{ -250,-80,385 }, 200);
    positionNode * back = new positionNode(2, true, K4ABT_JOINT_HAND_RIGHT, k4a_float3_t{ -200,150,800 }, 200);
    positionNode* left = new positionNode(3, false, K4ABT_JOINT_HAND_RIGHT, k4a_float3_t{ 250,-25,500 }, 200);
    positionNode* right = new positionNode(4, true, K4ABT_JOINT_HAND_RIGHT, k4a_float3_t{ -320,-85,425 }, 200);
    positionNode* left2 = new positionNode(5, true, K4ABT_JOINT_HAND_RIGHT, k4a_float3_t{ 250,-25,500 }, 200);
    positionNode* volume = new positionNode(6, true, K4ABT_JOINT_HAND_LEFT, k4a_float3_t{ 250,-60,250 }, 200);
    positionNode* volume_start = new positionNode(7, false, K4ABT_JOINT_HAND_LEFT, k4a_float3_t{ 490,-80,300 }, 200);

    back->set_gesture(1);
    right->set_gesture(2);
    left2->set_gesture(2);
    //test->set_gesture(3);
    volume->set_gesture(3);

    front->add_child(back);
    front->add_child(left2);
    left->add_child(right);
    volume_start->add_child(volume);
    //left_left_land->add_child(test);

    root->add_child(front);
    root->add_child(left);
    root->add_child(volume_start);
    //root->add_child(test);

    gestureTree mytree(root, root, &joint_map);

    int timeout = 0;

    k4a_device_configuration_t deviceConfig = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
    deviceConfig.depth_mode = K4A_DEPTH_MODE_WFOV_2X2BINNED;
    deviceConfig.color_resolution = K4A_COLOR_RESOLUTION_OFF;
    deviceConfig.camera_fps = K4A_FRAMES_PER_SECOND_15;

    k4abt_tracker_configuration_t tracker_config = K4ABT_TRACKER_CONFIG_DEFAULT;

    mykinect device(deviceConfig, tracker_config);
    device.start_cameras();

    while(true)
    {
        if (GetAsyncKeyState(VK_ESCAPE))
        {
            printf("ESC pressed, terminate\n");
            break;
        }
        if (device.update_skeleton(100))
        {
            //if (device._skeleton.joints[K4ABT_JOINT_HAND_RIGHT].confidence_level >= K4ABT_JOINT_CONFIDENCE_MEDIUM)
            //{
                joint_map[K4ABT_JOINT_HAND_RIGHT] = device._skeleton.joints[K4ABT_JOINT_HAND_RIGHT];
                joint_map[K4ABT_JOINT_HAND_LEFT] = device._skeleton.joints[K4ABT_JOINT_HAND_LEFT];
                joint_map[K4ABT_JOINT_WRIST_RIGHT] = device._skeleton.joints[K4ABT_JOINT_WRIST_RIGHT];

                //printf("X = %f, Y= %f, Z = %f\n", device._skeleton.joints[K4ABT_JOINT_HAND_LEFT].position.xyz.x, device._skeleton.joints[K4ABT_JOINT_HAND_LEFT].position.xyz.y, device._skeleton.joints[K4ABT_JOINT_HAND_LEFT].position.xyz.z);
                int gesture = mytree.traverse_map();
                if (gesture == -1)// timeout
                {
                }
                else if (gesture)     
                {
                    process_gesture(gesture, device);
                    Sleep(1000);
                }
            //}
        }
        else
        {
            continue;
        }
    } 
    printf("Finished body tracking processing!\n");

    delete root;
    delete front;
    delete back;
    delete left;
    delete right;
    delete left2;
    delete volume;
    delete volume_start;

    device.stop_cameras();

    return 0;
}