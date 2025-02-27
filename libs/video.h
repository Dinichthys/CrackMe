#ifndef VIDEO_H
#define VIDEO_H

#include "crack.h"

#include <stdlib.h>

const size_t kWindowWidth  = 800;
const size_t kWindowHeight = 450;
const size_t kImageWidth  = 80;
const size_t kImageHeight = 100;

const float kStartPositionX = 0;
const float kStartPositionY = 20;

const float kMoveX = 10;
const float kMoveY = 10;

const long int kDelay = 100;

const char* const kImageFile = "data/roflan.png";

const char* const kMusicFile = "data/VI_KA.mp3";

void* ShowVideo (void*);

#endif // VIDEO_H
