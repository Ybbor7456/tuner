# Guitar Tuner – ESP32 #

# Intro #
Most musicians who have been exposed to music for long enough will have a decent perception of which note is being played, but few are able to attain perfect tuning. This is why there are tuning devices, to ensure that there is a consistent reference pitch. It is one thing to play an instrument by yourself and tune that instrument to itself. It may not be perfect tuning , but as long as the frequencies of each string/fret are similar enough, this will work for solo play. Though, things do get more complicated once solo playing evolves into a group ensemble or even playing alongside a soundtrack.
I didn’t always have an easy to use tuner when I first began playing. I’d tune my instrument to the out of tune piano. This became problematic as note frequencies are not equidistant from another, and having a piano with over 6 octaves can skew tuning results rather dramatically. As notes become higher, the linear differences between frequencies become larger, making more accomplished pieces that take use a wider array of frets sound less in tune. 
The microphone being used (INMP441) uses the I²S peripheral. I²S (Inter IC Sound) is a serial communication protocol that sends digital audio data. 


# Base Goal #
Create a display that will listen to sound pressure, convert it to frequency, display and compare results to the frequencies of the notes in standard guitar tuning. 


# Hardware (Current) #
INMP441 I²S microphone, ESP32 microcontroller

# Software #
Platformio/Arduino IDE.
Arduino IDE (working) and PlatformIO (work-in-progress). The same sketch currently reads/prints valid samples in Arduino IDE, but shows only zeros in PlatformIO; I’ll migrate to PlatformIO after the issue has been resolved. 



# Approach #
The INMP441 does not measure frequency directly, rather it outputs digital samples of sound pressure over periods of time. These samples must then be analyzed to estimate the frequency. 
