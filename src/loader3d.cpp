#include <loader3d.h>

Loader3D::Loader3D()
{
}

void Loader3D::LoadModel(unistring fname)
{
	ifstream fin(fname);

	unistring ts;
	unistrlist parts;
	vector <GLfloat> coords;
	unistrlist duo;
	int i = 0;
	
	while (getline(fin, ts)) 
	{
	    if (ts[0] == 'v')
		{
			split(parts, ts, is_any_of(" "));
			coords.push_back(atof(parts.at(1).c_str()));
			coords.push_back(atof(parts.at(2).c_str()));
			coords.push_back(atof(parts.at(3).c_str()));
			printf("Vert: %f, %f, %f", atof(parts.at(1).c_str()), atof(parts.at(2).c_str()), atof(parts.at(3).c_str()));
			fflush(stdout);
		}
		else if (ts[0] == 'f')
		{
			split(parts, ts, is_any_of(" "));
			if (parts.size() != 4)
				return;
			for (i = 1; i < 4; i++);
			{
				split(duo, parts.at(i), is_any_of ("//"));
				if (duo.size() != 2)
					continue;
				printf("duo::%s,%s\n", duo.at(0).c_str(), duo.at(1).c_str());
				fflush(stdout);
				vertices.push_back(0);
			}
		}
		
	}

		fin.close();
}

void * Loader3D::GetModel()
{
	for (int i = 0; i < vertices.size(); i++)
	{
		tmp[i] = vertices.at(i);
	}

	return &tmp;
}
