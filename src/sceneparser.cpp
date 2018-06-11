#include "./include/sceneparser.h"

SceneParser::SceneParser()
{

}

void SceneParser::readScene(AppConfig &conf, Scene2d *target, QString file)
{
    QFile sfile(SCENE_ROOT+file);
    sfile.open(QIODevice::ReadOnly);

    QByteArray sdata = sfile.readAll();
    sfile.close();

    Document doc;

    bool err=false;
    QString derr;

    doc.Parse(sdata.constData());   
    if(!doc.IsObject()) // Check for json integrity
    {
        derr = CORRUPT_SCENE;
        err = true;
    }



    if(!doc.HasMember("name")) // Check if "name" exists.
    {
        derr = CORRUPT_SCENE_NAME;
        err = true;
    }
    if(!doc["name"].IsString()) // Check "name" type
    {
        derr = INVALID_SCENE_NAME;
        err = true;
    }
    else
        target->sinfo.setName( doc["name"].GetString() ); // Get "name"



    if(!doc.HasMember("author")) // If "author" exists.
    {
        derr = CORRUPT_SCENE_AUTHOR;
        err = true;
    }
    if(!doc["author"].IsString()) // Check "author" type
    {
        derr = INVALID_SCENE_AUTHOR;
        err = true;
    }
    else
        target->sinfo.setAuthor( doc["author"].GetString() ); // Get "author"



    if(!doc.HasMember("objects")) // Check if "objects" exists
    {
        derr = CORRUPT_OBJLIST;
        err = true;
    }
    if(!doc["objects"].IsObject()) // Check "objects" type
    {
        derr = INVALID_OBJLIST;
        err = true;
    }

    if(err)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                 "Scene parsing error",
                                 QString("SceneParsing error:\n'"+derr+"'").toStdString().c_str(),
                                 NULL);
        exit(-1);
    }

    int x,y,w,h,rw,rh;
    vec2 so; // Spawn Offset
    QString type, anim, tex, st;
    QMap <QString, QString> trigger;
    for(auto& i : doc["objects"].GetObject())
    {
        if( !(i.value["x"].IsInt() || i.value["y"].IsInt()) )
        {
            derr = INVALID_COORDS;
            err = true;
            break;
        }
        x = i.value["x"].GetInt();
        y = i.value["y"].GetInt();

        if( !(i.value["w"].IsInt() || i.value["h"].IsInt()) )
        {
            derr = INVALID_DIMS;
            err = true;
            break;
        }
        w = i.value["w"].GetInt();
        h = i.value["h"].GetInt();

        if( !(i.value.HasMember("rw") || i.value.HasMember("rh")) )
        {
            rw = w;
            rh = h;
        }
        else
        {
            rw = i.value["rw"].GetInt();
            rh = i.value["rh"].GetInt();
        }

        if(!i.value["type"].IsString())
        {
            Config::cfgerr(INVALID_TYPE);
        }
        type = QString(i.value["type"].GetString()).toLower();

        if(!i.value["texture"].IsString())
        {
            Config::cfgerr(INVALID_TYPE);
        }
        tex = i.value["texture"].GetString();

        if(!i.value["animation"].IsString())
        {
            Config::cfgerr(INVALID_TYPE);
        }
        anim = i.value["animation"].GetString();

        if(type == "button")
        {
            if(!i.value["hover"].IsString())
            {
                derr = INVALID_OBJLIST;
                err = true;
                break;
            }
            trigger["hover"] = i.value["hover"].GetString();
            if(!i.value["click"].IsString())
            {
                derr = INVALID_OBJLIST;
                err = true;
                break;
            }
            trigger["click"] = i.value["click"].GetString();
        }
        else if(type == "building")
        {
            if(!i.value.HasMember("sox"))
                Config::cfgerr("'sox' variable must be set!");
            if(!i.value.HasMember("soy"))
                Config::cfgerr("'soy' variable must be set!");
            if(!i.value.HasMember("stype"))
                Config::cfgerr("'stype' variable must be set!");

            if(!i.value["sox"].IsInt()) // Spawn Offset X
                Config::cfgerr("'sox' variable must be INT!");
            so.x = i.value["sox"].GetInt();

            if(!i.value["soy"].IsInt()) // Spawn Offset Y
                Config::cfgerr("'soy' variable must be INT!");
            so.y = i.value["soy"].GetInt();

            if(!i.value["stype"].IsString())
                Config::cfgerr("'stype' variable must be STRING!");
            st = i.value["stype"].GetString();
        }

        target->addActor(vec2(x,y), vec2(w,h), vec2(rw, rh), i.name.GetString(), type, trigger, tex, anim);
        target->objs().last().setSO(so);
        target->objs().last().setStructType(st);
    }

    if(doc.HasMember("bg-track"))
    {
        if(!doc["bg-track"].IsString())
        {
            derr = "Failed to get background track!";
            err = true;
        }
        else
            target->sinfo.setBgTrack(doc["bg-track"].GetString());
    }

    if(err)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                 "Scene parsing error",
                                 QString("SceneParsing error:\n'"+derr+"'").toStdString().c_str(),
                                 NULL);
        exit(-1);
    }

    if(target->sinfo.name.toLower() != "menu") // Not necessary to show game special scenes.
        Logger::log("SceneParser", QString("Loaded scene '"+target->sinfo.name+
                                           "', made by '"+target->sinfo.author+"'") );

}

void SceneParser::loadScene(Scene2d *target, AppConfig &conf)
{
    target->objs().clear();
    readScene(conf, target, conf.getStartScene());
}

void SceneParser::loadScene(Scene2d *target, QString file, AppConfig &conf)
{
    target->objs().clear();
    readScene(conf, target, file);
}
