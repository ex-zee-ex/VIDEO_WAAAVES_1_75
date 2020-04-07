# VIDEO_WAAAVES_2
This is a video mixer, framebuffer delay, and feedback resynthesis engine built in openFrameworks https://openframeworks.cc/

(QUICK ADVERTISEMENT FOR MYSELF https://andreijaycreativecoding.com/ this is my website, there is a pay pal button on here, if you have a fun time with the software i make and can afford to send me a donation it is super appreciated, the more donations i recieve means the more time i can spend working on crazy awesome open source software)

## Version 2.0

The most notable thing about the 2.0 update is that i fixed all the stuff that i broke when i put midi controls in, new controls for the sharpening algorithm that allow for a lot more complex reaction diffusion patterns, and fucking like a million lfos everywhere.  

![Image description](https://github.com/ex-zee-ex/VIDEO_WAAAVES_1_5/blob/master/hypercuuube.png)

## Installing
OK! so installing from these files here on the github are kind of only for folks reasonably well versed in navigating openFrameworks already!  I can't really help anyone out anymore with this sort of thing as the volume of requests has greatly outpaced the amount of time I can afford to spend on that! But for everyone who has no idea what the heck an open frameworks is don't worry I jurry rigged a reasonable solution for yalls!  

### OSX
For OSX download the zip file here: \
https://drive.google.com/open?id=1rHX1LVA2dGAoOP51-NxUSUD_c47C0YVP

Unzip and follow the instructions in the txt file named "(()))00ReadThisTextFile!" and have fun!

### Windows
For windows download the zip file here:\
https://drive.google.com/open?id=187pYedjJMxqHuMWmBdHOd8ZdMoiEaqNF

Follow the instructions here for installing visual studio 2017 https://openframeworks.cc/setup/vs/ and then follow the instructions in the txt file named "(()))00ReadThisTextFile!" and have fun!

### Linux
Please note that this build guide is for debian-based distributions and is tested on a clean Ubuntu 19.10.\
Mostly the whole process consists of two parts - building OpenFrameworks project and hooking it up to Video_Waaaves.

First, install `git` and `make`.\
`sudo apt-get install build-essential git`

#### 1) Install `OpenFrameworks`.

Clone the project from github:\
`git clone --recursive --depth 1 https://github.com/openframeworks/openFrameworks.git`

Go to `openframeworks/scripts/linux/ubuntu` and run both scripts:
```
sudo ./install_codecs.sh
sudo ./install_dependencies.sh
```

Now go back to `openframeworks/scripts/linux` and run:\
`sudo ./download_libs.sh`

Additionally download 2 packages `ofxImGui` and `ofxMidi`.\
Just go to `openframeworks/addons` and from there run:
```
git clone https://github.com/jvcleave/ofxImGui.git
git clone https://github.com/danomatika/ofxMidi.git
```

Now in case you want to check if everything's all right, you can compile the project by running:\
`sudo ./compileOF.sh`\
But you will have to recompile it from Video_Waaaves in the next step anyway.

Regarding `OpenFrameworks`, that's about it.\
If you've encountered any kind of problems at this point, check the thorough manual on the project's github page. You can also try to download an (uncompiled!) release from their website.

#### 2) Install `Video_Waaaves_2.0`.

Clone the project from github (you may want to keep it on the same level as `OpenFrameworks` in order to set a relative path):\
`git clone https://github.com/ex-zee-ex/VIDEO_WAAAVES_2.git`

Copy everything from `VIDEO_WAAAVES_2/nosyphon` to the root folder `VIDEO_WAAAVES_2` and choose to replace everything.

Now open `Makefile` and set the path to the root folder of OpenFrameworks in the line `#9`:\
`OF_ROOT=$(realpath ../../..)` \
should be changed to something like \
`OF_ROOT=$(realpath /home/denis/video_projects/openframework)`\
or you can set a relative path `OF_ROOT=$(realpath ../openframework)` if both project are on the same level.

Run:\
`make`

Now you can just run `Video_Waaaves_2/bin/VIDEO_WAAAVES_2`.

### For Developers (and troubleshooting)
The project requires the following addons:
* ofxSyphon https://github.com/astellato/ofxSyphon, 
* ofxMidi https://github.com/danomatika/ofxMidi, 
* ofxImGui https://github.com/jvcleave/ofxImGui

For windows and linux check the `nosyphon` folder for alternate versions of some files to copy over into the src folder to get u up and running in syphon free zones

![Image description](https://github.com/ex-zee-ex/VIDEO_WAAAVES_1_5/blob/master/swirl.png)

If you have never used OpenFrameworks and/or Xcode before here are some troubleshooting tips to try before contacting me:
1. The entire folder structure needs to be intact and moved into the apps/myApps folder within the openframeworks folder
2. Try running project generator and importing this folder (with the abovementioned addons) if theres still errors
3. https://openframeworks.cc/setup/xcode/ has some advice on how to troubleshoot troublesome addons as well so check this out too!

![Image description](https://github.com/ex-zee-ex/VIDEO_WAAAVES_1_5/blob/master/vlcsnap-2019-08-10-22h55m38s489.png)

Numereous secret keyboard commands:\
to scale syphon input press[] and ;/

to rotate and shift framebuffer xyz positions a,z,s,x,d,c,f,v,g,b,h,n,t,y can all be used, the numerical key '3' resets all positions

the numerical key '1'clears the framebuffer

![Image description](https://github.com/ex-zee-ex/VIDEO_WAAAVES_1_5/blob/master/vlcsnap-2019-08-10-22h57m07s147.png)


check within the code for more tips on how to use this as a music visualizer, for playing video loops, and for midi mapping controls 

https://youtu.be/PYapmZSiSE4 is a handy dandy video tutorial on how to get started playing around in heres!

https://youtu.be/LNDmF9-AcW8 on how to use the audio visualizer and midi mapping functions

https://vimeo.com/andreijay for many examples of this program in action.  





midi cc list for video waaaves!

ch1 hue  20

ch1 saturation  21

ch1 brightness  22


blur -25

sharpen 24

fb0 key V 28

fb0 mix 29

fb0 delay 30

fb0 x  4

fb0 y  3

fb0 z  12

fb0 rotate 11

fb0 hue 5

fb0 sat 2

fb0 bright 13

fb0 huemod 16-not bipolar

fb0 hueoffset 10

fb0 huelfo 17

fb1 key V 31

fb1 mix 27  

fb1 delay 26

fb1 x  6

fb1 y  1

fb1 z  14

fb1 rotate 9

fb1 hue 7

fb1 sat 0

fb1 bright 15

fb1 huemod 18-not bipolar

fb1 hueoffset 8

fb1 huelfo 19

(yes i know that is far from all of the parameters.  I'll be frank, adding full midi support for every parameter would be pretty boring and time consuming and adds very little to my personal usage of this software!  if you want more midi ccs mapped to parameters you'll basically have to pay me because its pretty unlikely that i'll decide one afternoon "hmmm what i could i do that is boring and time consuming and gives me no personal satisfaction.  a paypal button can be found at andreijaycreativecoding.com)
