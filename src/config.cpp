#include "../include/config.h"


void Config::cfgerr(QString errmsg)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                             "Config load error!",
                             errmsg.toStdString().c_str(),
                             NULL);
    exit(-1);
}

void Config::loadCfg(AppConfig *conf)
{
    QFile cfg(RES_ROOT"config.json");

    if(!cfg.open(QIODevice::ReadOnly))
        cfgerr(CORRUPT_CONFIG);

    Document doc;
    doc.Parse(cfg.readAll().constData());
    cfg.close();

    if(!doc.HasMember("app_name") || !doc["app_name"].IsString())
        cfgerr(CORRUPT_CONFIG_NAME);
    if(!doc.HasMember("app_author") || !doc["app_author"].IsString())
        cfgerr(CORRUPT_CONFIG_AUTHOR);
    if(!doc.HasMember("app_version") || !doc["app_version"].IsString())
        cfgerr(CORRUPT_CONFIG_VERSION);

    if(!doc.HasMember("width") || !doc["width"].IsInt() ||
       !doc.HasMember("height") || !doc["height"].IsInt() )
    {
        cfgerr(CORRUPT_CONFIG_DIMS);
    }

    conf->setName( doc["app_name"].GetString() );
    conf->setAuthor( doc["app_author"].GetString() );
    conf->setVersion( QByteArray::fromHex( doc["app_version"].GetString() ).data() );
    conf->setDimension( doc["width"].GetInt(), doc["height"].GetInt() );
    conf->is_full = doc["fullscreen"].GetBool();
    conf->setStartScene( doc["start_scene"].GetString() );

    if(!doc.HasMember("scenes"))
        cfgerr("Can't find 'scenes' list!");

    if(!doc.HasMember("textures"))
        cfgerr("Can't find 'textures' list!");

    if(!doc.HasMember("animations"))
        cfgerr("Can't find 'animations' list!");

    if(!doc.HasMember("sounds"))
        cfgerr("Can't find 'sounds' list!");

    if(!doc.HasMember("music"))
        cfgerr("Can't find 'music' list!");


    for( auto& s : doc["scenes"].GetObject() )
    {
        if(!s.name.IsString())
            cfgerr("Corrupt 'scenes' list!");
        conf->app_scenes[s.name.GetString()] = s.value.GetString();
    }

    for(auto& t : doc["textures"].GetObject())
    {
        if(!t.value.IsString())
            cfgerr("Error while 'textures' parsing!");
        conf->app_textures[t.name.GetString()] = t.value.GetString();
    }

    QString an;
    int afps, afc, asf;
    QByteArray adata;
    Document anim;
    SDL_Rect tmprect;
    QFile animfile;
    for( auto& a : doc["animations"].GetObject() )
    {
        if(!a.name.IsString())
            cfgerr("Corrupt 'animations' list!");

        animfile.setFileName(IMG_ROOT+QString(a.value.GetString()));
        animfile.open(QIODevice::ReadOnly);
        if(!animfile.isOpen())
            cfgerr("Error while "+QString(a.value.GetString())+" file open!");
        adata = animfile.readAll();
        animfile.close();

        anim.Parse(adata.constData());

        if(!anim.HasMember("animations"))
            cfgerr("Failed to retrieve 'animations' object from " IMG_ROOT+QString(a.value.GetString()));
        if(!anim["animations"].IsObject())
            cfgerr("Error in 'animations' object parsing!");
        for(auto& anims : anim["animations"].GetObject())
        {
            an = anims.name.GetString();
            for(auto& params : anims.value.GetObject())
            {
                if(params.value.GetType() == 6)
                {
                    if(QString(params.name.GetString()) == "frame-w")
                        tmprect.w = params.value.GetInt();
                    if(QString(params.name.GetString()) == "frame-h")
                        tmprect.h = params.value.GetInt();
                    if(QString(params.name.GetString()) == "startframe")
                        asf = params.value.GetInt();
                    if(QString(params.name.GetString()) == "fps")
                    {
                        if(params.value.GetInt() == 0)
                            afps = 0;
                        else
                            afps = params.value.GetInt();
                    }
                    if(QString(params.name.GetString()) == "frame-count")
                        afc = params.value.GetInt();
                }
            }
            conf->app_animations[an] = Animation2d(afc, afps);
            for(int i=asf; i < afc+asf; i++)
            {
                tmprect.x = i*tmprect.w;
                conf->app_animations[an].addFrame(tmprect);
            }
            Logger::log("Config", "Loaded "+QString::number(afc)+" frame(-s) of '"+an+"' animation with fps="+QString::number(afps));
        }
    }


    for( auto& s : doc["sounds"].GetObject() )
    {
        if(!s.name.IsString())
            cfgerr("Corrupt 'textures' list!");
        conf->sound_files[s.name.GetString()] = s.value.GetString();
    }

    for( auto& m : doc["music"].GetObject() )
    {
        if(!m.name.IsString())
            cfgerr("Corrupt 'textures' list!");
        conf->music_files[m.name.GetString()] = m.value.GetString();
    }

    Logger::log("Config", "Loaded 'config.json'.");
}

void Config::cfgwarn(QString warnmsg)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING,
                             "Warning in app!",
                             warnmsg.toStdString().c_str(),
                             NULL);
}

QStringList AppConfig::getSoundAliases()
{
    return sound_files.keys();
}

QStringList AppConfig::getSoundFiles()
{
    return sound_files.values();
}

QStringList AppConfig::getMusicAliases()
{
    return music_files.keys();
}

QStringList AppConfig::getMusicFiles()
{
    return music_files.values();
}
