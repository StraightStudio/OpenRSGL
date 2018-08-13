#include <fontmanager.h>

FontManager::FontManager()
{

}

void FontManager::loadFont(unistring file)
{
	ifstream in;
	in.open(file.c_str());
	if(!in.is_open())
		return;
	//
	unistring line;
	getline(in, line, '\n');
	vector<unistring> params;
	params.resize(2);
	split(params, line, is_any_of("x"), token_compress_off); // Compression needed only for big strings
	if(params.size() == 2)
	{
		int lw,lh; // Letter width/height
		lw = atoi(params[0].c_str());
		lh = atoi(params[1].c_str());

		// Second line
		line.clear();
		getline(in, line, '\n');
		split(params, line, is_any_of(" "), token_compress_on);
		for(int i=0; i < params.size(); i++)
		{
			u8char cc = *(params.at(i).c_str());
			charTextures[cc] = new Letter(lw, lh);
		}
	}
	else
	{
		Logger::warn("FontManager", "Error loading font: "+file+", continuing without it.");
	}
	//
	in.close();
}

void FontManager::renderText(unistring text, glm::vec2 pos)
{

}

void FontManager::clear()
{
	for(auto &i : charTextures)
	{
		if(i.second != NULL)
		{
			delete i.second;
		}
	}
}
