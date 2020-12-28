#include "gesturetree.h"
#include <cmath>

gestureNode::gestureNode(unsigned id, bool leaf, k4abt_joint_id_t joint)
{
    this->_gesture_id = NULL;
    this->_leaf = leaf;
    this->_id = id;
    this->_joint = joint;
}

bool gestureNode::add_child(gestureNode* child)
{
    _children.push_back(child);
    return true;
}

bool gestureNode::set_gesture(unsigned gesture_id)
{
    this->_gesture_id = gesture_id;
    return true;
}

k4abt_joint_id_t gestureNode::get_joint(void)
{
    return this->_joint;
}

positionNode::positionNode(unsigned id, bool leaf, k4abt_joint_id_t joint, k4a_float3_t position, float threshold) : gestureNode(id, leaf, joint), _position(position), _threshold(threshold)
{
}

double positionNode::distance(float x, float y, float z)
{
    return sqrt(pow((this->_position.xyz.x - x), 2) + pow((this->_position.xyz.y - y), 2) + pow((this->_position.xyz.z - z), 2));
}

bool positionNode::match(k4abt_joint_t read)
{
    return distance(read.position.xyz.x, read.position.xyz.y, read.position.xyz.z) >= _threshold ? false : true;
}

orientationNode::orientationNode(unsigned id, bool leaf, k4abt_joint_id_t joint, k4a_quaternion_t orientation, float threshold) : gestureNode(id, leaf, joint), _orientation(orientation), _threshold(threshold)
{
}

bool orientationNode::match(k4abt_joint_t read)
{
    printf("Orientation matching here\n");
    return true;
}


gestureTree::gestureTree(gestureNode* Node, std::unordered_map<k4abt_joint_id_t, k4abt_joint_t>* pjoint_map)
{
    this->pjoint_map = pjoint_map;
    this->state = Node;
}

//int gestureTree::traverse(float x, float y, float z)
//{
//    for (auto & child : state->_children)
//    {
//        if (child->match(x, y, z, 200))
//        {
//            if (child->leaf)
//            {
//                printf("Detect state ID = %d\n", child->id);
//                timeout = 0;
//                return child->gesture_id;
//            }
//            else
//            {
//                printf("Detect state ID = %d\n", child->id);
//                state = child;
//            }
//        }
//        else if (timeout >= 75)
//        {
//            timeout = 0;
//            printf("Gesture timeout\n");
//            return -1;
//        }
//        else
//        {
//            timeout++;
//        }
//    }
//    return 0;
//}

int gestureTree::traverse_map(void)
{
    for (auto& child : state->_children)
    {
        if (child->match((*pjoint_map)[child->get_joint()]))
        {
            if (child->_leaf)
            {
                printf("Detect state ID = %d\n", child->_id);
                timeout = 0;
                return child->_gesture_id;
            }
            else
            {
                printf("Detect state ID = %d\n", child->_id);
                state = child;
            }
        }
        else if (timeout >= 150)
        {
            timeout = 0;
            printf("Gesture timeout\n");
            return -1;
        }
        else
        {
            timeout++;
        }
    }
    return 0;
}

bool gestureTree::set_state(gestureNode * Node)
{
    state = Node;
    return true;
}


