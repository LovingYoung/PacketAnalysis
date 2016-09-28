#ifndef QUEUE_H
#define QUEUE_H

#include <queue>
#include "homogenization.h"

class myQueue{
public:
    static std::queue<Homo *> homoqueue;
    static int mutex;
};

#endif
