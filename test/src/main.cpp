// Waveform Example - Create 2 waveforms with adjustable
// frequency and phase
//
// This example is meant to be used with 3 buttons (pin 0,
// 1, 2) and 2 knobs (pins 16/A2, 17/A3), which are present
// on the audio tutorial kit.
//   https://www.pjrc.com/store/audio_tutorial_kit.html
//
// Use an oscilloscope to view the 2 waveforms.
//
// Button0 changes the waveform shape
//
// Knob A2 changes the frequency of both waveforms
//   You should see both waveforms "stretch" as you turn
//
// Knob A3 changes the phase of waveform #1
//   You should see the waveform shift horizontally
//
// This example code is in the public domain.

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>

// GUItool: begin automatically generated code
AudioSynthWaveform noteC;         // xy=90.1999397277832,69.19999122619629
AudioSynthWaveform noteD;         // xy=92.19989395141602,158.20000648498535
AudioSynthWaveform noteE;         // xy=91.99997329711914,253.19997787475586
AudioSynthWaveform noteCsharp;    // xy=102.19992446899414,112.20000457763672
AudioSynthWaveform noteDsharp;    // xy=103.1999397277832,207.19997596740723
AudioEffectEnvelope envelope2;    // xy=265.19994354248047,158.2000217437744
AudioEffectEnvelope envelope3;    // xy=265.1999740600586,207.1999454498291
AudioEffectEnvelope envelope4;    // xy=265.1999816894531,253.19998741149902
AudioEffectEnvelope envelope0;    // xy=266.1999740600586,69.19999599456787
AudioEffectEnvelope envelope1;    // xy=266.1999740600586,112.19999694824219
AudioMixer4 mixer1;               // xy=482.19997787475586,216.19997596740723
AudioMixer4 mixer0;               // xy=483.19997787475586,128.1999912261963
AudioSynthWaveformSine lfo;       // xy=610.2000350952148,173.20001411437988
AudioFilterStateVariable filter1; // xy=746.2000389099121,134.1999912261963
AudioFilterStateVariable filter2; // xy=746.2000389099121,222.20000076293945
AudioMixer4 mixerLelf;            // xy=921.2000427246094,128.20001602172852
AudioMixer4 mixerRight;           // xy=929.2000427246094,225.20000076293945
AudioOutputI2S audioOut;          // xy=1095.2000427246094,171.19999980926514
AudioConnection patchCord1(noteC, envelope0);
AudioConnection patchCord2(noteD, envelope2);
AudioConnection patchCord3(noteE, envelope4);
AudioConnection patchCord4(noteCsharp, envelope1);
AudioConnection patchCord5(noteDsharp, envelope3);
AudioConnection patchCord6(envelope2, 0, mixer0, 2);
AudioConnection patchCord7(envelope3, 0, mixer0, 3);
AudioConnection patchCord8(envelope4, 0, mixer1, 0);
AudioConnection patchCord9(envelope0, 0, mixer0, 0);
AudioConnection patchCord10(envelope1, 0, mixer0, 1);
AudioConnection patchCord11(mixer1, 0, filter2, 0);
AudioConnection patchCord12(mixer0, 0, filter1, 0);
AudioConnection patchCord13(lfo, 0, filter1, 1);
AudioConnection patchCord14(lfo, 0, filter2, 1);
AudioConnection patchCord15(filter1, 0, mixerLelf, 0);
AudioConnection patchCord16(filter1, 1, mixerLelf, 1);
AudioConnection patchCord17(filter1, 2, mixerLelf, 2);
AudioConnection patchCord18(filter2, 0, mixerRight, 0);
AudioConnection patchCord19(filter2, 1, mixerRight, 1);
AudioConnection patchCord20(filter2, 2, mixerRight, 2);
AudioConnection patchCord21(mixerLelf, 0, audioOut, 0);
AudioConnection patchCord22(mixerRight, 0, audioOut, 1);
AudioControlSGTL5000 codec; // xy=322.1999702453613,648.2000341415405
// GUItool: end automatically generated code

#define NUM_NOTES 5
AudioSynthWaveform *notes[NUM_NOTES] = {&noteC, &noteCsharp, &noteD, &noteDsharp, &noteE};
AudioEffectEnvelope *envs[NUM_NOTES] = {&envelope0, &envelope1, &envelope2, &envelope3, &envelope4};

#define KNOB_VOLUME_PIN A10
#define KNOB_LFO_FM_PIN A11
#define KNOB_ENVELOP_ATTACK_PIN A17
#define KNOB_ENVELOP_DECAY_PIN A16
#define KNOB_ENVELOP_SUSTAIN_PIN A15
#define KNOB_ENVELOP_RELEASE_PIN A14

