#ifndef CALLBACK_H
#define CALLBACK_H

#include "minion.h"

// This is used to manage the memory of a result from minion_read. It is
// freed before a call to backend(), whose result is then parsed and
// stored there.
extern minion::MinionValue input_value;

// This is used for writing (serializing) MINION messages.
extern minion::DumpBuffer dump_buffer;

const char* dump_value(minion::MValue m);

void Callback(minion::MValue m);
void Callback1(std::string_view widget, minion::MValue data);
void Callback2(
    std::string_view widget, 
    minion::MValue data, 
    minion::MValue data2);

#endif // CALLBACK_H
