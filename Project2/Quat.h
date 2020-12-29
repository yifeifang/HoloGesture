#pragma once

typedef  struct
{
    float x;
    float y;
    float z;
    float w;
}Quat4d;

void Quat4dconj(Quat4d& q);

Quat4d Quat4dmul(Quat4d q1, Quat4d q2);

Quat4d Quat4dsub(Quat4d q1, Quat4d q2);