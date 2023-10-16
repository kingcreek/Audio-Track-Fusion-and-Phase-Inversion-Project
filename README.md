# Audio Track Fusion and Phase Inversion Project

## Overview

This project is a project with no real-world utility, but it serves as a test for working with the "libsndfile" library. Its aim is to demonstrate a peculiar behavior when playing audio on different types of audio output devices, particularly stereo and mono speakers.

In stereo mode, the project will play both the primary audio and a hidden audio track, combining them for a unique auditory experience. However, on mono speakers, only the primary audio will be heard.

## Usage

To use this project:

1. Clone the repository to your local machine.

2. Compile the program by running the `make` command.

3. Execute the program, specifying the paths to the audio files you want to process. For example:

```
./Sound audio_original.wav audio_hidden.wav
```

Replace "audio_original.wav" and "audio_hidden.wav" with the paths to your audio files.

## Acknowledgments

- [LibSndfile](http://www.mega-nerd.com/libsndfile/)
- [PortAudio](http://www.portaudio.com/)
