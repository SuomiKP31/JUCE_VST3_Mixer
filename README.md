# JUCE_VST3_Mixer
Project for UVic CSC561 (Ongoing)
This project aims to program a toy project using JUCE Framework.
Ideally it should be able to apply some audio effects in real time, do filtering and play some launchpad samples.

## Purpose of this Course Project
Tap within one of the foundations of modern digital composing - sample manipulation algorithms.
In CSC 461/561, audio representation is one of the first things we learned. Audio signals are quantized and stored on computers, which not only enables playback, but also provides the potential of combination and manipulation. In the late 1990s, DAWs (Digital Audio Workstation) began to appear.
This project mainly showcases some interesting magics that you can perform on audio samples, and the fact that it can be done real time even created a new job type - the DJs.
We will use JUCE Framework for this project, as it provides a ready-to-use GUI library with a processing loop that is designated for DSP algorithms to work.

## Bi-weekly deliverables

For the first two weeks, there's mainly the GUI framework.
After that, I implement a new effector for each update and explain how it works, and where it can come handy. (You might find some familiar voicelines actually processed by those effectors!) 

## Setup Guide

To play with the vst plugin, you can download the release package(TODO) or compile using Visual Studio 2022 by yourself.
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
