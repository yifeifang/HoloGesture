#pragma once
#include <k4a/k4a.h>
#include <k4abt.h>
#include<vector>
#include <unordered_map>
#include "gesture.h"

#define PI 3.14159265

class gestureNode
{
    private:
        // store the id of joint i.e. HAND
        k4abt_joint_id_t _joint;
    public:
        unsigned _id;
        unsigned _gesture_id;
        bool _leaf;
        gestureNode(unsigned id, bool leaf, k4abt_joint_id_t joint);
        virtual bool activate(k4abt_joint_t read)=0;
        bool add_child(gestureNode * child);
        bool set_gesture(gesture & new_gesture, std::unordered_map<int, gesture> & gesture_map);
        k4abt_joint_id_t get_joint(void);
        std::vector<gestureNode *> _children;
};

class positionNode : public gestureNode
{
private:
    k4a_float3_t _position;
    float _threshold;
    double distance(float x, float y, float z);
public:
    positionNode(unsigned id, bool leaf, k4abt_joint_id_t joint, k4a_float3_t position, float threshold);
    virtual bool activate(k4abt_joint_t read) override;
};

class pitchNode : public gestureNode
{
private:
    double _pitch;
    float _threshold;
public:
    pitchNode(unsigned id, bool leaf, k4abt_joint_id_t joint, double pitch, float threshold);
    virtual bool activate(k4abt_joint_t read) override;
};

class gestureTree
{
    private:
        // store the id of joint i.e. HAND
        gestureNode * state;
        gestureNode* _root;
        std::unordered_map<k4abt_joint_id_t, k4abt_joint_t> * pjoint_map;
        int timeout = 0;
    public:
        gestureTree(gestureNode* Node, gestureNode* root, std::unordered_map<k4abt_joint_id_t, k4abt_joint_t>* pjoint_map);
        //int traverse(float x, float y, float z);
        int traverse_map(void);
        bool set_state(gestureNode* Node);
};