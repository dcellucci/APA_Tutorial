##Tutorial for Asynchronous Packet Automata

_Original Design by Neil Gershenfeld_ ~ _Tutorial Written by Daniel Cellucci_

##Starting Out
One of the first things I did when I began working with APA was port the system over to Arduino. I did this partly because reading through the code helped me learn what was going on, and partly because I enjoy Arduino and I think that APA will make a fine addition to the ecosystem.

For the most part, porting it over couldn't be easier. You simply add `apa.c` and `apa.h` as you would a [normal library](http://arduino.cc/en/Guide/Libraries). Then you add `#include <apa.h>` to the top of your sketch. 

The only real catch is defining the ports, within Arduino, but that, too, is straightforward and I'll show it below.

##Making APA work

APA comes with two sample boards and code to accompany them- one labelled *FTDI* and one labelled *IO*. These correspond to a board you talk to via USB, and a board that can talk to the FTDI board using APA and can change various inputs and outputs. 

In `./src`, I've included all of these files, so you can use them. They even have the traces so you can make the boards and play with them. 

We can recreate this code pretty easily on the Arduino, simply by connecting two together and writing a little code. Here is a simple diagram showing the wiring for this test:

![Two Arduinos](src\\apa_arduino\\Fritzing_Schematic_bb.png)

I've also included the Arduino sketches for the FTDI and IO boards. They're located under the 

We can test them out by hooking them up as in the picture above, uploading the code for each, and connecting your USB to the board labelled FTDI. Then just open up the serial monitor of your choice and wait a second for the system to boot.

You should see a message like `"Hello, I am an FTDI Arduino!"`. If you don't, you might have chosen the wrong port, or maybe the wrong baud rate (we're going to stick to a slow 9600). 

Regardless, once you get that we can send a test message to our system. In the top bar of the serial monitor, type something like `{1^|b}`. If everything is hooked up properly, you should get a response like `{0^|s}`

Now what exactly happened here? I'll go into the details over the next few paragraphs. 

###The APA Packet
First, lets talk about the APA packet. This is the string of text we sent over serial to the FTDI board. For instance, for the packet `{1^|b}` it consists of 3 parts. 

 1. The path: `1^`
 2. The divider: `|`
 3. The payload: `b`

These three parts are enclosed by brackets (`{}`) so that you and the arduino know when the packet is starting and when it is ending. The first part, the path, defines each port that the packet passes through on the way to its destination. It is separated from the data we want to send, the payload, by the divider. 

Think about how you give directions to someone. You usually say, `take a left, go for so many miles, then take a right`. Or maybe you say, `take a left, then take the second right after the In-N-Out`. These are similar to the path values, where each one can be thought of as `left` or `right`, but instead we encode them with a number. In a string like `^101` we are saying `go to port 1 on the next board, port 0 on the board after that, then finally port 1 on the third board`. The ports connect one port to another, so all you have to do is define a list of ports that traces out the path, and you're done. 

But what about this `^` character? The `^` is like a bookmark- it tells us where we are in the path, so we can keep the route we've already taken. This is what allows you to get the confirmation packet `{^0|s}` back from the second board. Every time the packet passes through a port, the `^` character is moved one space over to the right. So a path like `1021^101` that was located at port 0 would become `10210^01` once it moved from port 0 to port 1. Once `^` reaches `|`, the divider, we've officially reached our destination, and so the path looks like `1021010^`. What's even better is that, using this method, we also have the path to go back. We now have the full path that's required to go back. 





These boards do a great job of exhibiting a specific application, but as a platform for further growth they aren't that appropriate. For one, the FTDI board is too simple. It has one port dedicated to serial communication and and another dedicated to  its job is essentially to receive serial packets and, regardless of their content, pass them onto the nearest neighbor. 

The IO boards are slightly more complex, but most of the actual workings of the APA protocol is hidden under those `apa_port_scan()` methods.  

What if you want to talk to the FTDI board directly, or you want to generate packets on a board that you then pass to your neighbors? The process for doing this isn't exactly enumerated in these examples. The goal of this tutorial, then, is to take you from these first two examples to something that fully exhibits APA's potential.

##Expanding the FTDI Board
The first thing we can do is use the inherent functionality of the Arduinos to add a few features to the FTDI board. The FTDI board is strange because the 

