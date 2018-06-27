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


    if(!doc.HasMember("type")) // If "type" exists.
    {
        derr = CORRUPT_SCENE;
        err = true;
    }
    if(!doc["type"].IsString()) // Check "type" type
    {
        derr = INVALID_TYPE;
        err = true;
    }
    else
        target->sinfo.setType( doc["type"].GetString() ); // Get "type"

    if(doc["type"].GetString() == "game")
    {
        if(!doc.HasMember("players"))
            Config::cfgerr("Unable to get 'players' OBJECT!");
        if(!doc["players"].IsObject())
            Config::cfgerr("'players' must be OBJECT!");

        for(auto& p : doc["players"].GetObject())
        {

        }
    }


    if(!doc.HasMember("objects")) // Check if "objects" exists
        Config::cfgerr("Unable to get 'objects' OBJECT!");
    if(!doc["objects"].IsObject()) // Check "objects" type
        Config::cfgerr("'objects' must be OBJECT!");

    int x,y;
    QString model, parent;
    for(auto& i : doc["objects"].GetObject())
    {
        if( !(i.value["x"].IsInt() || i.value["y"].IsInt()) )
            Config::cfgerr(INVALID_COORDS);
        x = i.value["x"].GetInt();
        y = i.value["y"].GetInt();

        if(x < 0)
            x = conf.app_width+x;
        if(y < 0)
            y = conf.app_height+y;

        if(!i.value.HasMember("model"))
            Config::cfgerr("'model' parameter must exist!");
        if(!i.value["model"].IsString())
            Config::cfgerr("'model' variable must be STRING!");
        model = QString(i.value["model"].GetString());

        if(!i.value.HasMember("parent"))
            parent = "player";
        else
        {
            if(!i.value["parent"].IsString())
                Config::cfgerr("'parent' variable must be STRING!");
            parent = i.value["parent"].GetString();
        }
        target->addActor(conf, vec2(x,y), model, parent);
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
    target->sinfo.reset();
    target->clear();
    readScene(conf, target, conf.getStartScene());
}

void SceneParser::loadScene(Scene2d *target, QString file, AppConfig &conf)
{
    target->sinfo.reset();
    target->objs().clear();
    readScene(conf, target, file);
}
