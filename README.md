# PlayingWithArduino

This is my playground for Arduino projects.

## Setting up environment

I am using [vMicro](http://www.visualmicro.com/) in combination with [Visual Studio Community 2015](https://www.visualstudio.com/downloads/). To directly open and use my solution, you need to set it up correctly (see [Relative Paths for Arduino Projects in Visual Studio](http://www.visualmicro.com/page/Relative-Paths-for-Arduino-Projects-in-Visual-Studio.aspx)), in short:

1. Create a top level folder for all Arduino stuff, e.g. `D:\Arduino`
2. Create a sub folder `ide`, where you extract the [Arduino IDE](https://www.arduino.cc/en/Main/Software) into.
3. Create a sub folder `ide\portable`. This will contain all arduino ide settings, which are normally stored in `c:\users\[yourname]\appdata\local\arduino15`. You need to copy at least the `preferences.txt` from the `arduino15` folder here.
4. Create a sub folder `sketchbook`. Default location is `c:\users\[yourname]\Documents\arduino`. move this folder into the sketchbook folder. Then open Arduino IDE, go to File -> Preferences and set  the "sketch book location" to this folder.
5. Create a sub folder `projects`. This folder contains all Arduino projects, including [PlayingWithArduino](PlayingWithArduino
).

The resulting folder structure should look something like this:
```
\Arduino
	\ide
		\portable
			\preferences.txt
			\...
		\...
	\sketchbook
	\projects
```

Of course you can set up your environment differently. This is just, how I have done it to ensure relative paths in my solution.
