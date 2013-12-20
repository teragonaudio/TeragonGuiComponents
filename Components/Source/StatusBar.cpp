/*
  ==============================================================================

    StatusBar.cpp
    Created: 18 Dec 2013 6:48:02pm
    Author:  Nik Reiman

  ==============================================================================
*/

#include "StatusBar.h"
#include "PluginParameterComponent.h"

namespace teragon {

StatusBar::StatusBar(ThreadsafePluginParameterSet &parameters, const ResourceCache *resources) :
juce::Component(),
PluginParameterObserver(),
parameters(parameters), labelOpacity(1.0), clearTimeout(0) {
    addAndMakeVisible(&parameterNameLabel);
    addAndMakeVisible(&parameterValueLabel);

    for(int i = 0; i < parameters.size(); ++i) {
        parameters[i]->addObserver(this);
    }
}

void StatusBar::resized() {
    Component::resized();
    const int labelHeight = (getHeight() - kPadding) / 2;
    parameterNameLabel.setBounds(0, 0, getWidth(), labelHeight);
    parameterValueLabel.setBounds(0, labelHeight + kPadding, getWidth(), labelHeight);
    configureLabelProperties(parameterNameLabel);
    configureLabelProperties(parameterValueLabel);
}

void StatusBar::configureLabelProperties(Label &label) {
    label.setColour(Label::textColourId, Colours::black);
    label.setFont(Font(Font::getDefaultMonospacedFontName(), 12, Font::plain));
}

StatusBar::~StatusBar() {
    for(int i = 0; i < parameters.size(); ++i) {
        parameters[i]->removeObserver(this);
    }
}

bool StatusBar::isRealtimePriority() const {
    return false;
}

void StatusBar::onParameterUpdated(const PluginParameter *parameter) {
    juce::MessageManagerLock lock;

    // Don't bother showing boolean parameters, toggle buttons change their state too quickly
    // and push buttons are rather obvious.
    if(dynamic_cast<const BooleanParameter*>(parameter) == nullptr) {
        // Only update the parameter if it has at least one other component listening to it. Otherwise
        // we risk showing a bunch of internal parameters which we may not want to expose.
        for(unsigned int i = 0; i < parameter->getNumObservers(); ++i) {
            PluginParameterObserver *observer = parameter->getObserver(i);
            if(observer != this && dynamic_cast<PluginParameterComponent*>(observer) != nullptr) {
                displayParameter(parameter);
            }
        }
    }
}

void StatusBar::displayParameter(const PluginParameter *parameter) {
    parameterNameLabel.setText("aoeu aoeu aoeu aoeu aoeu aoeu aoeu ht sq");
    parameterValueLabel.setText(parameter->getDisplayText());

    labelOpacity = 1.0f;
    clearTimeout = kClearTimeoutInMs / 30; // ~30fps
    if (!isTimerRunning()) {
        startTimer(33); // ~30fps
    }
}

void StatusBar::timerCallback() {
    // Displaying text at full opacity
    if(clearTimeout > 0) {
        --clearTimeout;
        parameterNameLabel.setAlpha(1.0f);
        parameterValueLabel.setAlpha(1.0f);
    }
    // Fadeout has started
    else if(labelOpacity > 0.0f) {
        labelOpacity -= kFadeStepRate;
        parameterNameLabel.setAlpha(labelOpacity);
        parameterValueLabel.setAlpha(labelOpacity);
    }
    // Fadeout finished, hide the text and stop the timer
    else {
        parameterNameLabel.setText(String::empty);
        parameterValueLabel.setText(String::empty);
        stopTimer();
    }
}
} // namespace teragon
