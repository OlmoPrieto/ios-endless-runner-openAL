//
//  audio_source.h
//  EndlessRunner
//
//  Created by Olmo on 24/11/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

#ifndef audio_source_h
#define audio_source_h

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

struct AudioSource {
private:
    ALuint buffer_id;
    ALuint source_id;
    float pos[3];
    float speed[3];
    float pitch;
    float gain;
    char loop;

    int channels;
    int sample_rate;
    int size;
    short *data;

    bool paused;
    bool stoped;

public:
    AudioSource();
    void init(float pos_x, float pos_y, float pos_z, float speed_x, float speed_y, float speed_z,
              float _pitch, float _gain, char _loop);
    void setPos(float x, float y, float z);
    void setSpeed(float x, float y, float z);
    bool loadAudio(const char *path);
    void genBuffer();
    void genSource();
    void play();
    void pause();
    void stop();
    void destroy();

    bool isPlaying();
};

struct AudioListener {
private:
    float pos[3];
    float vel[3];
    float ori[3];

public:
    AudioListener();
    void init(float pos_x, float pos_y, float pos_z,
              float vel_x, float vel_y, float vel_z);
};

#endif /* audio_source_h */
