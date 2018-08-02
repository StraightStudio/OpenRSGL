#ifndef PAWN_H
#define PAWN_H

#include <depends.h>
#include <object3d.h>

#include <multichess.h>

class Pawn
{
public:
    Pawn();
    const unistring getOwner() const;
    uint mdl();
private:
    unistring owner;
    uint mdl_id;
};

#endif // PAWN_H
