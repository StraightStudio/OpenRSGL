#include <audiomanager.h>

AudioManager::AudioManager()
{
    a_audiodevice = alcOpenDevice(NULL);
    if(!a_audiodevice)
    {
        Logger::err("AudioManager", "Failed to open audiodevice!");
        exit(-1);
    }
    else
    {
        Logger::log("AudioManager", "Successfully opened audiodevice.");
    }
    a_audiocontext = alcCreateContext(a_audiodevice, NULL);
    alcMakeContextCurrent(a_audiocontext);

    if(Mix_OpenAudio(48000, AUDIO_S16SYS, 1, 2048) != 0)
    {
        Logger::err("AudioManager", "Can't open audiodevice!");
        exit(-1);
    }
}

AudioManager::~AudioManager()
{
    clear();

    Mix_CloseAudio();

    alcMakeContextCurrent(NULL);
    alcDestroyContext(a_audiocontext);
    alcCloseDevice(a_audiodevice);
}

void AudioManager::clear()
{
    for(pair<unistring, SND_File*> sf : a_sounds)
    {
        sf.second->unload();
        delete sf.second;
    }

    for(pair<unistring, MUS_File*> mf : a_music)
    {
        mf.second->unload();
        delete mf.second;
    }
}

void AudioManager::loadSounds(AppConfig &conf)
{
    for(unistring &s : conf.getSoundAliases())
    {
        a_sounds[s] = new SND_File;
        a_sounds[s]->readSound(conf.sound_files.at(s));
    }
}

void AudioManager::loadMusic(AppConfig &conf)
{
    for(unistring &m : conf.getSoundAliases())
    {
        //a_music[m] = new MUS_File;
        //a_music[m]->readMusic(conf.music_files.at(m));
    }
}

void AudioManager::playSound(unistring sound, ALuint src)
{
    //if(a_sounds[sound]->audiochunk != nullptr)
    //  Mix_PlayChannel(-1, a_sounds[sound]->audiochunk, -1);
    a_sounds[sound]->bindSnd(src);

    alSourcePlay(src);
}

void AudioManager::playMusic(unistring track, bool looped)
{

}

void AudioManager::playMusic(unistring track)
{

}

void AudioManager::stopMusic()
{
}

void SND_File::readSound(unistring file)
{
    audiochunk = Mix_LoadWAV(file.c_str());
    if(audiochunk == nullptr)
    {
        Logger::err("SND_File", "I failed to load sound. Shame on me.");
        return;
    }

    if(abuffid == 0)
        alGenBuffers(1, &abuffid);
    alBufferData(abuffid, AL_FORMAT_MONO16, audiochunk->abuf, audiochunk->alen, 48000);
}

void SND_File::bindSnd(ALuint src)
{
    alSourcei(src, AL_BUFFER, abuffid);
}

void SND_File::unload()
{
    if(abuffid != 0)
        alDeleteBuffers(1, &abuffid);
    if(audiochunk != nullptr)
        Mix_FreeChunk(audiochunk);
}
