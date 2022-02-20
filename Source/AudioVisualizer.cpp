#include "AudioVisualizer.h"
using namespace juce;

CustomVisualizer::CustomVisualizer() : AudioVisualiserComponent(2) {
    setBufferSize(1024);
    setSamplesPerBlock(16);
    setColours(Colours::black, Colours::red);    
};