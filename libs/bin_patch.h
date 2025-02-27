#ifndef BIN_PATCH_H
#define BIN_PATCH_H

#include "crack.h"

#include <stdlib.h>

const size_t kJEIndex = 0x36;
const u_int8_t kJMPHex = 0x36;

const char* const kFileOutName = "cracked.com";

enum CrackError Crack (const char* const file_input_name);

const char* EnumErrorToStr (const enum CrackError error);

#endif // BIN_PATCH_H
