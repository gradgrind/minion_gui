#ifndef CALLBACK_H
#define CALLBACK_H

#include "minion.h"

minion::MinionMap Callback(minion::MinionMap m);
minion::MinionMap Callback1(std::string widget, minion::MinionValue data);
minion::MinionMap Callback2(
    std::string widget, 
    minion::MinionValue data, 
    minion::MinionValue data2);

#endif // CALLBACK_H
