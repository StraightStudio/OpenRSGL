#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <include/depends.h>
#include <include/logger.h>

class AudioManager
{
public:
    AudioManager();
    ~AudioManager();

    void init();

    void loadSounds(AppConfig &conf);
    void loadMusic(AppConfig &conf);

    void playSound(QString sound);

    void playMusic(QString track);
    void playMusic(QString track, bool looped);
    void stopMusic();
private:
    QMap <QString, Mix_Chunk* > a_sounds;
    QMap <QString, Mix_Music* > a_music;
};

#endif // AUDIOMANAGER_H
