
#ifdef _WIN64
#include <audiomanager.h>
#else 
#include <include/audiomanager.h>
#endif

AudioManager::AudioManager()
{

}

void AudioManager::init()
{
    if(Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                 "Audio device is not open!",
                                 "Failed to open audio device!",
                                 NULL);
        exit(-1);
    }
}

void AudioManager::clear()
{
    for(auto &ch : a_sounds)
    {
        Mix_FreeChunk(ch.second);
        ch.second = nullptr;
    }

    for(auto &ms : a_music)
    {
        Mix_FreeMusic(ms.second);
        ms.second = nullptr;
    }

    Mix_CloseAudio();
    Mix_Quit();
}

void AudioManager::loadSounds(AppConfig &conf)
{
    for(unistring &s : conf.getSoundAliases())
    {
        if(!s.empty())
            a_sounds[s] = Mix_LoadWAV( unistring( AUDIO_ROOT "sounds/"+conf.sound_files[s] ).c_str() );
    }
}

void AudioManager::loadMusic(AppConfig &conf)
{
    for(unistring &m : conf.getMusicAliases())
    {
        if(!m.empty())
            a_music[m] = Mix_LoadMUS( unistring( AUDIO_ROOT "music/"+conf.music_files[m] ).c_str() );
    }
}

void AudioManager::playSound(unistring sound)
{
    Mix_PlayChannel(-1, a_sounds[sound.c_str()], 0);
}

void AudioManager::playMusic(unistring track, bool looped)
{
    if(Mix_PlayingMusic())
        stopMusic();
    if(looped)
        Mix_PlayMusic(a_music[track], -1);
    else
        Mix_PlayMusic(a_music[track], 0);
}

void AudioManager::playMusic(unistring track)
{
    if(Mix_PlayingMusic())
        stopMusic();
    Mix_PlayMusic(a_music[track], 0);
}

void AudioManager::stopMusic()
{
    Mix_HaltMusic();
}
