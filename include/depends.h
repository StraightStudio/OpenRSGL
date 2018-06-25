#ifndef DEPENDS_H
#define DEPENDS_H
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <include/SDL2/SDL.h>
#include <include/SDL2/SDL_image.h>
#include <include/SDL2/SDL_mixer.h>
#include <include/SDL2/SDL_net.h>
#include <include/SDL2/SDL_ttf.h>

#include <QString>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <QVariant>
#include <QMessageBox>
#include <QObject>

#include <cairo/cairo-svg.h>

#include <rapidjson/document.h>

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
using namespace rapidjson;

#endif
