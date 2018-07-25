#include <loader3d.h>

Loader3D::Loader3D()
{

}

Loader3D::~Loader3D()
{
}

void Loader3D::LoadModel(unistring fname, vector<GLfloat> &target)
{
	ifstream fin(fname);

	unistring ts;
	unistrlist parts;
    vector <vec3> coords;
    unistrlist trio;

    target.clear();
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
                    break;
                coords.push_back( vec3(atof(parts.at(1).c_str()), atof(parts.at(2).c_str()), atof(parts.at(3).c_str()) ) );
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
                target.push_back( coords.at( atol(trio.at(0).c_str())-1).X() );
                target.push_back( coords.at( atol(trio.at(0).c_str())-1).Y() );
                target.push_back( coords.at( atol(trio.at(0).c_str())-1).Z() );
}
		}
    }
    fin.close();
}
