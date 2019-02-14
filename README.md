# OSC2DMX module design

The simplest possible Arduino OSC to DMX controller. This is useful for situations when you just need to control one (or a few) LED stage light(s) from a computer, and want to set the red, green and blue levels independently.

The Arduino design is simple and uses an ATmega328/2560 based Arduino (e.g., Arduino Uno/Mega) Ethernet shield and a commonly available MAX485 breakout board. It also supports a (not required) LCD screen to increase cool blinkenlights factor.

The Arduino gives itself an IP address (`192.168.1.100`) which can be changed in the code.

### OSC Message format

You send UDP messages in OSC format to the board, the following message format is supported

    /light iiii red green blue total_dim

i.e., four integers 0-255 corresponding to the red, green and blue channels respectively, and a total_dim channel that scales the other three.

### DMX output.

The DMX output just affects the first three channels, i.e.:

    red, green, blue, 0, 0, 0, 0,..., 0

So, you need to set your lights to start on channel 1, and to be in 3-channel mode, such that Ch.1 corresponds to red, Ch.2 to green, and Ch.3 to blue.

Other configurations are possible, you can change this in the code.

### Installation.

There's one Arduino script `lighting-module.ino`. The project is distributed in platformio format to handle installing libraries, but you can also use the Arduino IDE and install the Conceptinetics, Ethernet, and OSC libraries by hand.

### Testing with Pure Data

There's a Pd patch `OSC2DMX-test.pd` in the pd folder which shows how to control lights from Pd.
