#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <depends.h>
#include <logger.h>

struct Letter
{
	GLuint texID;
	int width;
	int height;
	Letter() :
		width(0), height(0)
	{
		glGenTextures(1, &texID);
	}
	Letter(int w, int h) :
		width(w), height(h)
	{
		glGenTextures(1, &texID);
	}
	~Letter()
	{
		if(texID != 0)
			glDeleteTextures(1, &texID);
	}
};

class FontManager
{
public:
    FontManager();
    void loadFont(unistring file);
    void renderText(unistring text, glm::vec2 pos);
    void clear();
private:
    map<u8char, Letter*> charTextures;
};

#endif // FONTMANAGER_H
