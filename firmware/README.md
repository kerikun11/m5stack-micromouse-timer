# firmware

## ハードウェア

- [M5Stack GREY](https://m5stack.com/products/grey-development-core)
- (M5Stack Basic)
- (M5Stack Fire)

---

## 開発環境

- [PlatformIO](https://platformio.org/)
    - VSCode の拡張機能をインストールするだけで開発可能！

### 設定

設定ファイル `platformio.ini` でシリアルポートを設定する (例: `/dev/ttyUSB0`, `COM3`)

```ini
upload_port = /dev/ttyUSB0
monitor_port = /dev/ttyUSB0
```

### 実行例

```sh
# firmware をビルドする (VSCode: Ctrl+Alt+B)
platformio run
# 効果音ファイルを SPIFFS に書き込む (VSCode: ショートカットキーなし...)
platformio run -t uploadfs
# firmware を書き込む (VSCode: Ctrl+Alt+U)
platformio run -t upload
```

---

## 効果音ファイル

効果音ファイルは次のパスに配置してSPIFFS(内蔵ファイルシステム)にアップロードしている．  
ファイルがない場合は音がならないが，場合タイマーなどは動作する．

| 効果音       | パス                       |
| ------------ | -------------------------- |
| 競技開始     | `data/sound/begin.wav`     |
| スタート     | `data/sound/start.wav`     |
| ゴール       | `data/sound/goal.wav`      |
| タイムアップ | `data/sound/timeup.wav`    |
| リトライ     | `data/sound/touch.wav`     |
| エラー       | `data/sound/emergency.wav` |
