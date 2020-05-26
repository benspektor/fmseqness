/*
  ==============================================================================

    Constants.h
    Created: 4 Feb 2020 12:54:47pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

typedef AudioProcessorValueTreeState::SliderAttachment   SliderAttachment;
typedef AudioProcessorValueTreeState::ButtonAttachment   ButtonAttachment;
typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

const float PADDING = 10.0f;
const float SELECTOR_HEIGHT = 50.0F;
const float GATE_EDITOR_HEIGHT = 60.0f;
const float PITCH_LINE_THIKNESS = 3;

const int TOTAL_NUMBER_OF_STEPS = 32;
const int PIANO_ROLL_LENGTH = 25;
const int OCTAVE = 12;
const int QUARTER_DIVEDER = 4;

constexpr int BASE_PITCH = 60;
constexpr int MINIMUM_PITCH = 36;
constexpr int MAXIMUM_PITCH = 96;

constexpr int MIN_NUM_OF_STEPS = 4;
constexpr int MAX_NUM_OF_STEPS = 32;
constexpr int DEF_NUM_OF_STEPS = 16;

constexpr float MILLISECONDS_IN_SECOND = 1000.0;

constexpr float RESTART_TIME = 0.01;

constexpr float FM_AMNT_MIN_VALUE = 0.0;
constexpr float FM_AMNT_MAX_VALUE = 8.0;
constexpr float FM_AMNT_SKEW_VALUE = 0.4;
constexpr float FM_AMNT_SET_VALUE = 0.1;

constexpr float MOD_MULTI_MIN_VALUE = 0.25;
constexpr float MOD_MULTI_MAX_VALUE = 8.0;
constexpr float MOD_MULTI_SKEW_VALUE = 0.5;
constexpr float MOD_MULTI_SET_VALUE = 0.5;

constexpr int TEMPO_MIN_VALUE = 1;
constexpr int TEMPO_MAX_VALUE = 200;
constexpr int TEMPO_SKEW_VALUE = 1.0;
constexpr int TEMPO_SET_VALUE = 120;

constexpr float ATTACK_KNOB_MIN_VALUE = 10.0;
constexpr float ATTACK_KNOB_MAX_VALUE = 1000.0;
constexpr float ATTACK_KNOB_SKEW_VALUE = 0.5;
constexpr float ATTACK_KNOB_SET_VALUE = 10.0;

constexpr float HOLD_KNOB_MIN_VALUE = 0.0;
constexpr float HOLD_KNOB_MAX_VALUE = 3000.0;
constexpr float HOLD_KNOB_SKEW_VALUE = 0.5;
constexpr float HOLD_KNOB_SET_VALUE = 10.0;

constexpr float DECAY_KNOB_MIN_VALUE = 10.0;
constexpr float DECAY_KNOB_MAX_VALUE = 3000.0;
constexpr float DECAY_KNOB_SKEW_VALUE = 0.5;
constexpr float DECAY_KNOB_SET_VALUE = 300.0;

constexpr float FM_CONTROL_MIN_VALUE = 0.0;
constexpr float FM_CONTROL_MAX_VALUE = 16.0;
constexpr float FM_CONTROL_SKEW_VALUE = 1.0;
constexpr float FM_CONTROL_SET_VALUE = 1.0;

constexpr float SWING_MIN_VALUE = 1.0;
constexpr float SWING_MAX_VALUE = 1.75;
constexpr float SWING_DOT_VALUE = 1.5;
constexpr float SWING_TRIPLET_VALUE = 1.3333333334;

//constexpr double FM_CONTROL_MIN_VALUE = 0.0;
//constexpr double FM_CONTROL_MAX_VALUE = 1.0;
//constexpr double FM_CONTROL_SKEW_VALUE = 1.0;
//constexpr double FM_CONTROL_SET_VALUE = 0.0;

constexpr int MIN_PITCH_NOTE = 24;
constexpr int MAX_PITCH_NOTE = 120;
constexpr int RESET_PITCH_VALUE = 48;

constexpr int HARD_CODED_NOTES[32] = {48,60,72,67,65,60,48,72,48,60,72,67,65,60,48,72,48,60,72,67,65,60,48,72,48,60,72,67,65,60,48,72};

 
constexpr float SEQUENCER_PANEL_HEIGHT = 80;
constexpr float ENVELOPE_WINDOW_HEIGHT = 180;
constexpr float MOD_MATRIX_HEIGHT      = 60;

//constexpr float SWING_VALUE = 1.5f;

const StringArray LFO_SHAPES              { "Saw", "Sine", "Triangle", "Square" };
const StringArray LFO_POLARITIES          { "Bi-Polar", "Positive", "Negative" };
const StringArray LFO_RESTART_OPTIONS     { "Never", "Step", "Pattern" };
const StringArray MODULATION_SOURCES      { "Mod Env", "LFO", "Mod Seq" };
const StringArray MODULATION_DESTINATIONS { "FM", "Pitch", "Pan", "Volume", "Mod Multi", "Portamento", "Swing" };


