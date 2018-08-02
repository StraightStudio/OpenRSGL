#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <depends.h>
#include <config.h>
#include <logger.h>

struct SND_File
{
    ALuint abuffid;

    Mix_Chunk* audiochunk;

    SND_File():
        abuffid(0),
        audiochunk(nullptr)
    {}

    void readSound(unistring file);

    void bindSnd(ALuint src);

    void unload();
};

struct MUS_File
{
    void unload()
    {}
};

class AudioManager
{
public:
    AudioManager();
    ~AudioManager();

    void clear();

    void loadSounds(AppConfig &conf);
    void loadMusic(AppConfig &conf);

    void playSound(unistring sound, ALuint src);

    void playMusic(unistring track);
    void playMusic(unistring track, bool looped);
    void stopMusic();
private:
    map<unistring, SND_File*> a_sounds;
    map<unistring, MUS_File*> a_music;

    ALCdevice* a_audiodevice;
    ALCcontext* a_audiocontext;

    ALuint test_src;
};

#endif // AUDIOMANAGER_H
