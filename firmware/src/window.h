#pragma once

#include <M5Stack.h>

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#include <deque>

class Window {
public:
  struct Track {
    int count;
    int time_ms;
    bool valid;
  };
  struct Contents {
    int running_time_ms = 0;
    int remain_time_ms = 0;
    int try_count = 0;
    std::deque<Track> tracks;
  };

public:
  Window() {}
  void begin() { scanLayout(); }
  void update(const Contents &contents, const bool clean = false) {
    if (clean) {
      M5.Lcd.fillScreen(background_color);
      parts_prev.fill(-1);
    }
    drawRunningTime(contents.running_time_ms);
    drawRemainTime(contents.remain_time_ms);
    drawTryCount(contents.try_count);
    drawTimeList(contents.tracks);
  }

protected:
  enum Parts {
    PARTS_RUN_TIME_MM,
    PARTS_RUN_TIME_COLON,
    PARTS_RUN_TIME_SS,
    PARTS_RUN_TIME_DOT,
    PARTS_RUN_TIME_MS,
    PARTS_REM_TIME_MM,
    PARTS_REM_TIME_COLON,
    PARTS_REM_TIME_SS,
    PARTS_TRY_COUNT,
    PARTS_TIME_LIST,
    PARTS_SIZE,
  };

protected:
  std::array<std::pair<int16_t, int16_t>, Parts::PARTS_SIZE> parts_pos;
  std::array<int, Parts::PARTS_SIZE> parts_prev;
  const uint16_t background_color = TFT_BLACK;

