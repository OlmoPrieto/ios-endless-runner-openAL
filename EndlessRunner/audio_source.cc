//
//  audio_source.cpp
//  EndlessRunner
//
//  Created by Olmo on 24/11/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

#include <iostream>
#include "audio_source.h"
#include "support.h"
#include "utils.h"

AudioSource::AudioSource() {
        buffer_id = 0;
        source_id = 0;
        memset(pos, 0.0f, sizeof(float)* 3);
        memset(speed, 0.0f, sizeof(float)* 3);
        pitch = 0.0f;
        gain = 0;
        loop = 0;
        channels = 0;
        sample_rate = 0;
        size = 0;
        data = NULL;
        paused = false;
        stoped = true;
    }

    void AudioSource::init(float pos_x, float pos_y, float pos_z, float speed_x, float speed_y, float speed_z,
              float _pitch, float _gain, char _loop) {
        pos[0] = pos_x; pos[1] = pos_y, pos[2] = pos_z;
        speed[0] = speed_x; speed[1] = speed_y; speed[2] = speed_z;
        pitch = _pitch;
        gain = _gain;
        loop = _loop;

        genBuffer();
        genSource();
    }

    void AudioSource::setPos(float x, float y, float z) {
        pos[0] = x; pos[1] = y; pos[2] = z;
    }

    void AudioSource::setSpeed(float x, float y, float z) {
        speed[0] = x; speed[1] = y; speed[2] = z;
    }

    bool AudioSource::loadAudio(const char *path) {
        ALsizei outDataSize;
        ALenum outDataFormat;
        ALsizei outSampleRate;
        data = (short*)support::LoadOpenALAudioData(path, &outDataSize,
                                                    &outDataFormat, &outSampleRate);
        utils::LogDebug("DEBUG", "\n\nPath: %s\n\n", path);

        size = outDataSize;
        if (outDataFormat == AL_FORMAT_MONO16) {
            channels = 1;
        } else if (outDataFormat == AL_FORMAT_STEREO16){
            channels = 2;
        } else {
            printf("Unsupported format\n");
        }
        sample_rate = outSampleRate;
        if (size > 0) {
            printf("Audio loaded\n");
            return true;
        } else {
            printf("Failed to load source\n");
            return false;
        }
    }

    void AudioSource::genBuffer() {
        ALenum format;
        if (channels == 1) {
            format = AL_FORMAT_MONO16;
        }
        else if (channels == 2) {
            format = AL_FORMAT_STEREO16;
        }
        alGenBuffers(1, &buffer_id);
        alBufferData(buffer_id, format, (void*)data, size * 2 * channels, sample_rate);
    }

    void AudioSource::genSource() {
        alGenSources(1, &source_id);
        alSourcei(source_id, AL_BUFFER, buffer_id);
        alSourcef(source_id, AL_PITCH, pitch);
        alSourcef(source_id, AL_GAIN, gain);
        alSourcefv(source_id, AL_POSITION, pos);
        alSourcefv(source_id, AL_VELOCITY, speed);
        alSourcei(source_id, AL_LOOPING, loop);
    }
    
    void AudioSource::play() {
        alSourcePlay(source_id);
        paused = false;
        stoped = false;
    }
    
    void AudioSource::pause() {
        alSourcePause(source_id);
        paused = true;
    }

    void AudioSource::stop() {
        alSourceStop(source_id);
        stoped = true;
    }

    void AudioSource::destroy() {
        alDeleteBuffers(1, &buffer_id);
        alDeleteSources(1, &source_id);
    }

    bool AudioSource::isPlaying() {
        return !stoped;
    }

AudioListener::AudioListener() {

}

void AudioListener::init(float pos_x, float pos_y, float pos_z,
                         float vel_x, float vel_y, float vel_z) {
    float pos[3] = { pos_x, pos_y, pos_z };
    float vel[3] = { vel_x, vel_y, vel_z };
    float ori[6] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };

    alListenerfv(AL_POSITION, pos);
    alListenerfv(AL_VELOCITY, vel);
    alListenerfv(AL_ORIENTATION, ori);
}