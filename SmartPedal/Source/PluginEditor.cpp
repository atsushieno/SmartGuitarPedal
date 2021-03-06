/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SmartPedalAudioProcessorEditor::SmartPedalAudioProcessorEditor (SmartPedalAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to

    // Overall Widgets

    addAndMakeVisible(modelLabel);
    modelLabel.setText("Model", juce::NotificationType::dontSendNotification);
    modelLabel.setJustificationType(juce::Justification::centred);
    modelLabel.setColour(juce::Label::textColourId, juce::Colours::black);

    addAndMakeVisible(modelSelect);
    modelSelect.setColour(juce::Label::textColourId, juce::Colours::black);
    int c = 1;
    for (const auto& jsonFile : processor.jsonFiles) {
        modelSelect.addItem(jsonFile.getFileName(), c);
        c += 1;
    }
    modelSelect.onChange = [this] {modelSelectChanged();};

    auto font = modelLabel.getFont();
    float height = font.getHeight();
    font.setHeight(height);
    modelLabel.setFont(font);

    // Set Widget Graphics
    blackHexKnobLAF.setLookAndFeel(ImageCache::getFromMemory(BinaryData::knob_hex_png, BinaryData::knob_hex_pngSize));

    // Pre Amp Pedal Widgets
 
    // Overdrive
    odFootSw.setImages(true, true, true,
        ImageCache::getFromMemory(BinaryData::footswitch_up_png, BinaryData::footswitch_up_pngSize), 1.0, Colours::transparentWhite,
        Image(), 1.0, Colours::transparentWhite,
        ImageCache::getFromMemory(BinaryData::footswitch_down_png, BinaryData::footswitch_down_pngSize), 1.0, Colours::transparentWhite,
        0.0);
    addAndMakeVisible(odFootSw);
    odFootSw.addListener(this);

    odLED.setImages(true, true, true,
        ImageCache::getFromMemory(BinaryData::led_red_on_png, BinaryData::led_red_on_pngSize), 1.0, Colours::transparentWhite,
        Image(), 1.0, Colours::transparentWhite,
        ImageCache::getFromMemory(BinaryData::led_red_on_png, BinaryData::led_red_on_pngSize), 1.0, Colours::transparentWhite,
        0.0);
    addAndMakeVisible(odLED);

    addAndMakeVisible(odDriveKnob);
    odDriveKnob.setLookAndFeel(&blackHexKnobLAF);
    odDriveKnob.addListener(this);
    odDriveKnob.setRange(-23.0, 25.0);
    odDriveKnob.setValue(1.0);
    odDriveKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    odDriveKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    odDriveKnob.setNumDecimalPlacesToDisplay(1);
    odDriveKnob.setDoubleClickReturnValue(true, 0.0);

    addAndMakeVisible(odToneKnob);
    odToneKnob.setLookAndFeel(&blackHexKnobLAF);
    odToneKnob.addListener(this);
    odToneKnob.setRange(1000.0, 20000.0);
    odToneKnob.setValue(10500.0);
    odToneKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    odToneKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    odToneKnob.setNumDecimalPlacesToDisplay(1);
    odToneKnob.setDoubleClickReturnValue(true, 10000.0);

    addAndMakeVisible(odLevelKnob);
    odLevelKnob.setLookAndFeel(&blackHexKnobLAF);
    odLevelKnob.addListener(this);
    odLevelKnob.setRange(-23.0, 25.0);
    odLevelKnob.setValue(1.0);
    odLevelKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    odLevelKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    odLevelKnob.setNumDecimalPlacesToDisplay(1);
    odLevelKnob.setDoubleClickReturnValue(true, 0.0);

    // Size of plugin GUI
    setSize (500, 650);
}

SmartPedalAudioProcessorEditor::~SmartPedalAudioProcessorEditor()
{
}

//==============================================================================
void SmartPedalAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    background = ImageCache::getFromMemory(BinaryData::smart_pedal_png, BinaryData::smart_pedal_pngSize);

    g.drawImageAt(background, 0, 0);

    g.setColour (Colours::white);
    g.setFont (15.0f);

    // Should really override the ToggleButton class, but being lazy here
    if (processor.od_state == 1) {
        odLED.setImages(true, true, true,
            ImageCache::getFromMemory(BinaryData::led_red_on_png, BinaryData::led_red_on_pngSize), 1.0, Colours::transparentWhite,
            Image(), 1.0, Colours::transparentWhite,
            ImageCache::getFromMemory(BinaryData::led_red_on_png, BinaryData::led_red_on_pngSize), 1.0, Colours::transparentWhite,
            0.0);
    }
    else {
        odLED.setImages(true, true, true,
            ImageCache::getFromMemory(BinaryData::led_red_off_png, BinaryData::led_red_off_pngSize), 1.0, Colours::transparentWhite,
            Image(), 1.0, Colours::transparentWhite,
            ImageCache::getFromMemory(BinaryData::led_red_off_png, BinaryData::led_red_off_pngSize), 1.0, Colours::transparentWhite,
            0.0);
    }
}

void SmartPedalAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    //Overall Widgets
    modelSelect.setBounds(142, 36, 210, 25);
    modelLabel.setBounds(193, 12, 90, 25);

    // Overdrive Widgets
    odDriveKnob.setBounds(112, 115, 125, 145);
    odLevelKnob.setBounds(283, 115, 125, 145);
    odToneKnob.setBounds(195, 234, 125, 145);
    odFootSw.setBounds(220, 459, 75, 105);
    odLED.setBounds(234, 398, 75, 105);
}

void SmartPedalAudioProcessorEditor::loadButtonClicked()
{
    FileChooser chooser("Load model directory...",
        {},
        "*.json");
    if (chooser.browseForDirectory())
    {
        processor.currentDirectory = chooser.getResult();
         
    }
}

void SmartPedalAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &odFootSw)
        odFootSwClicked();
}

void SmartPedalAudioProcessorEditor::odFootSwClicked() {
    if (processor.od_state == 0)
        processor.od_state = 1;
    else
        processor.od_state = 0;
    repaint();
}

void SmartPedalAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    // Overdrive    
    if (slider == &odDriveKnob)
        processor.set_odDrive(slider->getValue());
    else if (slider == &odToneKnob)
        processor.set_odTone(slider->getValue());
    else if (slider == &odLevelKnob)
        processor.set_odLevel(slider->getValue());
}

void SmartPedalAudioProcessorEditor::modelSelectChanged()
{
    String selectedFile = modelSelect.getText();
    processor.loadConfig(File(selectedFile));
}
