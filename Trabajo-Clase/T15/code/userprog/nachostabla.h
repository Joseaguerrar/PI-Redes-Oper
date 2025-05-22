#ifndef NACHOSTABLA_H
#define NACHOSTABLA_H

#include "bitmap.h"

#define MAX_OPEN_FILES 20

class NachosOpenFilesTable
{
public:
    NachosOpenFilesTable();
    ~NachosOpenFilesTable();

    int Open(int unixHandle);
    int Close(int nachosHandle);
    bool isOpened(int nachosHandle);
    int getUnixHandle(int nachosHandle);

    void addThread();
    void delThread();
    void Print();

private:
    int *openFiles;
    BitMap *openFilesMap;
    int usage;
};

#endif
