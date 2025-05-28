#include "nachostabla.h"
#include "synch.h"
#include <unistd.h>

NachosOpenFilesTable::NachosOpenFilesTable()
{
    openFiles = new int[MAX_OPEN_FILES]; // 20 files
    openFilesMap = new BitMap(MAX_OPEN_FILES);
    usage = 0;
}

NachosOpenFilesTable::~NachosOpenFilesTable()
{
    for (int i = 0; i < MAX_OPEN_FILES; i++)
    {
        if (openFilesMap->Test(i))
        {
            close(openFiles[i]);
        }
    }
    delete[] openFiles;
    delete openFilesMap;
}

int NachosOpenFilesTable::Open(int unixHandle)
{
    int pos = openFilesMap->Find();
    if (pos == -1)
        return -1;
    openFiles[pos] = unixHandle;
    return pos;
}

int NachosOpenFilesTable::Close(int nachosHandle)
{
    if (!openFilesMap->Test(nachosHandle))
        return -1;
    close(openFiles[nachosHandle]);
    openFilesMap->Clear(nachosHandle);
    return 0;
}

bool NachosOpenFilesTable::isOpened(int nachosHandle)
{
    return openFilesMap->Test(nachosHandle);
}

int NachosOpenFilesTable::getUnixHandle(int nachosHandle)
{
    if (!openFilesMap->Test(nachosHandle))
        return -1;
    return openFiles[nachosHandle];
}

void NachosOpenFilesTable::addThread()
{
    usage++;
}

void NachosOpenFilesTable::delThread()
{
    usage--;
}

void NachosOpenFilesTable::Print()
{
    for (int i = 0; i < MAX_OPEN_FILES; i++)
    {
        if (openFilesMap->Test(i))
        {
            printf("Slot %d -> Unix FD %d\n", i, openFiles[i]);
        }
    }
}
