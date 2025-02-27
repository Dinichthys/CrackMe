#ifndef CRACK_H
#define CRACK_H

enum CrackError
{
    kDone = 0,

    kCantOpenFileIn   = 1,
    kCantCallocBuffer = 2,
    kCantReadFile     = 3,
    kCantOpenFileOut  = 4,
    kCantWriteFile    = 5
};

#include "bin_patch.h"
#include "video.h"

#endif // CRACK_H
