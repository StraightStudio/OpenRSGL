#include "include/audiomanager.h"

AudioManager::AudioManager()
{

}

AudioManager::~AudioManager()
{
    for(Mix_Chunk* ch : a_sounds.values())
        Mix_FreeChunk(ch);

    for(Mix_Music* ms : a_music.values())
        Mix_FreeMusic(ms);


    Mix_CloseAudio();
    Mix_Quit();
}

void AudioManager::init()
{
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                 "Audio device is not open!",
                                 "Failed to open audio device!",
                                 NULL);
        exit(-1);
    }
}

void AudioManager::loadSounds(AppConfig &conf)
{
    for(QString s : conf.getSoundAliases())
    {
        if(!s.isEmpty())
            a_sounds[s] = Mix_LoadWAV( QString(AUDIO_ROOT "sounds/"+conf.sound_files[s]).toStdString().c_str() );
    }
}

void AudioManager::loadMusic(AppConfig &conf)
{
    for(QString m : conf.getMusicAliases())
    {
        if(!m.isEmpty())
            a_music[m] = Mix_LoadMUS( QString(AUDIO_ROOT "music/"+conf.music_files[m]).toStdString().c_str() );
    }
}

void AudioManager::playSound(QString sound)
{
    Mix_PlayChannel(-1, a_sounds[sound], 0);
}

void AudioManager::playMusic(QString track, bool looped)
{
    if(Mix_PlayingMusic())
        stopMusic();
    if(looped)
        Mix_PlayMusic(a_music[track], -1);
    else
        Mix_PlayMusic(a_music[track], 0);
}

void AudioManager::playMusic(QString track)
{
    if(Mix_PlayingMusic())
        stopMusic();
    Mix_PlayMusic(a_music[track], 0);
}

void AudioManager::stopMusic()
{
    Mix_HaltMusic();
}
