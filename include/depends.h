#ifndef DEPENDS_H
#define DEPENDS_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>

#include <QString>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QVariant>
#include <QMessageBox>

#include <rapidjson/document.h>

#include <include/config.h>


// ================ S C E N E   S T R U C T U R E =========================

#define CORRUPT_SCENE           "Scene file is damaged or incomplete!"

#define CORRUPT_SCENE_NAME      "'name' is damaged or incomplete!"
#define INVALID_SCENE_NAME      "'name' must be STRING!"

#define CORRUPT_SCENE_AUTHOR    "'author' is damaged or incomplete!"
#define INVALID_SCENE_AUTHOR    "'author' must be STRING!"

#define INVALID_OBJLIST         "'objects' must be OBJECT ARRAY!"
#define CORRUPT_OBJLIST         "'objects' is damaged or incomplete!"

#define INVALID_COORDS          "'x' and 'y' must be INT!"
#define INVALID_DIMS            "'w' and 'h' must be INT!"

#define INVALID_TYPE            "Object type must be STRING!"

#define INVALID_TEXS            "'textures' list must be STRING ARRAY!"
#define CORRUPT_TEXS            "'textures' list damaged or incomplete!"

// ========================================================================

// ============== C O N F I G   S T R U C T U R E =========================

#define CORRUPT_CONFIG          "Config file is damaged or incomplete!"
#define CORRUPT_CONFIG_NAME     "'app_name' is damaged or incomplete!"
#define CORRUPT_CONFIG_AUTHOR   "'app_author' is damaged or incomplete!"
#define CORRUPT_CONFIG_VERSION  "'app_version' is damaged or incomplete!"
#define CORRUPT_CONFIG_DIMS            "'width' and 'height' must be INT!"

// ========================================================================

#define SOUND_ACTION    0x534e44    // 'SND' in hex
#define SCENE_ACTION    0x53434e    // 'SCN' in hex
#define QUIT_ACTION     0xffffff    // Just for fun

using namespace std;
using namespace rapidjson;

struct vec2
{
    int x, y;
    vec2(int a, int b):
        x(a), y(b){}
    vec2(){}

    int X(){return x;}
    int Y(){return y;}
    int W(){return x;}
    int H(){return y;}

};

#endif // DEPENDS_H
