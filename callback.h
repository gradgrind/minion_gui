#ifndef CALLBACK_H
#define CALLBACK_H

#include "minion.h"

const char* dump_value(minion::MValue m);

void Callback(minion::MValue m);
void Callback1(std::string_view widget, minion::MValue data);
void Callback2(
    std::string_view widget, 
    minion::MValue data, 
    minion::MValue data2);

#endif // CALLBACK_H
