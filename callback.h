#ifndef CALLBACK_H
#define CALLBACK_H

#include "minion.h"

void Callback(minion::MValue m);
void Callback1(const char* widget, minion::MValue data);
void Callback2(
    const char* widget, 
    minion::MValue data, 
    minion::MValue data2);

#endif // CALLBACK_H
