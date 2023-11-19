# JUCE_VST3_Mixer
Project for UVic CSC561

This project aims to program a toy project using JUCE Framework.
Ideally it should be able to apply some audio effects in real time, do filtering and play some launchpad samples.

## Purpose of this Course Project
Tap within one of the foundations of modern digital composing - sample manipulation algorithms.
In CSC 461/561, audio representation is one of the first things we learned. Audio signals are quantized and stored on computers, which not only enables playback, but also provides the potential of combination and manipulation. In the late 1990s, DAWs (Digital Audio Workstation) began to appear.
This project mainly showcases some interesting magics that you can perform on audio samples, and the fact that it can be done real time even created a new job type - the DJs.
We will use JUCE Framework for this project, as it provides a ready-to-use GUI library with a processing loop that is designated for DSP algorithms to work.

## Bi-weekly deliverables

For the first two weeks, there's mainly the GUI framework.
After that, I implement a new effect for each update and explain how it works, and where it can come handy. (You might find some familiar voicelines actually processed by those effectors!) 

## Reference

[musicdsp.org](https://www.musicdsp.org)
[Unnamed SDVX Clone - DSP Implementations](https://github.com/Drewol/unnamed-sdvx-clone/blob/develop/Audio/include/Audio/DSP.hpp)

## Setup Guide

To play with the vst plugin, you can download the [release package](https://github.com/SuomiKP31/JUCE_VST3_Mixer/releases) or compile using Visual Studio 2022 by yourself.
There will be one simple audio player plugin and JUCE plugin host available in the package.

### The plugin host
VST3 is a widely used standard for plugins in DAWs, therefore you can use this plugin in any composition software. However, a lightweight plugin host is enough for this project.
The plugin host is available in the JUCE Framework, in the extra folder.

Follow these steps:
- After you extracted the JUCE package (which you can get [here](https://juce.com/get-juce/) ), you can find ProJucer.exe, open it.
- and then open the project file(audiopluginhost.jucer) under JUCE\extras\AudioPluginHost.
- Compile it in Visual Studio.
- Find the compiled exe and run it!

### The audio player
I forked a plugin from several years ago and made it compatible with modern JUCE. You can get the codes [here](https://github.com/SuomiKP31/AudioFilePlayerPlugin). Open it the same way as the plugin host and compile it.
However this time, you won't need an exe, but a VST3 file that we can use in the plugin host.

![image](https://github.com/SuomiKP31/JUCE_VST3_Mixer/assets/50021290/59b4272d-fa66-43c0-9eba-25061d578ceb)

Manually build a VST3 file here, and find it in Builds/VS2022/Release/VST3. 

Then, in your plugin host, press Ctrl+P to get a list of VST plugins. Here, you click options->scan for new VST3 plugins, and add the aforementioned folder. 
**If you get the release package, do this for whereever you put the extracted files.**

![image](https://github.com/SuomiKP31/JUCE_VST3_Mixer/assets/50021290/3a6f10d1-cd7c-4d56-b443-3dff885b20e2)

Then perform a scan, you should see your plugin appears here after right click inside Plugin Host.
Drop it here and connect it to your output port. Double click the blocks to open GUI.

![image](https://github.com/SuomiKP31/JUCE_VST3_Mixer/assets/50021290/b99fe592-b27d-4f45-a576-77da8ca7e13b)

Drag and drop your file here and hit play!

### The mixer plugin

The mixer also contains a jucer file, and compile it like the audio player to get a VST3 file.

Drop it to the host, similar to what you are doing minutes ago. (Ctrl+P in host -> Add Path -> Scan -> Right Click)

Disconnect the pins (right click on Audio Player block), and re-connect them like this, to let the mixer process the audio.

![image](https://github.com/SuomiKP31/JUCE_VST3_Mixer/assets/50021290/a5859176-e2ea-4366-8890-a5780136cda0)

Play the audio, then start to turn the knobs.

## Update Oct 22, 2023
Added basic GUI frameworks and Cut Filters, this part is mostly from a Freecodecamp Tutorial (https://www.youtube.com/watch?v=i_Iq4_Kd7Rc)
However, since we are going a different direction, I made some changes and preparations for effectors. A new component is created to hold effector buttons, another new one will be added in the future to host launchpads.
![image](https://github.com/SuomiKP31/JUCE_VST3_Mixer/assets/50021290/1f87038f-7097-444c-b8c5-cddeb59b9d89)

## Update Nov 6, 2023
Two new effects added, occupying 8 slots.
These two showcases time-domain DSP, i.e. they are not doing frequency filtering. We'll have some frequency-domain stuff next time.

More implementation details on effects will be included in Project Report.

![image](https://github.com/SuomiKP31/JUCE_VST3_Mixer/assets/50021290/5e3f5cd6-5667-4f2a-a168-079daef83b29)


### RE (Resample / Repeat)
Resample is probably the simplest time domain effect. In real time mixing or composing, it might be used to create glitch effects, or simply alternate how you want the music to play out.
Note that in order to make it sound proper, a correct BPM (Beats Per Minute) parameter is needed, because resample will interrupt the rhythm if the BPM is wrong, or the effect is applied at the wrong moment.

Behind the scenes, once RE_x is enabled, the program starts to collect a sample array of the same length as an x_th note as the music goes. Say you press RE4, then the resample array will be exactly the length of a beat (1/4 note).
After the collection is complete, the resample effect just repeats this sample as your output, until you release.

This effect is rarely used in composing because for the most part it doesn't sound quite right. However, if you are deliberately create glitchy hearings, then it is your best bet!

### GA (Gate)
Gate is commonly used in a genre called Psy Trance. Similar to RE, it also works according to musical time.

Once GA_x is enabled, the program starts a noise gate that is moving linearly up and down along with the time axis. It will reach its peak at exactly the attack time of each x_th note, fall and rise again, reaching its peak once more on the next x_th note.

This will greatly emphasize the perception of certain notes on the beats, because audio samples in between will be weaker compared to the on-beat ones. Therefore it's especially good for Trances, where the rhythm of the song is mostly unaltered throughout.

## Update Nov 20, 2023
I've implemented a few more interesting effects for show here. Below is a brief explanation of how they work.

### Phaser

Phaser and Flager are two popular effects utilizing an LFO (Low Frequency Oscillator). They are commonly used frequency domain manipulations. You might even see them in physical form(pedals) on some electrical instruments.

An LFO is basically just a sine function that has a low frequency, typically 1-10 Hz.

For phaser, it creates a few IIR filters that has oscillating frequencies, according to our LFO. IIR filters have many notations, JUCE uses the [biquad](https://en.wikipedia.org/wiki/Digital_biquad_filter) notation. This is also the filter we used for the knobs.
Here, the filter (chain) we use is a band-cut filter, which is equivalent of a oscillating peak filter with a negative amplitude (i.e. removing certain frequency bands from the signal). Try setup the peak filter this way, turn the knob periodically, and hear if it's similar to the phaser effect.

Of course, you can set the phaser to act on any type of filter that has a frequency parameter. All those effects are generally called phaser and here we're just demonstrating one of them.

### Flanger

Flanger is a more inspriation-struck idea of utilizing LFOs. The algorithm behind it does a very weird thing - feeding a delayed sample of the original audio with an oscillating delay.
This sounds very counter-intuitive, but it somehow produces a very sci-fi tone to the audio.

Aside from creating frequency sweep hearings, flanger is also commonly used in voice acting. One famous example is 343 Guilty Spark in the Halo series. He has all his voice clips flanged, [hear here](https://www.youtube.com/watch?v=JpGu4xeDeKQ).

These two are the hard ones to understand, so if you are confused, watch this great [visualization video](https://www.youtube.com/watch?v=Ici_YOVDl_0).

### Bit Crusher

At this point of the class, you probably know that all digital audios are quantized, and stored in floating point numbers. In the process of quantization, we create quantization noise due to bit-depth limit.
In older days, the bit depth digital audio can achieve is low, usually 8bit or 16bit, as the result, they sounds lossy. Nowadays, a genre called chiptone creates 8-bit like audio to get some retro-vibe.

One of the tools they use is the Bit Crusher effect. It's the most basic way of deliberately introducing quantization noise. What this effect do is just take each sample, and map them to fewer bands (corresponding to a given bit depth) to simulate the quantization process.
You will need far more than this to create audible chiptone, but this one will suffice in creating a few glitchy effects.

In Nier:Automata, Platinium Studio creates a [Tone Filter that works alongside a bitcrusher](https://www.youtube.com/watch?v=pBD8zo-M9rs) to generate real-time chiptone variants of their music.

### Tape Stop

In old school days of DJs, they scratch the tape to create this effect. Now though... All audio are digitized and we don't have a tape to scratch, what do we do?
Well, the answer is we simulate the momentum of the tape! Once the effect is applied, the effect will sample slower and slower, until reaching a total mute. 
Note that we are not actually modifying the sample rate (output sample rate is usually constant), we are just feeding duplicated samples to simulate a slower and slower tape.


## Final Update

### Frequency Slam

A launchpad effect was added. 
Copyright Notice: The sound effect was taken from the rhythm game Sound Voltex. Do not redistribute this software before removing it.
The buttons slam the frequency band, making a cool sound and move your frequency knob to alternate hearing immediately.
Believe it or not, this is actually quite a headache to make, as you don't usually mix sounds in plugins. The SFX was loaded in binary and preloaded by a read-ahead thread.

![image](https://github.com/SuomiKP31/JUCE_VST3_Mixer/assets/50021290/865b2f2b-193b-4a75-b8ed-163a05542674)