#define BUTTON_ON_OFF_PIN 35
#define BUTTON_SINE_WAVE_PIN 26
#define BUTTON_SQUARE_WAVE_PIN 27
#define BUTTON_SAWTOOTH_WAVE_PIN 28
#define BUTTON_TRIANGLE_WAVE_PIN 29
#define BUTTON_NOTE_C_PIN 34
#define BUTTON_NOTE_C_SHARP_PIN 33
#define BUTTON_NOTE_D_PIN 30
#define BUTTON_NOTE_D_SHARP_PIN 31
#define BUTTON_NOTE_E_PIN 32

Bounce buttonOnOff = Bounce(BUTTON_ON_OFF_PIN, 15);
Bounce buttonSineWave = Bounce(BUTTON_SINE_WAVE_PIN, 15);
Bounce buttonSquareWave = Bounce(BUTTON_SQUARE_WAVE_PIN, 15);
Bounce buttonSawtoothWave = Bounce(BUTTON_SAWTOOTH_WAVE_PIN, 15);
Bounce buttonTriangleWave = Bounce(BUTTON_TRIANGLE_WAVE_PIN, 15);

Bounce buttonNoteC = Bounce(BUTTON_NOTE_C_PIN, 15);
Bounce buttonNoteCsharp = Bounce(BUTTON_NOTE_C_SHARP_PIN, 15);
Bounce buttonNoteD = Bounce(BUTTON_NOTE_D_PIN, 15);
Bounce buttonNoteDsharp = Bounce(BUTTON_NOTE_D_SHARP_PIN, 15);
Bounce buttonNoteE = Bounce(BUTTON_NOTE_E_PIN, 15);

Bounce *keys[NUM_NOTES] = {&buttonNoteC, &buttonNoteCsharp, &buttonNoteD, &buttonNoteDsharp, &buttonNoteE};

#define NOTE_C_FREQUENCY 261.63
#define NOTE_C_SHARP_FREQUENCY 277.18
#define NOTE_D_FREQUENCY 293.66
#define NOTE_D_SHARP_FREQUENCY 311.13
#define NOTE_E_FREQUENCY 329.63

int current_waveform = 0;
bool on_off = true;

void changeWaveform(int waveform)
{
  for (int i = 0; i < NUM_NOTES; i++)
  {
    notes[i]->begin(waveform);
  }
}

void changeEnvelope(float attack, float decay, float sustain, float release)
{
  for (int i = 0; i < NUM_NOTES; i++)
  {
    envs[i]->attack(attack * 11880);
    envs[i]->decay(decay * 11880);
    envs[i]->sustain(sustain);
    envs[i]->release(release * 11880);
  }
}

void playNotes()
{
  for (int i = 0; i < NUM_NOTES; i++)
  {
    if (keys[i]->fallingEdge())
    {
      envs[i]->noteOn();
    }
    else if (keys[i]->risingEdge())
    {
      envs[i]->noteOff();
    }
  }
}

