#include "Quat.h"

void Quat4dconj(Quat4d& q) {
    q.w *= 1;
    q.x *= -1;
    q.y *= -1;
    q.z *= -1;
}

Quat4d Quat4dmul(Quat4d q1, Quat4d q2) {
    Quat4d result = { 0 };
    result.x = q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x;
    result.y = -q1.x * q2.z + q1.y * q2.w + q1.z * q2.x + q1.w * q2.y;
    result.z = q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z;
    result.w = -q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w;

    return result;
}

Quat4d Quat4dsub(Quat4d q1, Quat4d q2) {
    Quat4d result = { 0 };
    result.x = q1.x - q2.x;
    result.y = q1.y - q2.y;
    result.z = q1.z - q2.z;
    result.w = q1.w - q2.w;

    return result;
}