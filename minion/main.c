#include "iofile.h"
#include "minion.h"
#include <stdio.h>
#include <time.h>

const char* readme = "{TEST: \"String with utf8:üößðħ\"}";

int main()
{
    const char* fp = "../../data/test4.minion";
    const char* f = read_file(fp);
    if (!f)
        printf("File not found: %s\n", fp);

    //printf("IN: '%s'\n", readme);

    struct timespec start, end;
    //clock_gettime(CLOCK_MONOTONIC, &start); // Initial timestamp
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start); // Initial timestamp

    //minion_value parsed = minion_read(readme);
    minion_doc parsed = minion_read(f);

    //clock_gettime(CLOCK_MONOTONIC, &end); // Get current time
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end); // Get current time
    double elapsed = end.tv_sec - start.tv_sec;
    elapsed += (end.tv_nsec - start.tv_nsec) / 1000.0;
    printf("%0.2f microseconds elapsed\n", elapsed);

    char* e = minion_error(parsed);
    if (e) {
        printf("ERROR: %s\n", (char*) e);
    } else {
        printf("Returned type: %d\n", parsed.minion_item.type);

        char* result = minion_dump(parsed.minion_item, 0);
        if (result)
            printf("\n -->\n%s\n", result);
        else
            printf("*** Dump failed\n");
    }
    minion_free(parsed);

    minion_tidy(); // free minion buffers
    return 0;
}
