#include "functions.h"
#include <FL/Fl.H>
//#include <iostream>
using namespace std;

void f_RUN(
    minion::MMap* m)
{
    (void) m;
    //auto cc = Fl::run();
    //std::cout << "Main Loop ended: " << cc << std::endl;
    Fl::run();
}

std::unordered_map<std::string, function_handler> function_map{{"RUN", f_RUN}};
