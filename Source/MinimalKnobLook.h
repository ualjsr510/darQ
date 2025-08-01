#pragma once
#include <JuceHeader.h>

class MinimalKnobLook : public juce::LookAndFeel_V4
{
public:
    // Devuelve la unidad correspondiente según el nombre del slider
    juce::String getUnitForSlider(const juce::String& name) const
    {
        if (name.containsIgnoreCase("Slope"))     return " dB/oct";
        if (name.containsIgnoreCase("Gain"))      return " dB";
        if (name.containsIgnoreCase("Freq"))      return " Hz";
        return {};
    }

    // Devuelve una representación formateada del valor del slider
    juce::String getFormattedValue(const juce::Slider& slider) const
    {
        auto name = slider.getName();
        auto value = slider.getValue();

        if (name.containsIgnoreCase("Slope"))
        {
            int index = static_cast<int>(value);
            int dBPerOct = 12 * (index + 1);  // Ej. índice 0 -> 12 dB/oct
            return juce::String(dBPerOct) + getUnitForSlider(name);
        }

        if (name.containsIgnoreCase("Freq"))
        {
            return juce::String(static_cast<int>(value)) + getUnitForSlider(name);
        }

        if (name.containsIgnoreCase("Gain"))
        {
            return juce::String(value, 1) + getUnitForSlider(name);
        }

        return juce::String(value, 2);  // fallback general
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle,
        float rotaryEndAngle, juce::Slider& slider) override
    {
        const float radius = juce::jmin(width, height) / 2.0f - 2.0f;
        const float centreX = x + width / 2.0f;
        const float centreY = y + height / 2.0f;
        const float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        // Contorno del knob
        g.setColour(juce::Colours::lightgrey);
        g.drawEllipse(centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f, 1.0f);

        // Indicador
        juce::Path p;
        float indicatorLength = radius * 0.15f;

        float startX = centreX + radius * std::cos(angle);
        float startY = centreY + radius * std::sin(angle);
        float endX = centreX + (radius - indicatorLength) * std::cos(angle);
        float endY = centreY + (radius - indicatorLength) * std::sin(angle);

        p.startNewSubPath(startX, startY);
        p.lineTo(endX, endY);

        g.setColour(juce::Colours::white);
        g.strokePath(p, juce::PathStrokeType(1.0f));

        // Texto
        juce::String nameText = slider.getName();
        juce::String valueText = getFormattedValue(slider);

        juce::Rectangle<float> textArea(centreX - radius, centreY - radius * 0.25f, radius * 2.0f, radius * 0.5f);
        auto nameArea = textArea.removeFromTop(textArea.getHeight() * 0.4f);
        auto valueArea = textArea.removeFromTop(textArea.getHeight() * 0.8f);

        g.setColour(juce::Colours::lightgrey);
        juce::Font nameFont("AR PL UKai CN", radius * 0.25f, juce::Font::plain);
        g.setFont(nameFont);
        g.drawFittedText(nameText, nameArea.toNearestInt(), juce::Justification::centred, 1);

        g.setColour(juce::Colours::white);
        juce::Font valueFont("AR PL UKai CN", radius * 0.30f, juce::Font::plain);
        g.setFont(valueFont);
        g.drawFittedText(valueText, valueArea.toNearestInt(), juce::Justification::centred, 1);
    }
};
