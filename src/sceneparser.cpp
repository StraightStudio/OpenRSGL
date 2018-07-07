#include <include/sceneparser.h>

SceneParser::SceneParser()
{

}

void SceneParser::readScene(AppConfig &conf, Scene2d &target, unistring file)
{
    target.sinfo.reset();
    target.clear();

    ifstream sfile;
    sfile.open(SCENE_ROOT+file);
    if(!sfile.is_open())
        Config::cfgerr("Failed to open config.json!");

    unistring fbuff;
    getline(sfile, fbuff, (char)sfile.eof());

    sfile.close();
    Document doc;

    doc.Parse(fbuff.c_str());
    if(!doc.IsObject()) // Check for json integrity
        Config::cfgerr(CORRUPT_SCENE);


    if(!doc.HasMember("name")) // Check if "name" exists.
        Config::cfgerr(CORRUPT_SCENE);

    if(!doc["name"].IsString()) // Check "name" type
        Config::cfgerr(CORRUPT_SCENE);
    else
        target.sinfo.setName( doc["name"].GetString() ); // Get "name"


    if(!doc.HasMember("author")) // If "author" exists.
        Config::cfgerr(CORRUPT_SCENE);

    if(!doc["author"].IsString()) // Check "author" type
        Config::cfgerr(CORRUPT_SCENE);
    else
        target.sinfo.setAuthor( doc["author"].GetString() ); // Get "author"


    if(!doc.HasMember("type")) // If "type" exists.
        Config::cfgerr(CORRUPT_SCENE);

    if(!doc["type"].IsString()) // Check "type" type
        Config::cfgerr(CORRUPT_SCENE);
    else
        target.sinfo.setType( doc["type"].GetString() ); // Get "type"


    if(unistring(doc["type"].GetString()) == "game")
    {
        if(!doc.HasMember("players"))
            Config::cfgerr("Unable to get 'players' OBJECT!");
        if(!doc["players"].IsObject())
            Config::cfgerr("'players' must be OBJECT!");

        for(auto& p : doc["players"].GetObject())
        {
            Logger::log("SceneParser", "Player "+unistring(p.value.GetString())+" joined game.");
        }
    }


    if(!doc.HasMember("objects")) // Check if "objects" exists
        Config::cfgerr("Unable to get 'objects' OBJECT!");

    if(!doc["objects"].IsObject()) // Check "objects" type
        Config::cfgerr("'objects' must be OBJECT!");


    int x,y;
    unistring model, parent;
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
        model = i.value["model"].GetString();

        if(!i.value.HasMember("parent"))
            parent = "player";
        else
        {
            if(!i.value["parent"].IsString())
                Config::cfgerr("'parent' variable must be STRING!");
            parent = i.value["parent"].GetString();
        }
        target.addActor(conf, vec2(x,y), model, parent);
    }

    if(doc.HasMember("bg-track"))
    {
        if(!doc["bg-track"].IsString())
            Config::cfgerr(CORRUPT_SCENE);
        else
            target.sinfo.setBgTrack(doc["bg-track"].GetString());
    }

    if(target.sinfo.name != "menu") // Not necessary to show game special scenes.
        Logger::log(unistring("SceneParser"), "Loaded scene '"+target.sinfo.name+
                                              "', made by '"+target.sinfo.author+"'" );

}

void SceneParser::loadScene(Scene2d &target, AppConfig &conf)
{
    readScene(conf, target, conf.getStartScene());
}

void SceneParser::loadScene(Scene2d &target, unistring file, AppConfig &conf)
{
    readScene(conf, target, file.c_str());
}
