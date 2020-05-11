/*
  ==============================================================================

    Constants.h
    Created: 4 Feb 2020 12:54:47pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

constexpr int BASE_PITCH = 60;

constexpr int MIN_NUM_OF_STEPS = 4;
constexpr int MAX_NUM_OF_STEPS = 32;
constexpr int DEF_NUM_OF_STEPS = 16;

constexpr double MILLISECONDS_IN_SECOND = 1000.0;

constexpr double RESTART_TIME = 0.01;

constexpr double FM_AMNT_MIN_VALUE = 0.0;
constexpr double FM_AMNT_MAX_VALUE = 8.0;
constexpr double FM_AMNT_SKEW_VALUE = 0.4;
constexpr double FM_AMNT_SET_VALUE = 0.1;

constexpr double MOD_MULTI_MIN_VALUE = 0.25;
constexpr double MOD_MULTI_MAX_VALUE = 8.0;
constexpr double MOD_MULTI_SKEW_VALUE = 0.5;
constexpr double MOD_MULTI_SET_VALUE = 0.5;

constexpr int TEMPO_MIN_VALUE = 50;
constexpr int TEMPO_MAX_VALUE = 200;
constexpr int TEMPO_SKEW_VALUE = 1.0;
constexpr int TEMPO_SET_VALUE = 120;

constexpr double ATTACK_KNOB_MIN_VALUE = 10.0;
constexpr double ATTACK_KNOB_MAX_VALUE = 1000.0;
constexpr double ATTACK_KNOB_SKEW_VALUE = 0.5;
constexpr double ATTACK_KNOB_SET_VALUE = 10.0;

constexpr double HOLD_KNOB_MIN_VALUE = 0.0;
constexpr double HOLD_KNOB_MAX_VALUE = 3000.0;
constexpr double HOLD_KNOB_SKEW_VALUE = 0.5;
constexpr double HOLD_KNOB_SET_VALUE = 10.0;

constexpr double DECAY_KNOB_MIN_VALUE = 10.0;
constexpr double DECAY_KNOB_MAX_VALUE = 3000.0;
constexpr double DECAY_KNOB_SKEW_VALUE = 0.5;
constexpr double DECAY_KNOB_SET_VALUE = 300.0;

constexpr double FM_CONTROL_MIN_VALUE = 0.0;
constexpr double FM_CONTROL_MAX_VALUE = 16.0;
constexpr double FM_CONTROL_SKEW_VALUE = 1.0;
constexpr double FM_CONTROL_SET_VALUE = 1.0;

//constexpr double FM_CONTROL_MIN_VALUE = 0.0;
//constexpr double FM_CONTROL_MAX_VALUE = 1.0;
//constexpr double FM_CONTROL_SKEW_VALUE = 1.0;
//constexpr double FM_CONTROL_SET_VALUE = 0.0;

constexpr int MIN_PITCH_NOTE = 24;
constexpr int MAX_PITCH_NOTE = 120;
constexpr int RESET_PITCH_VALUE = 48;

constexpr int HARD_CODED_NOTES[32] = {48,60,72,67,65,60,48,72,48,60,72,67,65,60,48,72,48,60,72,67,65,60,48,72,48,60,72,67,65,60,48,72};

 
constexpr float SEQUENCER_PANEL_HEIGHT = 80;
constexpr float ENVELOPE_WINDOW_HEIGHT = 160;

