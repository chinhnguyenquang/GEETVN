#pragma once

#include <Arduino.h>

#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "IotVision.h"

class Audio {

public:

    Audio(int bclk, int ws, int dout, int pin_en=13);

    bool begin();

    void play();

    void stop();

    void update();

    

private:

    int _bclk;
    int _ws;
    int _dout;
    int _pin_en;

    bool _isPlaying = false;
    MemoryStream _mp3;

    I2SStream _i2s;

    MP3DecoderHelix _decoder;

    EncodedAudioStream _out;

    StreamCopy _copier;

    VolumeStream _volume;
};