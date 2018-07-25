#include <loader3d.h>

Loader3D::Loader3D() :
    cache(nullptr)
{
    vertices.clear();
    model_map.clear();
    cachedModel.clear();
}

Loader3D::~Loader3D()
{
    free(cache);
    model_map.clear();
    cachedModel.clear();
}

void Loader3D::LoadModel(unistring fname, unistring alias)
{
	ifstream fin(fname);

	unistring ts;
	unistrlist parts;
    vector <vec3> coords;
    unistrlist trio;

    vertices.clear();
	while (getline(fin, ts)) 
	{
        parts.clear();
        if (ts[0] == 'v') // Vertex coords
		{
            if(ts[1] == 't') // Texture coords
            {

            }
            else if(ts[1] == 'n') // Normal coords
            {

            }
            else
            {
                split(parts, ts, is_any_of(" "));
                if(parts.size() < 4)
                    return;
                coords.push_back( vec3(atof(parts.at(1).c_str()), atof(parts.at(2).c_str()), atof(parts.at(3).c_str()) ) );
            }
		}
		else if (ts[0] == 'f')
		{
			split(parts, ts, is_any_of(" "));
			if (parts.size() != 4)
				return;

            for (int i = 1; i < 4; i++)
			{
                trio.clear();
                split(trio, parts.at(i), is_any_of("/"));
                if(trio.size() < 2)
                    return;
                vertices.push_back( coords.at( atol(trio.at(0).c_str())-1).X() );
                vertices.push_back( coords.at( atol(trio.at(0).c_str())-1).Y() );
                vertices.push_back( coords.at( atol(trio.at(0).c_str())-1).Z() );
			}
		}
    }
    fin.close();

    model_map[alias] = vertices;
    cacheModel(alias);
}

GLfloat* Loader3D::GetModel(unistring mdl)
{
    if(cachedModel != mdl)
        cacheModel(mdl);
    return cache;
}

size_t Loader3D::GetSize(unistring mdl)
{
    return model_map.at(mdl).size();
}

void Loader3D::cacheModel(unistring mdl)
{
    if(model_map.find(mdl) == model_map.end())
        return;

    free(cache);
    cache = (GLfloat*)calloc(model_map.size(), sizeof(GLfloat)); // Allocate memory for data!

    printf("Size: %lu\n", model_map.at(mdl).size());
    fflush(stdout);
    for(size_t i=0; i < model_map.at(mdl).size(); i++)
        cache[i] = model_map.at(mdl).at(i);
    cachedModel = mdl;
}
