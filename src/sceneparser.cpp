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

    doc.Parse(sdata.constData());
    assert(doc.IsObject()); // Check for json integrity

    assert(doc.HasMember("name")); // Check if "name" exists.
    assert(doc["name"].IsString()); // Check "name" type
    target->sinfo.setName( doc["name"].GetString() ); // Get "name"

    assert(doc.HasMember("author")); // If "author" exists.
    assert(doc["author"].IsString()); // Check "author" type
    target->sinfo.setAuthor( doc["author"].GetString() ); // Get "author"

    assert(doc.HasMember("objects")); // Check if "objects" exists
    assert(doc["objects"].IsObject()); // Check "objects" type

    int x,y,w,h;
    QStringList texs;
    for(auto& i : doc["objects"].GetObject())
    {
        assert(i.value["x"].IsInt());
        assert(i.value["y"].IsInt());
        x = i.value["x"].GetInt();
        y = i.value["y"].GetInt();

        assert(i.value["w"].IsInt());
        assert(i.value["h"].IsInt());
        w = i.value["w"].GetInt();
        h = i.value["h"].GetInt();

        assert(i.value["textures"].IsArray());
        for(SizeType n=0; n < i.value["textures"].Size(); n++)
            texs.append(i.value["textures"][n].GetString());

        target->addActor(vec2(x,y), vec2(w,h), texs);

        Logger::log("SceneParser", "Obj '"+QString(i.name.GetString())+"' Pos("+QString::number(x)+","+QString::number(y)+")");
    }

    if(target->sinfo.name.toLower() != "menu") // Not necessary to show game special scenes.
        Logger::log("SceneParser", QString("Loaded scene '"+target->sinfo.name+
                                           "', made by '"+target->sinfo.author+"'") );
}
