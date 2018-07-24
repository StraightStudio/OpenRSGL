#ifndef DEPENDS_H
#define DEPENDS_H

#define TESTING


#include <algorithm>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <locale>
#include <map>
#include <boost/algorithm/string.hpp>
#include <random>


#ifdef TESTING

#include <steam/steam_api.h>

#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#include <rapidjson/document.h>

#undef main // SDL Windows Fix


typedef std::string unistring;

typedef std::vector<unistring> unistrlist;
// ---------------------------------------------------------------------------------

#define TARGET_FPS 60
#define HEALTHBAR_HEIGHT 4

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

// ========================================================================

#define SOUND_ACTION        0x534e44    // 'SND' in hex
#define SCENE_ACTION        0x53434e    // 'SCN' in hex

#define RESIZE_ACTION       0x524553    // 'RES' in hex
#define POS_ACTION          0x504f53    // 'POS' in hex
#define POS_RES_ACTION      0xa294a6    // 'POS' + 'RES' in hex
#define RES_SND_ACTION      0xa59397    // 'SND' + 'RES' in hex
#define POS_RES_SND_ACTION  0xf2e3f9    // 'POS' + 'RES' + 'SND' in hex
#define MOV_ACTION          0x4d4f56    // 'MOV' in hex
#define SND_MOV_ACTION      0xa09d9a    // 'MOV' in hex
#define SPW_ACTION          0x535057    // 'SPW' in hex

#define SELECTED_BUILDING_ACTION 0xfb572d // FuckingBuilding5e7ec2eD
#define DESELECTED_BUILDING_ACTION 0xfbd72d // FuckingBuildingDese7ec2eD

#define QUIT_ACTION         0x2a        // Just for fun

using namespace std;
using namespace boost;
using namespace rapidjson;

#endif
