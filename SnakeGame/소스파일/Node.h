#pragma once

#include "include.h"


class Node {
public:
    Node();
    Node(int a, int b);
    virtual ~Node();
    void setX(int a);
    void setY(int b);
    int GameXY(int c);

    int x;
    int y;


};
