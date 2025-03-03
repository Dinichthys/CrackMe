#include "video.h"

#include <stdlib.h>
#include <malloc.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

static void Delay (const long int milliseconds);

void* ShowVideo (void*)
{
    sf::SoundBuffer sound_buffer;
    sound_buffer.loadFromFile (kMusicFile);
    sf::Sound sound_music (sound_buffer);
    sound_music.play ();

    sf::RenderWindow window (sf::VideoMode (kWindowWidth, kWindowHeight),kWindowName);

    sf::Texture texture;
    texture.loadFromFile (kImageFile, sf::IntRect({0, 0}, {kImageWidth, kImageHeight}));
    texture.setSmooth (true);

    sf::Sprite sprite (texture);

    float pos_x = kStartPositionX;
    float pos_y = kStartPositionY;

    float move_x = kMoveX;
    float move_y = kMoveY;

    sprite.setPosition ({pos_x, pos_y});

    while (window.isOpen ())
    {
        sf::Event event;
        while (window.pollEvent (event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear ();
        window.draw (sprite);
        window.display ();
        usleep (kDelay);

        if (pos_x < kMoveX)
        {
            move_x = kMoveX;
        }

        if (pos_x > kWindowWidth - kMoveX - kImageWidth)
        {
            move_x = - kMoveX;
        }

        if (pos_y < move_y)
        {
            move_y = kMoveY;
        }

        if (pos_y > kWindowHeight - kMoveY - kImageHeight)
        {
            move_y = - kMoveY;
        }

        pos_x += move_x;
        pos_y += move_y;

        sprite.setPosition ({pos_x, pos_y});
    }

    sound_music.stop ();

    return NULL;
}
