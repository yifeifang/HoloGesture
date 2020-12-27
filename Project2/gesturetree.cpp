#include "gesturetree.h"
#include <cmath>

double gestureNode::distance(float x, float y, float z)
{
    return sqrt(pow((this->position.xyz.x - x), 2) + pow((this->position.xyz.y - y), 2) + pow((this->position.xyz.z - z), 2));
}

gestureNode::gestureNode(unsigned id, bool leaf, k4abt_joint_id_t joint, k4a_float3_t position)
{
    this->gesture_id = NULL;
    this->leaf = leaf;
    this->id = id;
    this->joint = joint;
    this->position = position;
}

bool gestureNode::match(float x, float y, float z, float threshold)
{
    return distance(x, y, z) >= threshold ? false : true;
}

bool gestureNode::add_child(gestureNode child)
{ 
    children.push_back(child);
    return true;
}

bool gestureNode::set_gesture(unsigned gesture_id)
{
    this->gesture_id = gesture_id;
    return true;
}

k4abt_joint_id_t gestureNode::get_joint(void)
{
    return this->joint;
}

gestureTree::gestureTree(gestureNode* Node, std::unordered_map<k4abt_joint_id_t, k4a_float3_t>* pjoint_map)
{
    this->pjoint_map = pjoint_map;
    this->state = Node;
}

int gestureTree::traverse(float x, float y, float z)
{
    for (auto & child : state->children)
    {
        if (child.match(x, y, z, 200))
        {
            if (child.leaf)
            {
                printf("Detect state ID = %d\n", child.id);
                timeout = 0;
                return child.gesture_id;
            }
            else
            {
                printf("Detect state ID = %d\n", child.id);
                state = &child;
            }
        }
        else if (timeout >= 75)
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

int gestureTree::traverse_map(void)
{
    for (auto& child : state->children)
    {
        if (child.match((*pjoint_map)[child.get_joint()].xyz.x, (*pjoint_map)[child.get_joint()].xyz.y, (*pjoint_map)[child.get_joint()].xyz.z, 200))
        {
            if (child.leaf)
            {
                printf("Detect state ID = %d\n", child.id);
                timeout = 0;
                return child.gesture_id;
            }
            else
            {
                printf("Detect state ID = %d\n", child.id);
                state = &child;
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

bool gestureTree::set_state(gestureNode* Node)
{
    state = Node;
    return true;
}
