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

    if(!doc.HasMember("textures"))
        cfgerr("Can't find 'textures' list!");

    if(!doc.HasMember("animations"))
        cfgerr("Can't find 'animations' list!");

    if(!doc.HasMember("models"))
        cfgerr("Can't find 'models' list!");

    if(!doc.HasMember("sounds"))
        cfgerr("Can't find 'sounds' list!");

    if(!doc.HasMember("music"))
        cfgerr("Can't find 'music' list!");

    QFile mfile;
    QString tex, type, anim, st, name;
    QStringList sts, mts;
    int w,h, rw,rh;
    vec2 so;
    QMap<QString, QString> trigger;
    Document mds;
    Actor2d actor;
    for( auto& m : doc["models"].GetObject() )
    {
        if(!m.value.IsString())
            cfgerr("Corrupt 'models' list!");
        mfile.setFileName(MODELS_ROOT+QString(m.value.GetString()));
        mfile.open(QIODevice::ReadOnly);
        if(!mfile.isOpen())
            cfgerr("Unable to open '" MODELS_ROOT+QString(m.value.GetString())+"'!");

        mds.Parse(mfile.readAll().constData());
        mfile.close();
        for(auto& p : mds.GetObject())
        {
            name = p.name.GetString();
            if( !(p.value["w"].IsInt() || p.value["h"].IsInt()) )
                cfgerr(INVALID_COORDS);
            w = p.value["w"].GetInt();
            h = p.value["h"].GetInt();

            if( !(p.value.HasMember("rw") || p.value.HasMember("rh")) )
            {
                rw = w;
                rh = h;
            }
            else
            {
                rw = p.value["rw"].GetInt();
                rh = p.value["rh"].GetInt();
            }

            if(!p.value["type"].IsString())
                Config::cfgerr(INVALID_TYPE);
            type = QString(p.value["type"].GetString()).toLower();

            if(!p.value["texture"].IsString())
                Config::cfgerr(INVALID_TYPE);
            tex = QString(p.value["texture"].GetString());

            if(!p.value["animation"].IsString())
                Config::cfgerr(INVALID_TYPE);
            anim = QString(p.value["animation"].GetString());

            if(type == "button")
            {
                if(!p.value["hover"].IsString())
                    cfgerr("'hover' event is not defined!");
                trigger["hover"] = p.value["hover"].GetString();

                if(!p.value["click"].IsString())
                    cfgerr("'click' event is not defined!");
                trigger["click"] = p.value["click"].GetString();
            }
            else if(type == "building")
            {
                if(!p.value.HasMember("sox"))
                    Config::cfgerr("'sox' variable must be set!");
                if(!p.value.HasMember("soy"))
                    Config::cfgerr("'soy' variable must be set!");
                if(!p.value.HasMember("stype"))
                    Config::cfgerr("'stype' variable must be set!");

                if(!p.value["sox"].IsInt()) // Spawn Offset X
                    Config::cfgerr("'sox' variable must be INT!");
                so.x = p.value["sox"].GetInt();

                if(!p.value["soy"].IsInt()) // Spawn Offset Y
                    Config::cfgerr("'soy' variable must be INT!");
                so.y = p.value["soy"].GetInt();

                if(!p.value["stype"].IsString())
                    Config::cfgerr("'stype' variable must be STRING!");
                st = p.value["stype"].GetString();

                actor.setSO(so);
                actor.setStructType(st);
            }

            if(type == "actor")
            {
                if(!p.value.HasMember("sel-taunts"))
                    cfgerr("'sel-taunts' variable must be defined in actor!");
                if(!p.value["sel-taunts"].IsArray())
                    cfgerr("'sel-taunts' variable must be STRING ARRAY!");
                for(auto& t : p.value["sel-taunts"].GetArray())
                    sts.append(t.GetString());

                if(!p.value.HasMember("mov-taunts"))
                    cfgerr("'mov-taunts' variable must be defined in actor!");
                if(!p.value["mov-taunts"].IsArray())
                    cfgerr("'mov-taunts' variable must be STRING ARRAY!");
                for(auto& t : p.value["mov-taunts"].GetArray())
                    mts.append(t.GetString());
            }
            actor.setName(m.name.GetString());
            actor.setDim(w, h);
            actor.setRealDim(rw, rh);
            actor.setTex(tex);
            actor.setType(type);
            actor.trigger = trigger;
            actor.idle_anim = anim;
            actor.curAnim = anim;
            actor.sel_taunts = sts;
            actor.mov_taunts = mts;
            conf->app_models[name] = actor;
            Logger::log("Config", "Loaded model '"+name+"'.");
        }
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
    Document anim_doc;
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

        anim_doc.Parse(adata.constData());

        if(!anim_doc.HasMember("animations"))
            cfgerr("Failed to retrieve 'animations' object from " IMG_ROOT+QString(a.value.GetString()));
        if(!anim_doc["animations"].IsObject())
            cfgerr("Error in 'animations' object parsing!");
        for(auto& anims : anim_doc["animations"].GetObject())
        {
            an = anims.name.GetString();

            if(!anims.value["frame-w"].IsInt())
                cfgerr("'frame-w' must be INT!");
            tmprect.w = anims.value["frame-w"].GetInt();

            if(!anims.value["frame-h"].IsInt())
                cfgerr("'frame-h' must be INT!");
            tmprect.h = anims.value["frame-h"].GetInt();

            if(!anims.value["startframe"].IsInt())
                cfgerr("'startframe' must be INT!");
            asf = anims.value["startframe"].GetInt();

            if(!anims.value["fps"].IsInt())
                cfgerr("'fps' must be INT!");
            afps = anims.value["fps"].GetInt();

            if(!anims.value["frame-count"].IsInt())
                cfgerr("'frame-count' must be INT!");
            afc = anims.value["frame-count"].GetInt();

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
