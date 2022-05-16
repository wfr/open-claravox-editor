#include "MIDIParameter.h"
#include <vector>
#include <map>

const std::vector<MIDIParameter> midiparameters = {
    /***************************************************************
     * Instrument parameters (part of preset)
     ***************************************************************/
    { "volCurveVal", 12, 412, 16383,
       "Adjusts the linearity of the Volume Antenna response (concave to convex)." },

    { "pitchCurveVal", 13, 413, 16383,
      "Adjusts the linearity of the Pitch Antenna response (concave to convex)." },

    { "quantizeAmount", 16, 416, 16383,
      "Determines the level of quantization." },

    { "rootVal", 85, 485, 117,
      "Selects the root note (key) of the scale used for quantization." },

    { "scaleVal", 86, 486, 122,
      "Selects the scale used for quantization." },

    { "octaveState", 75, 475, 96,
      "Octave Offset for the Pitch Antenna." },

    { "filterVal", 19, 419, 16383,
      "Analog VCF (Oscillator 1 only)" },

    { "brightVal", 17, 417, 16383,
      "Analog Harmonic Enhancement (Oscillator 1 only)" },

    { "waveVal", 18, 418, 16383,
      "Analog Waveshaper (Oscillator 1 only)" },

    { "timeVal", 20, 420, 16383,
      "Adjusts the Delay Time (70ms to 700ms)." },

    { "delayAmount", 21, 421, 16383,
      "Adjusts the Delay Wet/Dry mix." },

    { "feedbackVal", 22, 422, 16383,
      "Adjusts the Delay Feedback amount." },

    { "pitchModWsFreq", 72, 472, 127, // Pitch Antenna Mod: Wavetable Scan Freq
      "This parameter controls the depth of the PITCH CV signal used to modulate the frequency at which the oscillators’ wavetable is scanned." },

    { "pitchModWsAmount", 73, 473, 127, // Pitch Antenna Mod: Wavetable Scan Pos
      "This parameter controls the depth of the PITCH CV signal used to modulate the scan position of the oscillators’ wavetable." },

    { "osc1Type", 81, 481, 96,
      "Selects the mode for Oscillator 1." },

    { "osc1Level", 25, 425, 16383,
      "Selects the level for Oscillator 1" },

    { "osc1Wavetable", 83, 483, 112,
      "Selects the Wavetable in use for Oscillator 1, when the Oscillator 1 MODE is set to Wavetable (102)." },

    { "osc1ScanPos", 87, 487, 127,
      "Specifies the starting position for scanning back and forth through the selected Wavetable." },

    { "osc1ScanFreq", 23, 423, 16383,
      "Adjusts the rate for scanning back and forth through the selected Wavetable (0.1 Hz to 100 Hz)." },

    { "osc1ScanAmount", 89, 489, 127,
      "Sets the width of the Wavetable scanning window (0 to full frame)." },

    { "osc2Beat", 28, 428, 127,
      "Linear offset (-10Hz to +10Hz) for the secondary oscillator frequency relative to the primary." },

    { "osc2Freq", 27, 427, 127,
      "Musical-pitch offset (-12 semitones to +12 semitones) for the secondary oscillator relative to the primary." },

    { "osc2Level", 26, 426, 16383,
      "Specifies the level of Oscillator 2." },

    { "noiseLevel", 9, 409, 16383,
      "Specifies the level of the Noise source sent to the Filter of Oscillator 2." },

//    { "osc2Type", ?, ?, 96,
//      "Selects the mode for Oscillator 2." },

    { "osc2Wavetable", 82, 482, 112,
      "Selects the Wavetable in use for Oscillator 2, when the Oscillator 2 MODE is set to Wavetable (102)." },

    { "osc2ScanPos", 88, 488, 127,
      "Specifies the starting position for scanning back and forth through the selected Wavetable." },

    { "osc2ScanFreq", 24, 424, 16383,
      "Adjusts the rate for scanning back and forth through the selected Wavetable (0.1 Hz to 100 Hz)." },

    { "osc2ScanAmount", 90, 490, 127,
      "Sets the width of the Wavetable scanning window (0 to full frame)." },

    { "osc2FilterFreq", 8, 408, 16383,
      "Sets the Cutoff Frequency of the two-pole State-Variable Filter (SVF) affecting Oscillator 2 and Noise." },

    { "osc2FilterMode", 91, 491, 96,
      "Sets the Filter mode of the two-pole State-Variable Filter (SVF) affecting Oscillator 2 and Noise." },

    { "osc2FilterRes", 10, 410, 16383,
      "Sets the Resonance Amount of the two-pole State-Variable Filter (SVF) affecting Oscillator 2 and Noise." },

    { "osc2FilterEnable", 103, 503, 64,
      "Determines whether or not the Oscillator 2 Filter is applied to Oscillator 2 and the Noise Source." },

    { "osc1FilterPitchAmount", 2, 402, 16383,
      "Determines how much the Pitch Antenna value changes the Cutoff Frequency of the Oscillator 1 Filter." },

    { "osc2FilterPitchAmount", 3, 403, 16383,
      "Determines how much the Pitch Antenna value changes the Cutoff Frequency of the Oscillator 2 Filter." },

    { "osc1FilterVolumeAmount", 4, 404, 16383,
      "Determines how much the Volume Antenna value changes the Cut-off Frequency of the Oscillator 1 Filter." },

    { "osc2FilterVolumeAmount", 5, 405, 16383,
      "Determines how much the Volume Antenna value changes the Cut-off Frequency of the Oscillator 2 Filter." },

    { "midiNoteNumberMode", 110, 510, 64,
      "Determines whether MIDI pitch control messages will be sent via Fixed mode or Chromatic mode." },

    { "midiPitchOut", 108, 508, 64,
      "Determines whether or not MIDI Pitch data corresponding with Volume and Pitch antenna CV will be sent." },

    { "midiNoteNumber", 109, 509, 127,
      "Set the base Note Number that will be used for MIDI Note On when in Pitch Move mode." },

    /***************************************************************
     * Global instrument settings (not part of preset)
     ***************************************************************/
    { "Volume CV Output Scale", 30, 430, 16383,
      "Attenuates the max CV Output level available to the selected Volume CV Out Range. Negative values invert the output range."},

    { "Volume CV Output Range", 104, 504, 86,
      "Sets the CV range found at the VOLUME OUT jack."},

    { "Pitch CV Output Range", 93, 493, 86,
      "Sets the voltage range for the PITCH OUT CV jack."},

    { "Pitch CV Out Quantize", 95, 495, 64,
      "Selects if the value of the PITCH OUT CV jack is determined before or after any quantization."},

    { "CV In Scale", 14, 414, 16383,
      "Scales the voltage of the CV IN jack plus/minus 100%; Center value = 0 output."},

    { "CV In Offset", 29, 429, 16383,
      "This parameter provides an offset voltage that is added or subtracted from the CV Input Range."},

    { "CV In Range", 92, 492, 64,
      "Sets the expected voltage range of the CV IN jack."},

    { "Mute Mode", 107, 507, 64,
      "Determines whether the panel MUTE jack toggles MUTE on/off with successive presses (latching), or mutes on press/unmutes on release (momentary)."},

    { "MIDI Note On Threshold", 111, 511, 16383,
      "Determines the Volume Antenna CV level at which a Note On/Off message will be sent. Note will remain On as long as Volume Antenna CV is above threshold, and will remain Off as long as Volume Antenna CV is below threshold."},

    { "Volume Antenna Control Mode", 113, 513, 86,
      "Determines whether the Volume Antenna is active, active with automute enabled, or disabled with external MIDI CC# 7 control of Volume enabled."},

    { "Volume Antenna MIDI CC# Selection", 0 /* None */, 2501, 31,
      "Determines which MIDI CC# is used to send values corresponding with Volume Antenna CV."},
};


std::optional<const MIDIParameter> mpFromCC(uint8_t cc) {
    static std::map<uint8_t, MIDIParameter> cc2mp;
    if (cc2mp.size() == 0) {
        for (const auto mp : midiparameters) {
            cc2mp[mp.cc] = mp;
        }
    }

    auto it = cc2mp.find(cc);
    if (it != cc2mp.end()) {
        return it->second;
    }
    return std::optional<MIDIParameter>();
}

std::optional<const MIDIParameter> mpFromName(const QString& name) {
    static std::map<QString, MIDIParameter> name2mp;
    if (name2mp.size() == 0) {
        for (const auto mp : midiparameters) {
            name2mp[mp.name] = mp;
        }
    }

    auto it = name2mp.find(name);
    if (it != name2mp.end()) {
        return it->second;
    }
    return std::optional<MIDIParameter>();
}
