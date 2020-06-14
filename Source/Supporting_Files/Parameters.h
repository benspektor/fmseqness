/*
 ==============================================================================
 
 Parameters.h
 Created: 6 Jun 2020 3:04:18pm
 Author:  Ben Spector
 
 ==============================================================================
 */

#pragma once
#include <JuceHeader.h>
#include "Constants.h"


struct Parameters
{
    static AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
    {
        std::vector<std::unique_ptr<RangedAudioParameter>> params;
        
        params.push_back (std::make_unique<AudioParameterFloat>  ("globalFMAmount", "Global FM Amount",
                                                                  NormalisableRange<float>(FM_AMNT_MIN_VALUE, FM_AMNT_MAX_VALUE, 0.001, FM_AMNT_SKEW_VALUE),
                                                                  FM_AMNT_SET_VALUE));
        params.push_back (std::make_unique<AudioParameterFloat>  ("modulatorMultiplier", "Modulator Multiplier",
                                                                  NormalisableRange<float>(MOD_MULTI_MIN_VALUE, MOD_MULTI_MAX_VALUE, 0.001, MOD_MULTI_SKEW_VALUE),
                                                                  MOD_MULTI_SET_VALUE));
        params.push_back (std::make_unique<AudioParameterFloat>  ("tempo", "Tempo",
                                                                  NormalisableRange<float>(TEMPO_MIN_VALUE, TEMPO_MAX_VALUE, 1.0, TEMPO_SKEW_VALUE),
                                                                  TEMPO_SET_VALUE));
        params.push_back (std::make_unique<AudioParameterInt>    ("steps", "Steps", MIN_NUM_OF_STEPS, MAX_NUM_OF_STEPS, DEF_NUM_OF_STEPS));
        params.push_back (std::make_unique<AudioParameterBool>   ("play", "Play", false));
        params.push_back (std::make_unique<AudioParameterFloat>  ("currentStep", "Current Step", NormalisableRange<float>(0.0, MAX_NUM_OF_STEPS, 1.0, 1.0),MAX_NUM_OF_STEPS));
        params.push_back (std::make_unique<AudioParameterInt>    ("firstStepIndex", "First Step Index", 0, MAX_NUM_OF_STEPS - 1, 0));
        params.push_back (std::make_unique<AudioParameterInt>    ("lastStepIndex", "Last Step Index", 0, MAX_NUM_OF_STEPS - 1, DEF_NUM_OF_STEPS - 1));
        params.push_back (std::make_unique<AudioParameterFloat>  ("swingValue", "Swing Value", SWING_MIN_VALUE, SWING_MAX_VALUE, SWING_MIN_VALUE));
        params.push_back (std::make_unique<AudioParameterInt>    ("basePitch", "Base Pitch", 36, 84, 60));
        params.push_back (std::make_unique<AudioParameterFloat>  ("portamento", "Portamento", 0.01f, 1.0f, 0.1f));
        params.push_back (std::make_unique<AudioParameterFloat>  ("lfoFrequency", "LFO Frequency", NormalisableRange<float>(0.01f, 10.0f, 0.001, 0.4), 2.0f, "Hz"));
        params.push_back (std::make_unique<AudioParameterInt>    ("LfoLength", "LFO Length", 1, 32, 4, "Steps"));
        params.push_back (std::make_unique<AudioParameterFloat>  ("lfo2FMAmount", "LFO to FM Amount", 0.0f, 1.0f, 0.3f));
        params.push_back (std::make_unique<AudioParameterFloat>  ("lfo2ModMulti", "LFO to modulator multiplier", 0.0f, 1.0f, 0.0f));
        params.push_back (std::make_unique<AudioParameterFloat>  ("lfo2Panning", "LFO to panning", 0.0f, 1.0f, 0.0f));
        params.push_back (std::make_unique<AudioParameterChoice> ("lfoShape", "LFO Shape",LFO_SHAPES, 1));
        params.push_back (std::make_unique<AudioParameterFloat>  ("lfoPhase", "LFO Phase", 0.0f, 1.0f, 0.f));
        params.push_back (std::make_unique<AudioParameterChoice> ("lfoPolarity", "LFO Polarity", LFO_POLARITIES, 0));
        params.push_back (std::make_unique<AudioParameterBool>   ("lfoStepSync", "LFO Step Sync", false));
        params.push_back (std::make_unique<AudioParameterChoice> ("lfoRestart", "LFO Restart", LFO_RESTART_OPTIONS, 2));
        params.push_back (std::make_unique<AudioParameterChoice> ("ampControl", "Amp Control", AMP_CONTROL_OPTIONS, 0));
        
        params.push_back (std::make_unique<AudioParameterFloat>  ("mod1Amount"     , "Mod 1 Amount"     , -1.0f, 1.0f, 0.f));
        params.push_back (std::make_unique<AudioParameterChoice> ("mod1Source"     , "Mod 1 Source"     , MODULATION_SOURCES, 0));
        params.push_back (std::make_unique<AudioParameterChoice> ("mod1Destination", "Mod 1 Destination", MODULATION_DESTINATIONS, 0));
        params.push_back (std::make_unique<AudioParameterFloat>  ("mod2Amount"     , "Mod 2 Amount"     , -1.0f, 1.0f, 0.f));
        params.push_back (std::make_unique<AudioParameterChoice> ("mod2Source"     , "Mod 2 Source"     , MODULATION_SOURCES, 0));
        params.push_back (std::make_unique<AudioParameterChoice> ("mod2Destination", "Mod 2 Destination", MODULATION_DESTINATIONS, 0));
        params.push_back (std::make_unique<AudioParameterFloat>  ("mod3Amount"     , "Mod 3 Amount"     , -1.0f, 1.0f, 0.f));
        params.push_back (std::make_unique<AudioParameterChoice> ("mod3Source"     , "Mod 3 Source"     , MODULATION_SOURCES, 0));
        params.push_back (std::make_unique<AudioParameterChoice> ("mod3Destination", "Mod 3 Destination", MODULATION_DESTINATIONS, 0));
        params.push_back (std::make_unique<AudioParameterFloat>  ("mod4Amount"     , "Mod 4 Amount"     , -1.0f, 1.0f, 0.f));
        params.push_back (std::make_unique<AudioParameterChoice> ("mod4Source"     , "Mod 4 Source"     , MODULATION_SOURCES, 0));
        params.push_back (std::make_unique<AudioParameterChoice> ("mod4Destination", "Mod 4 Destination", MODULATION_DESTINATIONS, 0));
        
        params.push_back (std::make_unique<AudioParameterFloat>  ("ampEnvAttack"     , "Amp Envelope Attack"      , 0.0f, 1.0f, 0.1f));
        params.push_back (std::make_unique<AudioParameterFloat>  ("ampEnvHold"       , "Amp Envelope Hold"        , 0.0f, 1.0f, 0.2f));
        params.push_back (std::make_unique<AudioParameterFloat>  ("ampEnvDecay"      , "Amp Envelope Decay"       , 0.0f, 1.0f, 0.4f));
        params.push_back (std::make_unique<AudioParameterFloat>  ("ampEnvLevel"      , "Amp Envelope Level"       , 0.0f, 1.0f, 1.0f));
        params.push_back (std::make_unique<AudioParameterFloat>  ("ampEnvAttackCurve", "Amp Envelope Attack Curve", 0.0f, 1.0f, 0.0f));
        params.push_back (std::make_unique<AudioParameterFloat>  ("ampEnvDecayCurve" , "Amp Envelope Decay Curve" , 0.0f, 1.0f, 0.7f));
        
        params.push_back (std::make_unique<AudioParameterFloat>  ("modEnvAttack"     , "Mod Envelope Attack"      , 0.0f, 1.0f, 0.0f));
        params.push_back (std::make_unique<AudioParameterFloat>  ("modEnvHold"       , "Mod Envelope Hold"        , 0.0f, 1.0f, 0.0f));
        params.push_back (std::make_unique<AudioParameterFloat>  ("modEnvDecay"      , "Mod Envelope Decay"       , 0.0f, 1.0f, 0.4f));
        params.push_back (std::make_unique<AudioParameterFloat>  ("modEnvLevel"      , "Mod Envelope Level"       , 0.0f, 1.0f, 0.5f));
        params.push_back (std::make_unique<AudioParameterFloat>  ("modEnvAttackCurve", "Mod Envelope Attack Curve", 0.0f, 1.0f, 0.0f));
        params.push_back (std::make_unique<AudioParameterFloat>  ("modEnvDecayCurve" , "Mod Envelope Decay Curve" , 0.0f, 1.0f, 0.0f));
 
        
        for (int stepIndex = 0; stepIndex < MAX_NUM_OF_STEPS; stepIndex++)
        {
            String parameterID {"step"};
            parameterID << stepIndex + 1 << "Pitch";
            String parameterName {"Step "};
            parameterName << stepIndex + 1 << " Pitch";
            params.push_back (std::make_unique<AudioParameterInt> (parameterID, parameterName, -12, 12, 0));
        }
        
        for (int stepIndex = 0; stepIndex < MAX_NUM_OF_STEPS; stepIndex++)
        {
            String parameterID {"step"};
            parameterID << stepIndex + 1 << "FM";
            String parameterName {"Step "};
            parameterName << stepIndex + 1 << " FM";
            params.push_back (std::make_unique<AudioParameterFloat> (parameterID, parameterName, 0.0f, 1.0f, 0.0f));
        }
        
        for (int stepIndex = 0; stepIndex < MAX_NUM_OF_STEPS; stepIndex++)
        {
            String parameterID {"step"};
            parameterID << stepIndex + 1 << "ModMulti";
            String parameterName {"Step "};
            parameterName << stepIndex + 1 << " Modulator Multiplier";
            params.push_back (std::make_unique<AudioParameterFloat> (parameterID, parameterName, 0.0f, 1.0f, 0.0f));
        }
        
        for (int stepIndex = 0; stepIndex < MAX_NUM_OF_STEPS; stepIndex++)
        {
            String parameterID {"step"};
            parameterID << stepIndex + 1 << "ModSeq";
            String parameterName {"Step "};
            parameterName << stepIndex + 1 << " Modulatation Sequencer";
            params.push_back (std::make_unique<AudioParameterFloat> (parameterID, parameterName, -1.0f, 1.0f, 0.0f));
        }
        
        for (int stepIndex = 0; stepIndex < MAX_NUM_OF_STEPS; stepIndex++)
        {
            String parameterID {"step"};
            parameterID << stepIndex + 1 << "GateState";
            String parameterName {"Step "};
            parameterName << stepIndex + 1 << " Gate State";
            params.push_back (std::make_unique<AudioParameterInt> (parameterID, parameterName, 0, 2, 0));
        }
        
        return { params.begin(), params.end() };
    }
};
