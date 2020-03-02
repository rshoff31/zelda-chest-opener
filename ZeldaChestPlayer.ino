// Play Zelda Chest opening sounds!
#include "XT_DAC_Audio.h"
#include "Zelda_Item_Fanfare.h"
#include "Zelda_Treasure_Chest.h"
#include "Zelda_Treasure_Chest_Song.h"

// Sound Vars:
XT_Wav_Class ZeldaTreasureChestPlayer(ZeldaTreasureChest);
XT_Wav_Class ZeldaTreasureChestSongPlayer(ZeldaTreasureChestSong);
XT_Wav_Class ZeldaItemFanfarePlayer(ZeldaItemFanfare);

XT_DAC_Audio_Class DacAudio(25,0);  // Create the main player class object. Use GPIO 25, one of the 2 DAC pins and timer 0
XT_Sequence_Class Sequence;  // Sequence player.

// Amplifier Vars:
const uint8_t AMP_EN_PIN = 1;  // ESP32 Breadboard pin 15.

// GPIO IN Vars:
const uint8_t SOUND_SELECT_PIN_0 = 22;  // ESP32 Breadboard pin 14.
const uint8_t SOUND_SELECT_PIN_1 = 23;  // ESP32 Breadboard pin 13.

void setup() {
  // Pin setup:
  pinMode(AMP_EN_PIN, OUTPUT);
  pinMode(SOUND_SELECT_PIN_0, INPUT);
  pinMode(SOUND_SELECT_PIN_1, INPUT);

  digitalWrite(AMP_EN_PIN, LOW); // Turn on amp.
  delay(0.05);

  // Select the clips to play based on the input switch values:
  if (digitalRead(SOUND_SELECT_PIN_1) == LOW && digitalRead(SOUND_SELECT_PIN_0) == LOW)  // 00 - Chest Open Only.
  {
    Sequence.AddPlayItem(&ZeldaTreasureChestPlayer);
  }
  else if (digitalRead(SOUND_SELECT_PIN_1) == LOW && digitalRead(SOUND_SELECT_PIN_0) == HIGH)  // 01 - Fanfare Only.
  {
    Sequence.AddPlayItem(&ZeldaItemFanfarePlayer);
  }
  else if (digitalRead(SOUND_SELECT_PIN_1) == HIGH && digitalRead(SOUND_SELECT_PIN_0) == LOW)  // 10 - Chest Open & Fanfare Combo.
  {
    Sequence.AddPlayItem(&ZeldaTreasureChestPlayer);
    Sequence.AddPlayItem(&ZeldaItemFanfarePlayer);
  }
  else if (digitalRead(SOUND_SELECT_PIN_1) == HIGH && digitalRead(SOUND_SELECT_PIN_0) == HIGH)  // 11 - Chest Open & Song & Fanfare Combo (Full).
  {
    Sequence.AddPlayItem(&ZeldaTreasureChestPlayer);
    Sequence.AddPlayItem(&ZeldaTreasureChestSongPlayer);
    Sequence.AddPlayItem(&ZeldaItemFanfarePlayer);
  }
  else  // Chest open only (Default; maybe in case switch unit falls off).
  {
    Sequence.AddPlayItem(&ZeldaTreasureChestPlayer);
  }

  // PLAY IT:
  DacAudio.Play(&Sequence);

  // Tear Down:
  digitalWrite(AMP_EN_PIN, HIGH); // Turn off amp.
}


void loop() {
  // Just to fill the buffer. needed:
  DacAudio.FillBuffer();
}
