#pragma once
#include <string>
#include "mykinect.h"

class gesture
{
    private:
        // store the id of joint i.e. HAND
        int _gesture_id;
        std::string _gesture_name;
        void (*_fp)(mykinect& device);
    public:
        gesture(int id, std::string name, void (*fp)(mykinect& device));
        int get_gesture_id();
        std::string get_gesture_name();
        void set_gesture_name(std::string name);
        void operator() (mykinect& device);
};