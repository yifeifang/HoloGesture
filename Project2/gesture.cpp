#include "gesture.h"

// should never use the default constructor. It is provided only for unordered map cast auto var = map[key] which should never
// used here
gesture::gesture()
{
    _gesture_id = -1;
    _gesture_name = "Non-exsit gesture";
    _fp = NULL;
}

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
