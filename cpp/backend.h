#ifndef BACKEND_H
#define BACKEND_H

#ifdef __cplusplus
extern "C" {
#endif

// The actual function pointer variable is in minion_gui.cpp
extern const char* (*backend)(const char*);

void SetCallbackFunction(
    const char* (*backend_pointer)(const char*) )
{
    backend = backend_pointer;
}

void Init(char* data0);

#ifdef __cplusplus
}
#endif
#endif // BACKEND_H
