#ifndef CALLBACK_H
#define CALLBACK_H

#include "minion.h"

minion_value Callback(minion_value m);
minion_value Callback1(const char* widget, minion_value data);
minion_value Callback2(
    const char* widget, 
    minion_value data, 
    minion_value data2);

#endif // CALLBACK_H
