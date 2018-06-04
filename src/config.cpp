#include "../include/config.h"


void Config::cfgerr(QString errmsg)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                             "Config load error!",
                             errmsg.toStdString().c_str(),
                             NULL);
}

void Config::loadCfg(AppConfig *conf)
{
    QFile cfg(RES_ROOT"config.json");

    if(!cfg.open(QIODevice::ReadOnly))
    {
        cfgerr(CORRUPT_CONFIG);
        exit(-1);
    }
    QTextStream in(&cfg);
    QByteArray cdata;
    QString line;
    while(!in.atEnd())
    {
        line = in.readLine();
        if(!line.trimmed().startsWith("#"))
            cdata.append(line);
    }

    Document doc;
    doc.Parse(cdata.constData());

    if(!doc.HasMember("app_name") || !doc["app_name"].IsString())
        cfgerr(CORRUPT_CONFIG_NAME);
    if(!doc.HasMember("app_author") || !doc["app_author"].IsString())
        cfgerr(CORRUPT_CONFIG_AUTHOR);
    if(!doc.HasMember("app_version") || !doc["app_version"].IsString())
        cfgerr(CORRUPT_CONFIG_VERSION);
    if(!doc.HasMember("background-audio") || !doc["background-audio"].IsArray())
        cfgerr("'background-audio' must be STRING ARRAY!");

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

    for( auto& s : doc["scenes"].GetObject() )
    {
        if(!s.name.IsString())
            cfgerr("Corrupt 'scenes' list!");
        conf->app_scenes[s.name.GetString()] = s.value.GetString();
    }

    for( auto& t : doc["textures"].GetObject() )
    {
        if(!t.name.IsString())
            cfgerr("Corrupt 'textures' list!");
        conf->app_textures[t.name.GetString()] = t.value.GetString();
    }

    for(SizeType i=0; i < doc["background-audio"].Size(); i++)
    {
        conf->background_audio_list.append(doc["background-audio"][i].GetString());
    }
    Logger::log("Config", "Loaded 'config.json'.");
}
