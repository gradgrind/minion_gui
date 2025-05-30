#include "minion.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>

using namespace std;
using namespace minion;

bool readfile(
    string &data, const string &filepath)
{
    std::ifstream file(filepath);
    if (file) {
        data.assign((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        return true;
    }
    return false;
}

string callback(data string) string {
	printf("callback got '%s'\n", data)
	return "{callback_result: Value}"
}

int main()
{
    auto fplist = {
        "../examples/buttons1.minion"
        //
    };

    string guidata;

    for (int count = 0; count < 1; ++count) {
        for (const auto& fp : fplist) {
            if (!readfile(guidata, fp)) {
                printf("File not found: %s\n", fp);
                exit(1);
            }


MinionGui(guidata, callback)

}
    }

    return 0;
}
