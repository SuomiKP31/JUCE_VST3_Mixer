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




## Update Oct 22, 2023
Added basic GUI frameworks and Cut Filters, this part is mostly from a Freecodecamp Tutorial (https://www.youtube.com/watch?v=i_Iq4_Kd7Rc)
However, since we are going a different direction, I made some changes and preparations for effectors. A new component is created to hold effector buttons, another new one will be added in the future to host launchpads.
![image](https://github.com/SuomiKP31/JUCE_VST3_Mixer/assets/50021290/1f87038f-7097-444c-b8c5-cddeb59b9d89)
