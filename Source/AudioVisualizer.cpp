/*
  ==============================================================================

    AudioVisualizer.cpp
    Created: 18 Feb 2022 7:01:49pm
    Author:  windows

  ==============================================================================
*/

#include "AudioVisualizer.h"
using namespace juce;

CustomVisualizer::CustomVisualizer() : AudioVisualiserComponent(2) {
    setBufferSize(1024);
    setSamplesPerBlock(16);
    setColours(Colours::black, Colours::red);    
};