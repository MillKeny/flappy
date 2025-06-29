/*

Thanks https://github.com/BlyZeDev for most of this code :)

*/ 

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <3ds.h>

static FS_Archive archive;

#define savepath "/config/millkeny"
#define savepathfile "/config/millkeny/flappy.dat"

void initSd() {
    fsInit();

    FSUSER_OpenArchive(&archive, ARCHIVE_SDMC, fsMakePath(PATH_ASCII, ""));

    FSUSER_CreateDirectory(archive, fsMakePath(PATH_ASCII, savepath), FS_ATTRIBUTE_DIRECTORY);
}

u32 readHighscore() {
    Handle fileHandle;
    u32 value = 0;
    u32 bytesRead;

    Result result = FSUSER_OpenFile(&fileHandle, archive, fsMakePath(PATH_ASCII, savepathfile), FS_OPEN_READ, FS_ATTRIBUTE_HIDDEN);
    if (!R_FAILED(result)){
        FSFILE_Read(fileHandle, &bytesRead, 0, &value, sizeof(u32));
    }

    FSFILE_Close(fileHandle);

    return value;
}

void writeHighscore(u32 value) {
    Handle fileHandle;
    u32 bytesWritten;

    FSUSER_OpenFile(&fileHandle, archive, fsMakePath(PATH_ASCII, savepathfile), FS_OPEN_WRITE | FS_OPEN_CREATE, FS_ATTRIBUTE_HIDDEN);

    FSFILE_Write(fileHandle, &bytesWritten, 0, &value, sizeof(u32), FS_WRITE_FLUSH);

    FSFILE_Close(fileHandle);
}

void exitSd() {
    FSUSER_CloseArchive(archive);
    fsExit();
}