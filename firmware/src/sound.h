#pragma once

#include <M5Stack.h>

#include <AudioFileSourceID3.h>
#include <AudioFileSourceSPIFFS.h>
#include <AudioGeneratorWAV.h>
#include <AudioOutputI2S.h>

#include <driver/dac.h>
#include <driver/i2s.h>

class Sound {
public:
  enum Music {
    MUSIC_BEGIN,
    MUSIC_START,
    MUSIC_GOAL,
    MUSIC_TOUCH,
    MUSIC_EMERGENCY,
    MUSIC_TIMEUP,
    MUSIC_SIZE,
  };
  static const char *getFilePath(const enum Music m) {
    static const char *const filepaths[MUSIC_SIZE] = {
        "/sound/begin.wav", "/sound/start.wav",     "/sound/goal.wav",
        "/sound/touch.wav", "/sound/emergency.wav", "/sound/timeup.wav",
    };
    return filepaths[m];
  }

public:
  Sound() : out(0, AudioOutputI2S::INTERNAL_DAC), id3(&file) {
    playList = xQueueCreate(10, sizeof(enum Music));
  }
  void begin() {
    out.SetOutputModeMono(true);
    out.SetGain(0.2);
    xTaskCreate([](void *obj) { static_cast<Sound *>(obj)->task(); }, "sound",
                8192, this, 2, NULL);
  }
  void play(const enum Music music) { xQueueSendToBack(playList, &music, 0); }

private:
  QueueHandle_t playList;
  AudioGeneratorWAV gen;
  AudioOutputI2S out;
  AudioFileSourceSPIFFS file;
  AudioFileSourceID3 id3;

  void play(const char *filepath) {
    if (gen.isRunning())
      gen.stop();
    file.open(filepath);
    gen.begin(&id3, &out);
    dac_output_enable(DAC_CHANNEL_1);
  }
  void update() {
    if (gen.isRunning() && !gen.loop()) {
      gen.stop();
      i2s_stop(I2S_NUM_0);
      dac_output_disable(DAC_CHANNEL_1);
      out.stop();
    }
  }
  void task() {
    while (1) {
      Music music;
      const auto res = xQueueReceive(playList, &music, pdMS_TO_TICKS(10));
      if (res)
        play(getFilePath(music));
      update();
    }
  }
};
