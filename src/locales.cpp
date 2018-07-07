#include "include/locales.h"

Locales::Locales()
{

}

void Locales::init(unistring lang)
{
    curTr = a_translations.at(lang);
}

const unistring &Locales::getString(unistring tid) const
{
    return curTr.words.at(tid);
}

const unistring &Locales::getString(unistring lang, unistring tid) const
{
    return a_translations.at(lang).words.at(tid);
}
