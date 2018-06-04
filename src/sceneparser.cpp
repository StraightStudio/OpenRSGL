#include "./include/sceneparser.h"

SceneParser::SceneParser()
{

}

void SceneParser::readScene(Scene2d *target, QString file)
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

    int x,y,w,h;
    QString type;
    QMap <QString, QString> trigger;
    QStringList texs;
    for(auto& i : doc["objects"].GetObject())
    {
        texs.clear();
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

        if(!i.value["type"].IsString())
        {
            derr = INVALID_TYPE;
            err = true;
            break;
        }
        type = i.value["type"].GetString();

        for(SizeType n=0; n < i.value["textures"].Size(); n++)
        {
            if(!i.value["textures"][n].IsString())
            {
                derr = INVALID_TEXS;
                err = true;
                break;
            }
            texs.append(i.value["textures"][n].GetString());
        }

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

        target->addActor(vec2(x,y), vec2(w,h), texs, i.name.GetString(), type, trigger);
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

void SceneParser::loadScene(Scene2d *target, QString file)
{
    target->objs().clear();
    readScene(target, file);
}
