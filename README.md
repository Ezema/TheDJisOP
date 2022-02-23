# The DJ is OOP

![Screenshoot](./screenshoot.png?raw=true "The DJ is OOP")

## Background
This project was part of a coursework for the Object Oriented Programming (OOP) module at the university. 
## Description

This is a GUI audio player created with the JUCE library that allows you to mix two tracks while they are being played. You can adjust the volume and balance of each track independently, and use the crossfader to smoothly transition from one track to another. The app supports most common audio file formats, and features a responsive and intuitive interface.

## Features

- Play two tracks simultaneously and mix them in real time
- Adjust volume and balance for each track independently
- Use the crossfader to blend tracks smoothly
- Support for most common audio file formats
- Create a playlist and use the search bar to quickly find your favorite tracks
- Simple and intuitive user interface

## Dependencies

This project relies on the JUCE library, which must be installed before building and running the project. JUCE requires the following dependencies:

- A modern C++ compiler that supports C++11 or later
- OpenGL 2.0 or later
- libfreetype
- libcurl
- libasound (on Linux)
- CoreAudio (on macOS)
- XAudio2 (on Windows)

For more information on installing and configuring JUCE and its dependencies, please refer to the [JUCE documentation](https://github.com/juce-framework/JUCE/blob/master/docs/Linux%20Dependencies.md).

## Usage

1. Open the JUCE Audio Mixer app
2. Load two audio files by clicking on the "Load File" buttons
3. Adjust the volume and balance of each track to your liking
4. Use the crossfader to transition between tracks
5. Enjoy your mixed audio!

## Getting started

1. Install the JUCE library and its dependencies as outlined in the documentation.
2. Clone this repository to your local machine.
3. Open the Projucer app and open the AudioPlayer.jucer file.
4. Build the project in your desired IDE.
5. Run the project and enjoy DJing with your favorite tracks!

## Contributing

If you find a bug or have a feature request, please create an issue on GitHub or submit a pull request with your changes.

