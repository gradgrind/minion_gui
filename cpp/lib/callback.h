#ifndef CALLBACK_H
#define CALLBACK_H

#include "minion.h"

// This is used to manage the memory of a result from minion_read. It is
// freed before a call to backend(), whose result is then parsed and
// stored there.
extern minion::MValue input_value;

// Writing (serializing) MINION messages.
std::string dump_value(minion::MValue m);
void value_error(std::string msg, minion::MValue m);

void Callback(minion::MValue m);
void Callback0(std::string& widget);
void Callback1(std::string& widget, minion::MValue data);
void Callback2(std::string& widget, minion::MValue data, minion::MValue data2);

#endif // CALLBACK_H
