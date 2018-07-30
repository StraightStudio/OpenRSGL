#include <loader3d.h>

Loader3D::Loader3D()
{

}

Loader3D::~Loader3D()
{

}

//void Loader3D::LoadModel(unistring fname, Object3d *target)
//{
//    if(target == nullptr)
//    {
//        Logger::err("Loader3D", "Error, working with null object!");
//        return;
//    }

//	ifstream fin(fname);
//    if(!fin.is_open())
//    {
//        Logger::err("Loader3D", "No such file: "+fname);
//        return;
//    }

//	unistring ts;
//	unistrlist parts;
//    vector <glm::vec3> coords;
//    vector <glm::vec2> texcoords;
//    unistrlist trio;

//    target->vertexData.clear();
//	while (getline(fin, ts))
//	{
//        parts.clear();
//        if (ts[0] == 'v') // Vertex coords
//		{
//            if(ts[1] == 't') // Texture coords
//            {
//                split(parts, ts, is_any_of(" "));
//                if(parts.size() != 3)
//                    break;
//                texcoords.push_back( glm::vec2( atof(parts.at(1).c_str()), atof(parts.at(2).c_str()) ) );
//            }
//            else if(ts[1] == 'n') // Normal coords
//            {

//            }
//            else
//            {
//                split(parts, ts, is_any_of(" "));
//                if(parts.size() < 4)
//                    break;
//                coords.push_back( glm::vec3(atof(parts.at(1).c_str()), atof(parts.at(2).c_str()), atof(parts.at(3).c_str()) ) );
//            }
//		}
//		else if (ts[0] == 'f')
//        {
//            split(parts, ts, is_any_of(" "));
//			if (parts.size() != 4)
//                break;

//            for (int i = 1; i < 4; i++)
//			{
//                trio.clear();
//                split(trio, parts.at(i), is_any_of("/"));

//                if(trio.size() < 2)
//                    break;

//                target->vertexData.push_back( coords.at( atol(trio.at(0).c_str())-1).x );
//                target->vertexData.push_back( coords.at( atol(trio.at(0).c_str())-1).y );
//                target->vertexData.push_back( coords.at( atol(trio.at(0).c_str())-1).z );

//                if(texcoords.size() > 0)
//                {
//                    target->vertexData.push_back( texcoords.at( atol(trio.at(1).c_str())-1).x );
//                    target->vertexData.push_back( texcoords.at( atol(trio.at(1).c_str())-1).y );
//                }
//                else
//                {
//                    target->vertexData.push_back( 0.f );
//                    target->vertexData.push_back( 0.f );
//                }
//            }
//		}
//    }
//    fin.close();

//    target->update(GL_STATIC_DRAW);
//    target->update("none");
//}

void Loader3D::LoadModel(unistring fname, vector<GLfloat> *vertexData)
{
    ifstream fin(fname);
    if(!fin.is_open())
    {
        Logger::err("Loader3D", "No such file: "+fname);
        return;
    }

    unistring ts;
    unistrlist parts;
    vector <glm::vec3> coords;
    vector <glm::vec2> texcoords;
    unistrlist trio;

    vertexData->clear();
    while (getline(fin, ts))
    {
        parts.clear();
        if (ts[0] == 'v') // Vertex coords
        {
            if(ts[1] == 't') // Texture coords
            {
                split(parts, ts, is_any_of(" "));
                if(parts.size() != 3)
                    break;
                texcoords.push_back( glm::vec2( atof(parts.at(1).c_str()), atof(parts.at(2).c_str()) ) );
            }
            else if(ts[1] == 'n') // Normal coords
            {

            }
            else
            {
                split(parts, ts, is_any_of(" "));
                if(parts.size() < 4)
                    break;
                coords.push_back( glm::vec3(atof(parts.at(1).c_str()), atof(parts.at(2).c_str()), atof(parts.at(3).c_str()) ) );
            }
        }
        else if (ts[0] == 'f')
        {
            split(parts, ts, is_any_of(" "));
            if (parts.size() != 4)
                break;

            for (int i = 1; i < 4; i++)
            {
                trio.clear();
                split(trio, parts.at(i), is_any_of("/"));

                if(trio.size() < 2)
                    break;

                vertexData->push_back( coords.at( atol(trio.at(0).c_str())-1).x );
                vertexData->push_back( coords.at( atol(trio.at(0).c_str())-1).y );
                vertexData->push_back( coords.at( atol(trio.at(0).c_str())-1).z );

                if(texcoords.size() > 0)
                {
                    vertexData->push_back( texcoords.at( atol(trio.at(1).c_str())-1).x );
                    vertexData->push_back( texcoords.at( atol(trio.at(1).c_str())-1).y );
                }
                else
                {
                    vertexData->push_back( 0.f );
                    vertexData->push_back( 0.f );
                }
            }
        }
    }
    fin.close();
}
