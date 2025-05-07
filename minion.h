#ifndef MINION_H
#define MINION_H

typedef unsigned int msize;

typedef struct
{
    short type;
    short flags;
    msize size;
    void* data;
} minion_value;

typedef struct macro_node
{
    char* name;
    struct macro_node* next;
    minion_value value;
} macro_node;

typedef struct
{
    minion_value minion_item;
    minion_value error;
    macro_node* macros;
} minion_doc;

// The result must be freed when it is no longer needed
minion_doc minion_read(const char* input);

// Return the error message, or NULL if there was no error
char* minion_error(minion_doc doc);

// Free the memory used for a minion_doc.
void minion_free(minion_doc doc);

char* minion_dump(minion_value source, int depth);
// Free the memory used for a minion dump.
void minion_tidy_dump();

// Free longer term minion memory (can be retained between minion_read calls)
void minion_tidy();

#endif // MINION_H
