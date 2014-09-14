#Tutorial for Asynchronous Packet Automata

_Original Design by Neil Gershenfeld_ ~ _Tutorial Written by Daniel Cellucci_

##Starting Out
One of the first things I did when I began working with APA was port the system over to Arduino. I did this partly because reading through the code helped me learn what was going on, and partly because I enjoy Arduino and I think that APA will make a fine addition to the ecosystem.

For the most part, porting it over couldn't be easier. You simply add `apa.c` and `apa.h` as you would a [normal library](http://arduino.cc/en/Guide/Libraries). Then you add `#include <apa.h>` to the top of your sketch. 

The only real catch is defining the ports, within Arduino, but that, too, is straightforward and I'll show it below.

##Making APA work

APA comes with two sample boards and code to accompany them- one labelled *FTDI* and one labelled *IO*. These correspond to a board you talk to via USB, and a board that can talk to the FTDI board using APA and can change various inputs and outputs. 

In `./src`, I've included all of these files, so you can use them. They even have the traces so you can make the boards and play with them. 

We can recreate this code pretty easily on the Arduino, simply by connecting two together. Here is a simple diagram showing the wiring I for this test:

![Two Arduinos](/src/apa_arduino/Fritzing_Schematic_bb)



I have one small problem with these examples- they're too simple! Especially the FTDI board, it just doesn't have enough going on. 