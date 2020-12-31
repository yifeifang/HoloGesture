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

bool positionNode::activate(k4abt_joint_t read)
{
    return distance(read.position.xyz.x, read.position.xyz.y, read.position.xyz.z) >= _threshold ? false : true;
}

gestureTree::gestureTree(gestureNode* Node, gestureNode* root, std::unordered_map<k4abt_joint_id_t, k4abt_joint_t>* pjoint_map)
{
    this->pjoint_map = pjoint_map;
    this->state = Node;
    this->_root = root;
}

int gestureTree::traverse_map(void)
{
    if (timeout >= 30)
    {
        timeout = 0;
        printf("Gesture timeout\n");
        set_state(_root);
        return -1;
    }
    for (auto& child : state->_children)
    {      
        if ((*pjoint_map)[child->get_joint()].confidence_level >= K4ABT_JOINT_CONFIDENCE_MEDIUM && child->activate((*pjoint_map)[child->get_joint()]))
        {
            if (child->_leaf)
            {
                printf("Detect leaf state ID = %d\n", child->_id);
                timeout = 0;
                set_state(_root);
                return child->_gesture_id;
            }
            else
            {
                printf("Detect state ID = %d\n", child->_id);
                timeout = 0;
                state = child;
            }
        }
    }
    timeout++;
    return 0;
}

bool gestureTree::set_state(gestureNode * Node)
{
    state = Node;
    return true;
}


