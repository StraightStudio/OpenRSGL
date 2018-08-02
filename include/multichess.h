#ifndef MULTICHESS_H
#define MULTICHESS_H

#include <depends.h>
#include <config.h>

#define DIR_FRONT   glm::vec3(1.f, 0.f, 0.f)
#define DIR_BACK    -DIR_FRONT
#define DIR_RIGHT   glm::vec3(0.f, 0.f, 1.f)
#define DIR_LEFT    -DIR_RIGHT

#define PAWN_MDL    RES_ROOT "pawn.obj"
#define ROOK_MDL    RES_ROOT "rook.obj"
#define KNIGHT_MDL  RES_ROOT "knight.obj"
#define BISHOP_MDL  RES_ROOT "bishop.obj"
#define QUEEN_MDL   RES_ROOT "queen.obj"
#define KING_MDL    RES_ROOT "king.obj"

#define PAWN_NAME    "PAWN"
#define ROOK_NAME    "ROOK"
#define KNIGHT_NAME  "KNIGHT"
#define BISHOP_NAME  "BISHOP"
#define QUEEN_NAME   "QUEEN"
#define KING_NAME    "KING"

#define PAWN_ID     0x706177
#define ROOK_ID     0x726f6f
#define KNIGHT_ID   0x6b6e69
#define BISHOP_ID   0x626973
#define QUEEN_ID    0x717565
#define KING_ID     0x6b696e

static unistring pawnNameById(uint id)
{
    unistring mdl="err";
    switch (id) {
    case PAWN_ID:
        mdl=PAWN_NAME;
        break;
    case ROOK_ID:
        mdl=ROOK_NAME;
        break;
    case KNIGHT_ID:
        mdl=KNIGHT_NAME;
        break;
    case BISHOP_ID:
        mdl=BISHOP_NAME;
        break;
    case QUEEN_ID:
        mdl=QUEEN_NAME;
        break;
    case KING_ID:
        mdl=KING_NAME;
        break;
    default:
        break;
    }
    return mdl;
}

struct PAI // PawnAdditionalInfo
{
    unistring owner;
    glm::vec3 pos;
    uint mdl_id;
};

struct PINSS // PlayerInfoNetSyncStruct
{
    unistring pname;
    map<unistring, PAI> obj_positions;
};

static const float singleSquare = 0.04233f;

static const map<unistring, unistring> mdl_list = boost::assign::map_list_of(PAWN_NAME, PAWN_MDL)(ROOK_NAME, ROOK_MDL)
                                                                            (KNIGHT_NAME, KNIGHT_MDL)(BISHOP_NAME,BISHOP_MDL)
                                                                            (QUEEN_NAME, QUEEN_MDL)(KING_NAME, KING_MDL);
#endif // MULTICHESS_H
