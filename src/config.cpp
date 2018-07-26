#include <config.h>


void Config::cfgerr(unistring errmsg)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                             "Config load error!",
                             errmsg.c_str(),
                             nullptr);
    exit(-1);
}

void Config::loadCfg(AppConfig &conf)
{

}

void Config::cfgwarn(unistring warnmsg)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING,
                             "Warning in app!",
                             warnmsg.c_str(),
                             nullptr);
}

vector<unistring> AppConfig::getSoundAliases()
{
    vector<unistring> sndAliases;
    sndAliases.resize(sound_files.size(), "");
    for(auto snd : sound_files)
        sndAliases.push_back(snd.first);
    return sndAliases;
}

vector<unistring> AppConfig::getSoundFiles()
{
    vector<unistring> sndFiles;
    sndFiles.resize(sound_files.size(), "");
    for(auto snd : sound_files)
        sndFiles.push_back(snd.second);
    return sndFiles;
}
vector<unistring> AppConfig::getMusicAliases()
{
    vector<unistring> musAliases;
    musAliases.resize(music_files.size(), "");
    for(auto snd : music_files)
        musAliases.push_back(snd.first);
    return musAliases;
}

vector<unistring> AppConfig::getMusicFiles()
{
    vector<unistring> musFiles;
    musFiles.resize(music_files.size(), "");
    for(auto snd : music_files)
        musFiles.push_back(snd.second);
    return musFiles;
}
