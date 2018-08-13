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
