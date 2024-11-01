#pragma once
//
//    FILE: HC4067.h
//  AUTHOR: Rob Tillaart
//    DATE: 2023-01-25
// VERSION: 0.3.0
// PURPOSE: Arduino library for CD74HC4067 1 x 16 channel multiplexer and compatibles.
//     URL: https://github.com/RobTillaart/HC4067

#include "Arduino.h"
#include "digitalWriteFast.h"

#define HC4067_LIB_VERSION (F("0.3.0"))

class HC4067
{
public:
  explicit HC4067(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3, uint8_t enablePin = 255)
  {
    _pins[0] = s0;
    _pins[1] = s1;
    _pins[2] = s2;
    _pins[3] = s3;
    uint8_t i = 4;
    while (i--)
    {
      pinModeFast(_pins[i], OUTPUT);
      digitalWriteFast(_pins[i], LOW);
    }
    _channel = 0;

    if (enablePin != 255)
    {
      _enablePin = enablePin;
      pinModeFast(_enablePin, OUTPUT);
      digitalWriteFast(_enablePin, HIGH);
    }
  }

  bool setChannel(uint8_t channel, bool disable = true)
  {
    if (channel > 15)
      return false;
    uint8_t _new = channel;
    if (_new != _channel)
    {
      uint8_t _changed = _new ^ _channel;
      uint8_t mask = 0x08;
      uint8_t i = 3;
      if (disable)
      {
        this->disable(); //  prevent ghost channels.
      }
      while (mask)
      {
        //  only write changed pins. //  AVR only?
        if (mask & _changed)
        {
          digitalWriteFast(_pins[i], (mask & _new));
        }
        i--;
        mask >>= 1;
      }
      enable();
      _channel = _new;
    }
    return true;
  }

  uint8_t getChannel()
  {
    return _channel;
  }

  void enable()
  {
    if (_enablePin != 255)
    {
      digitalWriteFast(_enablePin, LOW);
    }
  }

  void disable()
  {
    if (_enablePin != 255)
    {
      digitalWriteFast(_enablePin, HIGH);
    }
  }

  bool isEnabled()
  {
    if (_enablePin != 255)
    {
      return (digitalReadFast(_enablePin) == LOW);
    }
    return true;
  }

private:
  uint8_t _pins[4];
  uint8_t _enablePin = 255;
  uint8_t _channel = 0;
};

//  -- END OF FILE --
