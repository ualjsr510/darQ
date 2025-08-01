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
        g.setColour(juce::Colours::white);
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

        g.setColour(juce::Colours::lightgrey);
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

        // --- Min y Max en extremos del knob ---
        const float labelRadius = radius + radius * 0.20f;  // 20% más afuera
        const float minAngle = rotaryStartAngle;
        const float maxAngle = rotaryEndAngle;

        const float minX = centreX + labelRadius * std::cos(minAngle);
        const float minY = centreY + labelRadius * std::sin(minAngle);

        const float maxX = centreX + labelRadius * std::cos(maxAngle);
        const float maxY = centreY + labelRadius * std::sin(maxAngle);

        juce::String minText, maxText;
        double minValue = slider.getMinimum();
        double maxValue = slider.getMaximum();

        if (nameText.containsIgnoreCase("Freq"))
        {
            auto formatFreq = [](double v) {
                return v < 1000.0 ? juce::String((int)v) + " Hz"
                    : juce::String((int)(v / 1000.0)) + " kHz";
                };

            minText = formatFreq(minValue);
            maxText = formatFreq(maxValue);
        }
        else if (nameText.containsIgnoreCase("Gain"))
        {
            minText = juce::String(static_cast<int>(minValue)) + " dB";
            maxText = juce::String(static_cast<int>(maxValue)) + " dB";
        }
        else if (nameText.containsIgnoreCase("Slope"))
        {
            minText = "12 dB";
            maxText = "48 dB";
        }
        else if (nameText.containsIgnoreCase("Q"))
        {
            minText = juce::String(minValue, 2);
            maxText = juce::String(static_cast<int>(maxValue));
        }

        g.setColour(juce::Colours::lightgrey);
        g.setFont(juce::Font("AR PL UKai CN",radius * 0.13f, juce::Font::plain));

        auto labelWidth = 60;
        auto labelHeight = 20;

        g.drawText(minText, (int)minX - labelWidth / 2, (int)minY - labelHeight / 2, labelWidth, labelHeight, juce::Justification::centred);
        g.drawText(maxText, (int)maxX - labelWidth / 2, (int)maxY - labelHeight / 2, labelWidth, labelHeight, juce::Justification::centred);
    }
};