void setup()
{
  Serial.begin(115200);
  // Wait for USB Serial
  delay(200);

  pinMode(BUTTON_ON_OFF_PIN, INPUT_PULLUP);
  pinMode(BUTTON_SINE_WAVE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_SQUARE_WAVE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_SAWTOOTH_WAVE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_TRIANGLE_WAVE_PIN, INPUT_PULLUP);

  pinMode(BUTTON_NOTE_C_PIN, INPUT_PULLUP);
  pinMode(BUTTON_NOTE_C_SHARP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_NOTE_D_PIN, INPUT_PULLUP);
  pinMode(BUTTON_NOTE_D_SHARP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_NOTE_E_PIN, INPUT_PULLUP);

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(20);
  codec.enable();

  // Set the volume
  float volume = (float)map(analogRead(KNOB_VOLUME_PIN) / 1023.0, 0, 1, 0.02, 1);
  codec.volume(volume);

  // Set the envelope
  float attack = (float)analogRead(KNOB_ENVELOP_ATTACK_PIN) / 1023.0;
  float decay = (float)analogRead(KNOB_ENVELOP_DECAY_PIN) / 1023.0;
  float sustain = (float)analogRead(KNOB_ENVELOP_SUSTAIN_PIN) / 1023.0;
  float release = (float)analogRead(KNOB_ENVELOP_RELEASE_PIN) / 1023.0;
  changeEnvelope(attack, decay, sustain, release);

  // set the mixers
  mixer0.gain(0, .8);
  mixer0.gain(1, .8);
  mixer0.gain(2, .8);
  mixer0.gain(3, .8);
  mixer1.gain(0, .8);
  mixerLelf.gain(0, 1.0);
  mixerLelf.gain(1, 1.0);
  mixerRight.gain(0, 1.0);
  mixerRight.gain(1, 1.0);

  // Set the LFO
  float lfo_fm = (float)map(analogRead(KNOB_LFO_FM_PIN) / 1023.0, 0, 1, 0, 20);
  lfo.frequency(lfo_fm);
  lfo.amplitude(1.0);

  // Set the filters
  filter1.frequency(200);
  filter1.resonance(0.5);
  filter2.frequency(200);
  filter2.resonance(0.5);

  // Set the waveform
  noteC.frequency(NOTE_C_FREQUENCY);
  noteC.amplitude(1.0);

  noteCsharp.frequency(NOTE_C_SHARP_FREQUENCY);
  noteCsharp.amplitude(1.0);

  noteD.frequency(NOTE_D_FREQUENCY);
  noteD.amplitude(1.0);

  noteDsharp.frequency(NOTE_D_SHARP_FREQUENCY);
  noteDsharp.amplitude(1.0);

  noteE.frequency(NOTE_E_FREQUENCY);
  noteE.amplitude(1.0);

  current_waveform = WAVEFORM_SINE;
  noteC.begin(current_waveform);
  noteCsharp.begin(current_waveform);
  noteD.begin(current_waveform);
  noteDsharp.begin(current_waveform);
  noteE.begin(current_waveform);
}

void loop()
{
  // Read the buttons
  buttonOnOff.update();
  buttonSineWave.update();
  buttonSquareWave.update();
  buttonSawtoothWave.update();
  buttonTriangleWave.update();

  buttonNoteC.update();
  buttonNoteCsharp.update();
  buttonNoteD.update();
  buttonNoteDsharp.update();
  buttonNoteE.update();

  on_off = buttonOnOff.read();

  if (buttonSineWave.fallingEdge())
  {
    current_waveform = WAVEFORM_SINE;
    changeWaveform(current_waveform);
    Serial.println("Sine");
  }
  else if (buttonSquareWave.fallingEdge())
  {
    current_waveform = WAVEFORM_SQUARE;
    changeWaveform(current_waveform);
    Serial.println("Square");
  }
  else if (buttonSawtoothWave.fallingEdge())
  {
    current_waveform = WAVEFORM_SAWTOOTH;
    changeWaveform(current_waveform);
    Serial.println("Sawtooth");
  }
  else if (buttonTriangleWave.fallingEdge())
  {
    current_waveform = WAVEFORM_TRIANGLE;
    changeWaveform(current_waveform);
    Serial.println("Triangle");
  }

  // Volume and pitch are read
  // Serial.println("--------------------  Start  --------------------");
  float volume = (float)map(analogRead(KNOB_VOLUME_PIN) / 1023.0, 0, 1, 0.02, 1);
  // Serial.print("Volume: ");
  // Serial.println(volume);
  float lfo_fm = (float)map(analogRead(KNOB_LFO_FM_PIN) / 1023.0, 0, 1, 0.02, 20);
  // Serial.print("FM: ");
  // Serial.println(fm);

  // Emvelope controlls are read
  float attack = (float)analogRead(KNOB_ENVELOP_ATTACK_PIN) / 1023.0;
  // Serial.print("Attack: ");
  // Serial.println(attack);
  float decay = (float)analogRead(KNOB_ENVELOP_DECAY_PIN) / 1023.0;
  // Serial.print("Decay: ");
  // Serial.println(decay);
  float sustain = (float)analogRead(KNOB_ENVELOP_SUSTAIN_PIN) / 1023.0;
  // Serial.print("Sustain: ");
  // Serial.println(sustain);
  float release = (float)analogRead(KNOB_ENVELOP_RELEASE_PIN) / 1023.0;
  // Serial.print("Release: ");
  // Serial.println(release);
  // Serial.println("--------------------  End  --------------------");

  AudioNoInterrupts();
  on_off ? codec.unmuteHeadphone() : codec.muteHeadphone();

  codec.volume(volume);
  changeEnvelope(attack, decay, sustain, release);
  // envelope.attack(attack * 11880);
  // envelope.decay(decay * 11880);
  // envelope.sustain(sustain);
  // envelope.release(release * 11880);
  lfo.frequency(lfo_fm);
  // filter1.frequency(lfo_fm * 100);
  // filter2.frequency(lfo_fm * 100);
  playNotes();
  AudioInterrupts();
}
