#include "crack.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ERROR_HANDLER(error)                                                                            \
    if (error != kDone)                                                                                 \
    {                                                                                                   \
        fprintf (stderr, "Code error = {%d} with name \"%s\"\n",                                        \
                         error, EnumErrorToStr (error));                                                \
        pthread_join(video_thread, NULL);                                                               \
        return EXIT_FAILURE;                                                                            \
    }

int main ()
{
    pthread_t video_thread;
    pthread_create (&video_thread, NULL, ShowVideo, NULL);

    ERROR_HANDLER (Crack ("VZLOM.COM"));

    pthread_join(video_thread, NULL);

    return EXIT_SUCCESS;
}
