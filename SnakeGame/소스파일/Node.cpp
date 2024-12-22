#include "Node.h"

Node::Node() {
    x = 0;
    y = 0;
}

Node::Node(int a, int b) {
    this->x = GameXY(a);
    this->y = GameXY(b);
}

Node::~Node() {

}

void Node::setX(int a) {
    this->x = GameXY(a);
}

void Node::setY(int b) {
    this->y = GameXY(b);
}

int Node::GameXY(int c) { //�ȼ� -> ����ȭ
    c = c * S_partSize;
    return c;
}