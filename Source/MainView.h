/*
  ==============================================================================

    MainView.h
    Created: 26 Nov 2020 2:51:46pm
    Author:  idakil

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class MainView : public juce::Component {
public:
    MainView::MainView() {
        tabbedComponent.reset(new juce::TabbedComponent(juce::TabbedButtonBar::TabsAtTop));
        addAndMakeVisible(tabbedComponent.get());
        tabbedComponent->setTabBarDepth(30);
        tabbedComponent->addTab(TRANS("Tab 0"), juce::Colours::lightgrey, 0, false);
        tabbedComponent->addTab(TRANS("Tab 1"), juce::Colours::lightgrey, 0, false);
        tabbedComponent->setCurrentTabIndex(0);

        tabbedComponent->setBounds(0, 0, 344, 432);

        setSize(600, 400);

    };
    void paint(juce::Graphics& g) override;
    void resized() override;
private:
    std::unique_ptr<juce::TabbedComponent> tabbedComponent;

};