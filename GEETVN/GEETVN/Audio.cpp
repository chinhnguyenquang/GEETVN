#include "Audio.h"
#include "zero.h"

extern const unsigned char zero_mp3[];
extern const unsigned int zero_mp3_len;

Audio::Audio(int bclk, int ws, int dout, int pin_en)

    : _mp3(zero_mp3, zero_mp3_len),
      _volume(_i2s),
      _out(&_volume, &_decoder),
      _copier(_out, _mp3)

{
    _bclk = bclk;
    _ws = ws;
    _dout = dout;
    _pin_en = pin_en;
}

bool Audio::begin() {

    pinMode(_pin_en, OUTPUT_OPEN_DRAIN);

    // disable amplifier
    digitalWrite(_pin_en, LOW);

    delay(10);

    // I2S config
    auto cfg = _i2s.defaultConfig(TX_MODE);

    cfg.pin_bck  = _bclk;
    cfg.pin_ws   = _ws;
    cfg.pin_data = _dout;

    cfg.sample_rate = 24000;
    cfg.channels = 1;

    // start i2s only once
    if (!_i2s.begin(cfg)) {

        DBG("I2S begin failed");

        return false;
    }

    // default volume
    _volume.setVolume(0.8f);
    
    // start decoder ONLY ONCE
    _out.begin();

    DBG("Audio begin OK");

    return true;
}

void Audio::play() {

    // already playing
    if (_isPlaying) return;

    DBG("Play");

    // reset mp3 stream to beginning
    _mp3.begin();

    // enable amp
    digitalWrite(_pin_en, HIGH);

    _isPlaying = true;
}

void Audio::stop() {

    if (!_isPlaying) return;

    DBG("Stop");

    // disable amp
    digitalWrite(_pin_en, LOW);

    _isPlaying = false;
}

void Audio::update() {

    if (!_isPlaying) return;

    if (_mp3.available()) {

        // decode + output
        _copier.copy();

    } else {

        DBG("End of MP3");

        stop();
    }
}

