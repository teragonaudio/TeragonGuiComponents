/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "TeragonPluginBase.h"

namespace teragon {

TeragonPluginBase::TeragonPluginBase() {
    parameters.pause();
}

// Playback ////////////////////////////////////////////////////////////////////

void TeragonPluginBase::prepareToPlay(double, int) {
    parameters.resume();
}

void TeragonPluginBase::processBlock(AudioSampleBuffer &buffer, MidiBuffer &) {
    parameters.processRealtimeEvents();

    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for(int i = getNumInputChannels(); i < getNumOutputChannels(); ++i) {
        buffer.clear(i, 0, buffer.getNumSamples());
    }
}

void TeragonPluginBase::releaseResources() {
    parameters.pause();
}

// Plugin properties ///////////////////////////////////////////////////////////

bool TeragonPluginBase::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool TeragonPluginBase::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

// Parameter handling //////////////////////////////////////////////////////////

int TeragonPluginBase::getNumParameters() {
    return parameters.size();
}

float TeragonPluginBase::getParameter(int index) {
    return (float)parameters[index]->getScaledValue();
}

void TeragonPluginBase::setParameter(int index, float newValue) {
    parameters.setScaled((const size_t)index, newValue);
}

const String TeragonPluginBase::getParameterName(int index) {
    return parameters[index]->getName();
}

const String TeragonPluginBase::getParameterText(int index) {
    return parameters[index]->getDisplayText();
}

bool TeragonPluginBase::isMetaParameter(int index) const {
    return (dynamic_cast<VoidParameter *>(parameters[index]) == nullptr);
}

// State save/restore //////////////////////////////////////////////////////////

void TeragonPluginBase::getStateInformation(MemoryBlock &destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TeragonPluginBase::setStateInformation(const void *data, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

} // namespace teragon
