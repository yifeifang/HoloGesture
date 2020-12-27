#pragma once
#include <k4a/k4a.h>
#include <k4abt.h>
#include<vector>
#include <unordered_map>

class gestureNode
{
    private:
        // store the id of joint i.e. HAND
        k4abt_joint_id_t joint;
        k4a_float3_t position;
        double distance(float x, float y, float z);
    public:
        unsigned id;
        unsigned gesture_id;
        bool leaf;
        gestureNode(unsigned id, bool leaf, k4abt_joint_id_t joint, k4a_float3_t position);
        bool match(float x, float y, float z, float threshold);
        bool add_child(gestureNode child);
        bool set_gesture(unsigned gesture_id);
        k4abt_joint_id_t get_joint(void);
        std::vector<gestureNode> children;
};

class gestureTree
{
    private:
        // store the id of joint i.e. HAND
        gestureNode * state;
        std::unordered_map<k4abt_joint_id_t, k4a_float3_t> * pjoint_map;
        int timeout = 0;
    public:
        gestureTree(gestureNode* Node, std::unordered_map<k4abt_joint_id_t, k4a_float3_t>* pjoint_map);
        int traverse(float x, float y, float z);
        int traverse_map(void);
        bool set_state(gestureNode* Node);
};