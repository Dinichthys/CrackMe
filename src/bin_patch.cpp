#include "bin_patch.h"

#include <stdio.h>

#include "My_lib/Logger/logging.h"
#include "My_lib/Assert/my_assert.h"
#include "My_lib/My_stdio/my_stdio.h"

enum CrackError Crack (const char* const file_input_name)
{
    ASSERT (file_input_name != NULL, "The function Crack got (file_input_name = %p)\n", file_input_name);

    FILE* crackme_file = fopen (file_input_name, "r");
    if (crackme_file == NULL)
    {
        fprintf (stderr, "Can't open file for reading\n");
        return kCantOpenFileIn;
    }

    LOG (kInfo, "File \"%s\" was opened\n", file_input_name);

    size_t file_size = size_of_file (crackme_file);

    char* input_buffer = (char*) calloc (file_size, sizeof (char));
    if (input_buffer == NULL)
    {
        return kCantCallocBuffer;
    }

    {
        size_t return_value = fread (input_buffer, sizeof (char), file_size, crackme_file);
        if (return_value != file_size)
        {
            free (input_buffer);
            return kCantReadFile;
        }
    }

    LOG (kInfo, "File \"%s\" was read\n", file_input_name);
    fclose (crackme_file);
    LOG (kInfo, "File \"%s\" was closed\n", file_input_name);


    input_buffer [kJEIndex] = kJMPHex;

    LOG (kInfo, "Print JMP hex code instead of JE hex code\n");

    FILE* cracked_file = fopen (kFileOutName, "w");
    if (cracked_file == NULL)
    {
        free (input_buffer);
        return kCantOpenFileOut;
    }

    LOG (kInfo, "File \"%s\" was opened\n", kFileOutName);

    {
        size_t return_value = fwrite (input_buffer, sizeof (char), file_size, cracked_file);
        if (return_value != file_size)
        {
            free (input_buffer);
            return kCantWriteFile;
        }
    }

    free (input_buffer);
    LOG (kInfo, "File \"%s\" was written\n", kFileOutName);
    fclose (cracked_file);
    LOG (kInfo, "File \"%s\" was closed\n", kFileOutName);

    return kDone;
}

const char* EnumErrorToStr (const enum CrackError error)
{
    #define CASE(error)             \
        case error:                 \
        {                           \
            return #error;          \
        }

    switch (error)
    {
        CASE (kDone);

        CASE (kCantOpenFileIn);
        CASE (kCantCallocBuffer);
        CASE (kCantReadFile);
        CASE (kCantOpenFileOut);
        CASE (kCantWriteFile);

        default:
            return "Invalid enum element";
    }

    #undef CASE
}