  void scanCursor(const enum Parts p) {
    parts_pos[p] = {M5.Lcd.getCursorX(), M5.Lcd.getCursorY()};
  }
  void setCursor(const enum Parts p) const {
    M5.Lcd.setCursor(parts_pos[p].first, parts_pos[p].second);
  }
  void scanLayout() {
    /* font */
    M5.Lcd.fillScreen(background_color);
    M5.Lcd.setTextColor(background_color);
    // M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextFont(7);
    M5.Lcd.setTextSize(1);
    /* Running Time */
    M5.Lcd.setCursor(0, 4);
    M5.Lcd.print(" -");
    scanCursor(PARTS_RUN_TIME_MM);
    M5.Lcd.print("88");
    scanCursor(PARTS_RUN_TIME_COLON);
    M5.Lcd.print(":");
    scanCursor(PARTS_RUN_TIME_SS);
    M5.Lcd.print("88");
    scanCursor(PARTS_RUN_TIME_DOT);
    M5.Lcd.print(".");
    scanCursor(PARTS_RUN_TIME_MS);
    M5.Lcd.print("888\n");
    /* Remain Time */
    M5.Lcd.setCursor(0, M5.Lcd.getCursorY() + 4);
    scanCursor(PARTS_REM_TIME_MM);
    M5.Lcd.print(" -88");
    scanCursor(PARTS_REM_TIME_COLON);
    M5.Lcd.print(":");
    scanCursor(PARTS_REM_TIME_SS);
    M5.Lcd.print("88");
    M5.Lcd.print(" ");
    /* Try Count */
    scanCursor(PARTS_TRY_COUNT);
    M5.Lcd.print("888\n");
    /* Try Times */
    M5.Lcd.setTextFont(4);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(M5.Lcd.getCursorX(), M5.Lcd.getCursorY() + 4);
    scanCursor(PARTS_TIME_LIST);
    const auto x0 = M5.Lcd.getCursorX();
    const auto y0 = M5.Lcd.getCursorY();
    for (int i = 0; i < 10; ++i) {
      const auto x = (i / 5 == 0) ? x0 : M5.Lcd.width() / 2;
      const auto y = (i % 5 == 0) ? y0 : M5.Lcd.getCursorY();
      M5.Lcd.setCursor(x, y);
      M5.Lcd.print("88  88:88.888\n");
    }
  }
  void drawRunningTime(const int time_ms) {
    const auto textcolor = TFT_BLUE;
    M5.Lcd.setTextFont(7);
    M5.Lcd.setTextSize(1);
    const auto mm = (time_ms / 1000 / 60) % 60;
    const auto ss = (time_ms / 1000) % 60;
    const auto ms = (time_ms % 1000);
    const auto draw_colon = true;
    /* minute */
    if (parts_prev[PARTS_RUN_TIME_MM] != mm) {
      parts_prev[PARTS_RUN_TIME_MM] = mm;
      /* background */
      setCursor(PARTS_RUN_TIME_MM);
      M5.Lcd.setTextColor(0x1903);
      M5.Lcd.print("88");
      /* foreground */
      setCursor(PARTS_RUN_TIME_MM);
      if (mm < 10)
        M5.Lcd.print("8");
      M5.Lcd.setTextColor(textcolor);
      M5.Lcd.printf("%d", mm);
    }
    /* colon */
    if (parts_prev[PARTS_RUN_TIME_COLON] != draw_colon) {
      parts_prev[PARTS_RUN_TIME_COLON] = draw_colon;
      /* background */
      setCursor(PARTS_RUN_TIME_COLON);
      M5.Lcd.setTextColor(draw_colon ? textcolor : 0x1903);
      M5.Lcd.print(":");
    }
    /* second */
    if (parts_prev[PARTS_RUN_TIME_SS] != ss) {
      parts_prev[PARTS_RUN_TIME_SS] = ss;
      /* background */
      setCursor(PARTS_RUN_TIME_SS);
      M5.Lcd.setTextColor(0x1903);
      M5.Lcd.print("88");
      /* foreground */
      setCursor(PARTS_RUN_TIME_SS);
      M5.Lcd.setTextColor(textcolor);
      M5.Lcd.printf("%02d", ss);
    }
    /* dot */
    if (parts_prev[PARTS_RUN_TIME_DOT] != draw_colon) {
      parts_prev[PARTS_RUN_TIME_DOT] = draw_colon;
      /* background */
      setCursor(PARTS_RUN_TIME_DOT);
      M5.Lcd.setTextColor(draw_colon ? textcolor : 0x1903);
      M5.Lcd.print(".");
    }
    /* millisecond */
    if (parts_prev[PARTS_RUN_TIME_MS] != ms / 89) {
      parts_prev[PARTS_RUN_TIME_MS] = ms / 89;
      /* background */
      setCursor(PARTS_RUN_TIME_MS);
      M5.Lcd.setTextColor(0x1903);
      M5.Lcd.print("888");
      /* foreground */
      setCursor(PARTS_RUN_TIME_MS);
      M5.Lcd.setTextColor(textcolor);
      M5.Lcd.printf("%03d", ms);
    }
  }
  void drawRemainTime(const int time_ms) {
    const auto textcolor = TFT_GREEN;
    M5.Lcd.setTextFont(7);
    M5.Lcd.setTextSize(1);
    const auto abs_time_ms = std::abs(time_ms);
    const bool negative = (time_ms < 0);
    const auto mm = (abs_time_ms / 1000 / 60) % 60;
    const auto ss = (abs_time_ms / 1000) % 60;
    const auto ms = abs_time_ms % 1000;
    const auto draw_colon = (ms > 500) ^ negative;
    /* minute */
    if (parts_prev[PARTS_REM_TIME_MM] != mm ||
        (parts_prev[PARTS_REM_TIME_SS] != ss && negative && ss == 1)) {
      parts_prev[PARTS_REM_TIME_MM] = mm;
      /* background */
      setCursor(PARTS_REM_TIME_MM);
      M5.Lcd.setTextColor(background_color);
      M5.Lcd.print(" -");
      M5.Lcd.setTextColor(0x1903);
      M5.Lcd.print("88");
      /* foreground */
      setCursor(PARTS_REM_TIME_MM);
      if (negative) {
        M5.Lcd.setTextColor(textcolor);
        M5.Lcd.print(" -");
        M5.Lcd.printf("%02d", mm);
      } else {
        M5.Lcd.setTextColor(background_color);
        M5.Lcd.print(" -");
        M5.Lcd.setTextColor(0x1903);
        if (mm < 10)
          M5.Lcd.print("8");
        M5.Lcd.setTextColor(textcolor);
        M5.Lcd.printf("%d", mm);
      }
    }
    /* colon */
    if (parts_prev[PARTS_REM_TIME_COLON] != draw_colon) {
      parts_prev[PARTS_REM_TIME_COLON] = draw_colon;
      /* background */
      setCursor(PARTS_REM_TIME_COLON);
      M5.Lcd.setTextColor(draw_colon ? TFT_GREEN : 0x1903);
      M5.Lcd.print(":");
    }
    /* second */
    if (parts_prev[PARTS_REM_TIME_SS] != ss) {
      parts_prev[PARTS_REM_TIME_SS] = ss;
      /* background */
      setCursor(PARTS_REM_TIME_SS);
      M5.Lcd.setTextColor(0x1903);
      M5.Lcd.print("88");
      /* foreground */
      setCursor(PARTS_REM_TIME_SS);
      M5.Lcd.setTextColor(TFT_GREEN);
      M5.Lcd.printf("%02d", ss);
    }
  }
  void drawTryCount(const int try_count) {
    M5.Lcd.setTextFont(7);
    M5.Lcd.setTextSize(1);
    /* try count */
    if (parts_prev[PARTS_TRY_COUNT] != try_count) {
      parts_prev[PARTS_TRY_COUNT] = try_count;
      /* background */
      setCursor(PARTS_TRY_COUNT);
      M5.Lcd.setTextColor(0x1903);
      M5.Lcd.print("888");
      /* foreground */
      setCursor(PARTS_TRY_COUNT);
      if (try_count < 100)
        M5.Lcd.print("8");
      if (try_count < 10)
        M5.Lcd.print("8");
      M5.Lcd.setTextColor(TFT_ORANGE);
      M5.Lcd.printf("%d", try_count % 1000);
    }
  }
  void drawTimeList(const std::deque<Track> &tracks) {
    if (tracks.empty()) {
      /* erase when 1 -> 0 tracks */
      if (parts_prev[PARTS_TIME_LIST] != -1) {
        parts_prev[PARTS_TIME_LIST] = -1;
        setCursor(PARTS_TIME_LIST);
        M5.Lcd.fillRect(M5.Lcd.getCursorX(), M5.Lcd.getCursorY(),
                        M5.Lcd.width(), M5.Lcd.height(), background_color);
      }
      return;
    }
    if (parts_prev[PARTS_TIME_LIST] == tracks.back().count)
      return;
    parts_prev[PARTS_TIME_LIST] = tracks.back().count;
    /* erase */
    setCursor(PARTS_TIME_LIST);
    M5.Lcd.fillRect(M5.Lcd.getCursorX(), M5.Lcd.getCursorY(), M5.Lcd.width(),
                    M5.Lcd.height(), background_color);
    /* draw */
    M5.Lcd.setTextFont(4);
    M5.Lcd.setTextSize(1);
    setCursor(PARTS_TIME_LIST);
    const auto x0 = M5.Lcd.getCursorX();
    const auto y0 = M5.Lcd.getCursorY();
    int min_track = -1;
    int min_time_ms = INT_MAX;
    for (const auto track : tracks)
      if (track.time_ms < min_time_ms && track.valid) {
        min_time_ms = track.time_ms;
        min_track = track.count;
      }
    /* show last 10 elements */
    for (int i = 0; i < std::min(10, (int)tracks.size()); ++i) {
      const auto track = tracks[std::max(0, (int)tracks.size() - 10) + i];
      const auto time_ms = track.time_ms;
      const auto mm = (time_ms / 1000 / 60) % 60;
      const auto ss = (time_ms / 1000) % 60;
      const auto ms = time_ms % 1000;
      const auto x = (i / 5 == 0) ? x0 : M5.Lcd.width() / 2;
      const auto y = (i % 5 == 0) ? y0 : M5.Lcd.getCursorY();
      M5.Lcd.setCursor(x, y);
      M5.Lcd.setTextColor(background_color);
      if (track.count % 100 < 10)
        M5.Lcd.print("8");
      M5.Lcd.setTextColor(TFT_MAGENTA);
      M5.Lcd.printf("%d. ", track.count % 100);
      if (time_ms > 0) {
        M5.Lcd.setTextColor(
            track.valid ? (track.count == min_track ? TFT_YELLOW : TFT_WHITE)
                        : TFT_RED);
        M5.Lcd.printf("%02d:%02d.%03d\n", mm, ss, ms);
      } else {
        M5.Lcd.setTextColor(background_color);
        M5.Lcd.print("88::");
        M5.Lcd.setTextColor(TFT_RED);
        M5.Lcd.print("R\n");
      }
    }
  }
};
