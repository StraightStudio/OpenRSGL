#include <loader3d.h>

Loader3D::Loader3D()
{

}

Loader3D::~Loader3D()
{
}

void Loader3D::LoadModel(unistring fname, Object3d &target)
{
	ifstream fin(fname);

	unistring ts;
	unistrlist parts;
    vector <glm::vec3> coords;
    vector <glm::vec2> texcoords;
    unistrlist trio;

    target.vertices.clear();
    target.texCoords.clear();

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

                target.vertices.push_back( coords.at( atol(trio.at(0).c_str())-1).x );
                target.vertices.push_back( coords.at( atol(trio.at(0).c_str())-1).y );
                target.vertices.push_back( coords.at( atol(trio.at(0).c_str())-1).z );

                if(texcoords.size() > 0)
                {
                    target.texCoords.push_back( texcoords.at( atol(trio.at(1).c_str())-1).x );
                    target.texCoords.push_back( texcoords.at( atol(trio.at(1).c_str())-1).y );
                }
            }
		}
    }
    fprintf(stdout, "Model read. Filling buffers...\n");
    fflush(stdout);
    fin.close();

    target.update(target.vertices);
}
