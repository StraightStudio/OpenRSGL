#include <pawn.h>

Pawn::Pawn()
{

}

const unistring Pawn::getOwner() const
{
    return owner;
}

uint Pawn::mdl()
{
    return mdl_id;
}
