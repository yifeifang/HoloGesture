#include "gesture.h"

gesture::gesture(int id, std::string name, void(*fp)(mykinect& device)) : _gesture_id(id), _gesture_name(name), _fp(fp)
{
}

int gesture::get_gesture_id()
{
    return _gesture_id;
}

std::string gesture::get_gesture_name()
{
    return _gesture_name;
}

void gesture::set_gesture_name(std::string name)
{
    _gesture_name = name;
}

void gesture::operator()(mykinect& device)
{
    _fp(device);
}
