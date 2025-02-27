#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <pthread.h>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

const size_t kJEIndex = 0x36;
const size_t kWindowWidth  = 800;
const size_t kWindowHeight = 450;

const u_int8_t kJMPHex = 0x36;

void* ShowVideo (__attribute_maybe_unused__ void* null_ptr);

int main ()
{
    pthread_t video_thread;
    pthread_create (&video_thread, NULL, ShowVideo, NULL);

    FILE* crackme_file = fopen ("VZZZLOM.COM", "r");
    if (crackme_file == NULL)
    {
        fprintf (stderr, "Can't open file for reading\n");
        return EXIT_FAILURE;
    }

    fprintf (stdout, "File was opened\n");

    fseek (crackme_file, 0, SEEK_END);
    size_t size_of_file = (size_t) ftell (crackme_file);
    fseek (crackme_file, 0, SEEK_SET);

    char* input_buffer = (char*) calloc (size_of_file, sizeof (char));
    if (input_buffer == NULL)
    {
        fprintf (stderr, "Can't calloc input buffer\n");
        return EXIT_FAILURE;
    }

    {
        size_t return_value = fread (input_buffer, sizeof (char), size_of_file, crackme_file);
        if (return_value != size_of_file)
        {
            fprintf (stderr, "Can't read the file\n");
            free (input_buffer);
            return EXIT_FAILURE;
        }
    }

    fprintf (stdout, "File was read\n");
    fclose (crackme_file);
    fprintf (stdout, "File was closed\n");


    input_buffer [kJEIndex] = kJMPHex;

    fprintf (stdout, "Print JMP hex code instead of JE hex code\n");

    FILE* cracked_file = fopen ("cracked.com", "w");
    if (cracked_file == NULL)
    {
        fprintf (stderr, "Can't open file for writing\n");
        free (input_buffer);
        return EXIT_FAILURE;
    }

    {
        size_t return_value = fwrite (input_buffer, sizeof (char), size_of_file, cracked_file);
        if (return_value != size_of_file)
        {
            fprintf (stderr, "Can't write the file\n");
            free (input_buffer);
            return EXIT_FAILURE;
        }
    }

    free (input_buffer);
    fprintf (stdout, "File was written\n");
    fclose (cracked_file);
    fprintf (stdout, "File was closed\n");

    pthread_join(video_thread, NULL);

    return EXIT_SUCCESS;
}

void* ShowVideo (void* null_ptr)
{
    (void)null_ptr;
    sf::RenderWindow window (sf::VideoMode (kWindowWidth, kWindowHeight),"Draw");

    sf::Texture texture("rofl.png", false, sf::IntRect({10, 10}, {32, 32}));

    bool done = false;

    while (window.isOpen ())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        if (!done)
        {
            window.display ();
            done = true;
        }
    }

    return NULL;
}
