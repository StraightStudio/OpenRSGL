
#ifdef _WIN64
#include <locales.h>
#else 
#include <include/locales.h>
#endif


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
