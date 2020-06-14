/*
  ==============================================================================

    Constants.h
    Created: 4 Feb 2020 12:54:47pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#define GATE_ON    0
#define GATE_GLIDE 1
#define GATE_OFF   2

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
const int SIXTEEN_DIVEDER = 4;

constexpr int BASE_PITCH    = 48;
constexpr int MINIMUM_PITCH = 36;
constexpr int MAXIMUM_PITCH = 96;

constexpr int MIN_NUM_OF_STEPS = 4;
constexpr int MAX_NUM_OF_STEPS = 32;
constexpr int DEF_NUM_OF_STEPS = 16;

constexpr float MILLISECONDS_IN_SECOND = 1000.0;

constexpr float RESTART_TIME = 0.05;

constexpr float FM_AMNT_MIN_VALUE  = 0.0;
constexpr float FM_AMNT_MAX_VALUE  = 8.0;
constexpr float FM_AMNT_SKEW_VALUE = 0.4;
constexpr float FM_AMNT_SET_VALUE  = 0.1;

constexpr float MOD_MULTI_MIN_VALUE  = 0.25;
constexpr float MOD_MULTI_MAX_VALUE  = 8.0;
constexpr float MOD_MULTI_SKEW_VALUE = 0.5;
constexpr float MOD_MULTI_SET_VALUE  = 0.5;

constexpr int TEMPO_MIN_VALUE  = 1;
constexpr int TEMPO_MAX_VALUE  = 200;
constexpr int TEMPO_SKEW_VALUE = 1.0;
constexpr int TEMPO_SET_VALUE  = 120;

constexpr float ATTACK_KNOB_MIN_VALUE  = 10.0;
constexpr float ATTACK_KNOB_MAX_VALUE  = 1000.0;
constexpr float ATTACK_KNOB_SKEW_VALUE = 0.5;
constexpr float ATTACK_KNOB_SET_VALUE  = 10.0;

constexpr float HOLD_KNOB_MIN_VALUE  = 0.0;
constexpr float HOLD_KNOB_MAX_VALUE  = 3000.0;
constexpr float HOLD_KNOB_SKEW_VALUE = 0.5;
constexpr float HOLD_KNOB_SET_VALUE  = 10.0;

constexpr float DECAY_KNOB_MIN_VALUE  = 10.0;
constexpr float DECAY_KNOB_MAX_VALUE  = 3000.0;
constexpr float DECAY_KNOB_SKEW_VALUE = 0.5;
constexpr float DECAY_KNOB_SET_VALUE  = 300.0;

constexpr float FM_CONTROL_MIN_VALUE  = 0.0;
constexpr float FM_CONTROL_MAX_VALUE  = 16.0;
constexpr float FM_CONTROL_SKEW_VALUE = 1.0;
constexpr float FM_CONTROL_SET_VALUE  = 1.0;

constexpr float SWING_MIN_VALUE     = 1.0;
constexpr float SWING_MAX_VALUE     = 1.75;
constexpr float SWING_DOT_VALUE     = 1.5;
constexpr float SWING_TRIPLET_VALUE = 1.3333333334;


constexpr int MIN_PITCH_NOTE    = 24;
constexpr int MAX_PITCH_NOTE    = 120;
constexpr int RESET_PITCH_VALUE = 48;
 
constexpr float SEQUENCER_PANEL_HEIGHT = 80;
constexpr float ENVELOPE_WINDOW_HEIGHT = 180;
constexpr float MOD_MATRIX_HEIGHT      = 60;

constexpr int   NUM_OF_MOD_VALUES = 10;
constexpr float MODULATOR_MULTI_VALUES[NUM_OF_MOD_VALUES] = { 0.25f, 0.5f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f };

constexpr int MOD_MATRIX_CHANNELS     = 4;
constexpr int MOD_MATRIX_SOURCES      = 3;
constexpr int MOD_MATRIX_DESTINATIONS = 7;


constexpr int ENVELOPE_ATTACK       = 0;
constexpr int ENVELOPE_HOLD         = 1;
constexpr int ENVELOPE_DECAY        = 2;
constexpr int ENVELOPE_LEVEL        = 3;
constexpr int ENVELOPE_ATTACK_CURVE = 4;
constexpr int ENVELOPE_DECAY_CURVE  = 5;




const StringArray LFO_SHAPES              { "Saw", "Sine", "Triangle", "Square" };
const StringArray LFO_POLARITIES          { "Bi-Polar", "Positive", "Negative" };
const StringArray LFO_RESTART_OPTIONS     { "Never", "Step", "Pattern" };
const StringArray MODULATION_SOURCES      { "Mod Env", "LFO", "Mod Seq" };
const StringArray MODULATION_DESTINATIONS { "FM", "Pitch", "Pan", "Volume", "Mod Multi", "Portamento", "Swing" };
const StringArray STEP_GATE_STATES        { "On", "Glide", "Off" };
const StringArray AMP_CONTROL_OPTIONS     { "Amp Env", "None"};

const StringArray STEPS_PITCH {"step1Pitch","step2Pitch","step3Pitch","step4Pitch","step5Pitch","step6Pitch","step7Pitch","step8Pitch","step9Pitch","step10Pitch","step11Pitch","step12Pitch","step13Pitch","step14Pitch","step15Pitch","step16Pitch","step17Pitch","step18Pitch","step19Pitch","step20Pitch","step21Pitch","step22Pitch","step23Pitch","step24Pitch","step25Pitch","step26Pitch","step27Pitch","step28Pitch","step29Pitch","step30Pitch","step31Pitch","step32Pitch"};

const StringArray STEPS_FM {"step1FM","step2FM","step3FM","step4FM","step5FM","step6FM","step7FM","step8FM","step9FM","step10FM","step11FM","step12FM","step13FM","step14FM","step15FM","step16FM","step17FM","step18FM","step19FM","step20FM","step21FM","step22FM","step23FM","step24FM","step25FM","step26FM","step27FM","step28FM","step29FM","step30FM","step31FM","step32FM"};

const StringArray STEPS_MOD_MULTI {"step1ModMulti","step2ModMulti","step3ModMulti","step4ModMulti","step5ModMulti","step6ModMulti","step7ModMulti","step8ModMulti","step9ModMulti","step10ModMulti","step11ModMulti","step12ModMulti","step13ModMulti","step14ModMulti","step15ModMulti","step16ModMulti","step17ModMulti","step18ModMulti","step19ModMulti","step20ModMulti","step21ModMulti","step22ModMulti","step23ModMulti","step24ModMulti","step25ModMulti","step26ModMulti","step27ModMulti","step28ModMulti","step29ModMulti","step30ModMulti","step31ModMulti","step32ModMulti"};

const StringArray STEPS_MOD_SEQ {"step1ModSeq","step2ModSeq","step3ModSeq","step4ModSeq","step5ModSeq","step6ModSeq","step7ModSeq","step8ModSeq","step9ModSeq","step10ModSeq","step11ModSeq","step12ModSeq","step13ModSeq","step14ModSeq","step15ModSeq","step16ModSeq","step17ModSeq","step18ModSeq","step19ModSeq","step20ModSeq","step21ModSeq","step22ModSeq","step23ModSeq","step24ModSeq","step25ModSeq","step26ModSeq","step27ModSeq","step28ModSeq","step29ModSeq","step30ModSeq","step31ModSeq","step32ModSeq"};

const StringArray STEPS_GATE {"step1GateState","step2GateState","step3GateState","step4GateState","step5GateState","step6GateState","step7GateState","step8GateState","step9GateState","step10GateState","step11GateState","step12GateState","step13GateState","step14GateState","step15GateState","step16GateState","step17GateState","step18GateState","step19GateState","step20GateState","step21GateState","step22GateState","step23GateState","step24GateState","step25GateState","step26GateState","step27GateState","step28GateState","step29GateState","step30GateState","step31GateState","step32GateState"};

const StringArray BASE_NOTES
{
    "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A1", "A#1", "B1",
    "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A2", "A#2", "B2",
    "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A3", "A#3", "B3",
    "C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4",
    "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5", "A#5", "B5", "C6"
};


