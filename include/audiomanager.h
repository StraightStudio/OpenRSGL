#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H


#include <include/depends.h>
#include <include/config.h>
#include <include/logger.h>

class AudioManager
{
public:
    AudioManager();

    void init();

    void clear();

    void loadSounds(AppConfig &conf);
    void loadMusic(AppConfig &conf);

    void playSound(unistring sound);

    void playMusic(unistring track);
    void playMusic(unistring track, bool looped);
    void stopMusic();
private:
    map<unistring, Mix_Chunk* > a_sounds;
    map<unistring, Mix_Music* > a_music;
};

#endif // AUDIOMANAGER_H
