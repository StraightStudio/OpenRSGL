
#ifdef _WIN64
#include <config.h>
#else 
#include <include/config.h>
#endif


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
    ifstream cfg;
    cfg.open(RES_ROOT "config.json");

    if(!cfg.is_open())
        cfgerr(CORRUPT_CONFIG);

    unistring fbuff;
    getline (cfg, fbuff, (char)cfg.eof());

    Document doc;
    doc.Parse(fbuff.c_str());
    cfg.close();

    if(!doc.IsObject())
        cfgerr(CORRUPT_CONFIG);

    if(!doc.HasMember("app_name") || !doc["app_name"].IsString())
        cfgerr(CORRUPT_CONFIG);
    if(!doc.HasMember("app_author") || !doc["app_author"].IsString())
        cfgerr(CORRUPT_CONFIG);

    if(!doc.HasMember("width") || !doc["width"].IsInt() ||
       !doc.HasMember("height") || !doc["height"].IsInt() )
    {
        cfgerr(CORRUPT_CONFIG);
    }

    if(!doc.HasMember("playername"))
        conf.playername = "Guest";
    else
    {
        if(!doc["playername"].IsString())
            cfgerr("'playername' parameter must be STRING!");
        conf.playername = doc["playername"].GetString();
    }


    conf.setName( doc["app_name"].GetString() );
    conf.setAuthor( doc["app_author"].GetString() );
    conf.setDimension( doc["width"].GetInt(), doc["height"].GetInt() );
    conf.is_full = doc["fullscreen"].GetBool();
    conf.setStartScene( doc["start_scene"].GetString() );


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

    if(!doc.HasMember("resources"))
        cfgerr("Can't find 'resources' list!");

    ifstream mfile;
    unistring tex, type, anim, st, name;
    unistrlist sts, mts, punits;
    int w,h, rw,rh;
    vec2 so;
    map<unistring, unistring> trigger;
    Document mds;
    Actor2d actor;
    for( auto& m : doc["models"].GetObject() )
    {
        if(!m.value.IsString())
            cfgerr("Corrupt 'models' list!");
        mfile.open(MODELS_ROOT+unistring(m.value.GetString()));
        if(!mfile.is_open())
            cfgerr("Unable to open '" MODELS_ROOT+unistring(m.value.GetString())+"'!");

        unistring afbuff;
        getline(mfile, afbuff, (char)mfile.eof());

        mds.Parse(afbuff.c_str());
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
            type = p.value["type"].GetString();

            if(!p.value["texture"].IsString())
                Config::cfgerr(INVALID_TYPE);
            tex = p.value["texture"].GetString();

            if(!p.value["animation"].IsString())
                Config::cfgerr(INVALID_TYPE);
            anim = p.value["animation"].GetString();

            if(type == "button")
            {
                if(!p.value["hover"].IsString())
                    cfgerr("'hover' event is not defined!");
                trigger["hover"] = p.value["hover"].GetString();

                if(!p.value["click"].IsString())
                    cfgerr("'click' event is not defmined!");
                trigger["click"] = unistring(p.value["click"].GetString());
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

                if(p.value.HasMember("units"))
                {
                    if(p.value["units"].IsArray())
                    {
                        for(auto& u : p.value["units"].GetArray())
                            punits.push_back(u.GetString());
                    }
                }

                actor.setSO(so);
                actor.setStructType(st);
                actor.punits = punits;
            }

            if(type == "actor")
            {
                if(!p.value.HasMember("sel-taunts"))
                    cfgerr("'sel-taunts' variable must be defined in actor!");
                if(!p.value["sel-taunts"].IsArray())
                    cfgerr("'sel-taunts' variable must be STRING ARRAY!");
                for(auto& t : p.value["sel-taunts"].GetArray())
                    sts.push_back(t.GetString());

                if(!p.value.HasMember("mov-taunts"))
                    cfgerr("'mov-taunts' variable must be defined in actor!");
                if(!p.value["mov-taunts"].IsArray())
                    cfgerr("'mov-taunts' variable must be STRING ARRAY!");
                for(auto& t : p.value["mov-taunts"].GetArray())
                    mts.push_back(t.GetString());
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
            conf.app_models[name] = actor;
            Logger::log("Config", "Loaded model '"+name+"'.");
        }
    }

    for(auto& t : doc["textures"].GetObject())
    {
        if(!t.value.IsString())
            cfgerr("Error while 'textures' parsing!");
        conf.app_textures[t.name.GetString()] = t.value.GetString();
    }

    unistring an;
    int afps, afc, asf;
    Document anim_doc;
    SDL_Rect tmprect;
    ifstream animfile;
    for( auto& a : doc["animations"].GetObject() )
    {
        if(!a.name.IsString())
            cfgerr("Corrupt 'animations' list!");

        animfile.open(IMG_ROOT+unistring(a.value.GetString()));
        if(!animfile.is_open())
            cfgerr("Error while "+unistring(a.value.GetString())+" file open!");


        string afbuff;
        getline(animfile, afbuff, (char)animfile.eof());

        animfile.close();


        anim_doc.Parse(afbuff.c_str());

        if(!anim_doc.HasMember("animations"))
            cfgerr("Failed to retrieve 'animations' object from " IMG_ROOT+unistring(a.value.GetString()));
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

            conf.app_animations.insert(std::pair<unistring, Animation2d>(an, Animation2d(afc, afps) ) );
            for(int i=asf; i < afc+asf; i++)
            {
                tmprect.x = i*tmprect.w;
                conf.app_animations[an].addFrame(tmprect);
            }
            Logger::log("Config", "Loaded "+to_string(afc)+" frame(-s) of '"+an+"' animation with fps="+to_string(afps));
        }
    }


    for( auto& s : doc["sounds"].GetObject() )
    {
        if(!s.name.IsString())
            cfgerr("Corrupt 'textures' list!");
        conf.sound_files.insert(std::pair<unistring, unistring>(s.name.GetString(), s.value.GetString()));
    }

    for( auto& m : doc["music"].GetObject() )
    {
        if(!m.name.IsString())
            cfgerr("Corrupt 'textures' list!");
        conf.music_files.insert(std::pair<unistring, unistring>(m.name.GetString(), m.value.GetString()));
    }

    for(auto& r : doc["resources"].GetArray())
    {
        conf.resource_files.push_back(r.GetString());
    }

    Logger::log("Config", "Loaded 'config.json'.");
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
