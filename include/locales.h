#ifndef LOCALES_H
#define LOCALES_H

#include <include/depends.h>

struct Translation
{
    unistring name; // LANG, (english;russian) ASCII
    map<unistring, unistring> words; // Translation list. UTF-8
};

class Locales
{
public:
    Locales();

    void init(unistring lang);

    const unistring &getString(unistring tid) const;
    const unistring &getString(unistring lang, unistring tid) const; // LANG - language (russian, english); TID - Text-ID of word (Usually ASCII) .

    map<unistring, Translation> a_translations;
    Translation curTr;
};

#endif // LOCALES_H
