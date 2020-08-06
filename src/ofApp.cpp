/*
 re the midi biz
 * Copyright (c) 2013 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 */






/*thanks to ed tannenbaum for help in fixing up the framebuffer code!*/










//so what is going to be added in here
//try out the new hsb controls for the framebuffers and also test with the other channels
//how many shader runs can we do?  curious if it will be feasible to do seperate shader runs for all of the vertex displacements but i think that it would be a good goal to have in terms of making a universial function for that to reuse as much as possible!  plus a goal is to test how many shader passes i can run before noticing slowdown!





#include "ofApp.h"


#include <iostream>


//this fixes the total maximum delay time possible in frames
//
const int fbob=60;

//variables for the midi controllers
float c1=0;
float c2=0;
float c3=0;
float c4=0;
float c5=0;
float c6=0;
float c7=0;
float c8=0;
float c9=0;
float c10=0;
float c11=0;
float c12=0;
float c13=0;
float c14=0;
float c15=0;
float c16=0;
float c17=0;
float c18=0;
float c19=0;
float c20=0;
float c21=0;
float c22=0;
float c23=0;
float c24=0;
float c25=0;
float c26=0;
float c27=0;
float c28=0;
float c29=0;
float c30=0;
float c31=0;

//lfo thetas

//fb0
float fb0_theta_mix;
float fb0_theta_delay;
float fb0_theta_hue;
float fb0_theta_saturation;
float fb0_theta_bright;
float fb0_theta_huexmod;
float fb0_theta_huexoffset;
float fb0_theta_huexlfo;
float fb0_theta_x;
float fb0_theta_y;
float fb0_theta_z;
float fb0_theta_rotate;

//fb1
float fb1_theta_mix;
float fb1_theta_delay;
float fb1_theta_hue;
float fb1_theta_saturation;
float fb1_theta_bright;
float fb1_theta_huexmod;
float fb1_theta_huexoffset;
float fb1_theta_huexlfo;
float fb1_theta_x;
float fb1_theta_y;
float fb1_theta_z;
float fb1_theta_rotate;

//fb2
float fb2_theta_mix;
float fb2_theta_delay;
float fb2_theta_hue;
float fb2_theta_saturation;
float fb2_theta_bright;
float fb2_theta_huexmod;
float fb2_theta_huexoffset;
float fb2_theta_huexlfo;
float fb2_theta_x;
float fb2_theta_y;
float fb2_theta_z;
float fb2_theta_rotate;

//fb3
float fb3_theta_mix;
float fb3_theta_delay;
float fb3_theta_hue;
float fb3_theta_saturation;
float fb3_theta_bright;
float fb3_theta_huexmod;
float fb3_theta_huexoffset;
float fb3_theta_huexlfo;
float fb3_theta_x;
float fb3_theta_y;
float fb3_theta_z;
float fb3_theta_rotate;

ofPolyline tetrahedron;
//ofPolyline pnt_line;

float aa=0.0;
float ss=0.0;
float dd=0.0;
float ff=0.0;
float xw=1.01;
float yw=1.01;
float jj=1;
float kk=1;
float ll=.05;

float qq=0;
float ee=0;

float oo=1.0;
int ii=1;

float gg=0;
float hh=0;

float theta=0;

float frequency=0;


float amp=0;

float alpha=255;

float cam1_scale=1;
float scale1=1;
float scale2=1;

//vidmixer variables
float scale=.5;

float tt=0;

float midi_controls[127];


int framedelayoffset=0;  // this is used as an index to the circular framebuffers eeettt
unsigned int framecount=0; // framecounter used to calc offset eeettt


//i got this up to 240 frames with steady on my computer, is a bit skewed extreme for real time video
//executions tho
 // number of "framedelay" buffers eeettt
//int dd=fbob;
//this buffers the framebuffers from the final screen draws

//this buffers the framebuffers from the final screen draws
ofFbo pastFrames[fbob];

//ofFbo pastFrames_comp[fbob];
//ofFbo is the openframeworks framebuffer object
//this line is declaring an array of framebuffer objects
//many c++ afficionados prefer a c++ vector object instead of arrays every time
//because c++ vectors are like arrays but with some handier ways to access and add and delete contents
//but on the other hand vectors can have dynamic lengths and one can really end up in some
//trouble with dynamically working with framebuffers bc its generally best pratice to allocate
//memory on the gpu for all of yr framebuffers before yr code runs, trying to mess with that on
//the fly could have some interesting appeal to a glitch art style but is not going to please
//others

void incIndex()  // call this every frame to calc the offset eeettt
{
    
    framecount++;
    framedelayoffset=framecount % fbob;
}

//--------------------------------------------------------------
void ofApp::setup() {
    
    ofSetFrameRate(30);
   // ofDisableAlphaBlending();
    //ofSetVerticalSync(FALSE);
    
    //syphon input
    
    //mClient.set("","Black Syphon");
    mClient.setup();
    mClient.set("","Syphoner");
    //mClient.set("","Black Syphon");
    
    //syphonoutput
    mainOutputSyphonServer.setName("VIDEO_WAAAVES_screen");
    
    
    
    
    //audiovisualizer biz
    /*
    loop.load("sounds/5.wav");
    
    loop.setLoop(1);
    
    loop.setVolume(1);
    
    */
    
	ofSetVerticalSync(true);
	ofBackground(0);
	//ofSetLogLevel(OF_LOG_VERBOSE);
	
    
    /**MIDIBIX***/
    
	// print input ports to console
	midiIn.listInPorts();
	
	// open port by number (you may need to change this)
	midiIn.openPort(0);
	//midiIn.openPort("IAC Pure Data In");	// by name
	//midiIn.openVirtualPort("ofxMidiIn Input"); // open a virtual port
	
	// don't ignore sysex, timing, & active sense messages,
	// these are ignored by default
	midiIn.ignoreTypes(false, false, false);
	
	// add ofApp as a listener
	midiIn.addListener(this);
	
	// print received messages to the console
	midiIn.setVerbose(true);
    
    /*******/
    
    
    /**shaderobiz***/
    
#ifdef TARGET_OPENGLES

#else
    if(ofIsGLProgrammableRenderer()){
    
    }else{
   
        shader_mixer.load("shadersGL2/vertpass.vert", "shadersGL2/shader_mixer.frag");
        shader_blur.load("shadersGL2/vertpass.vert", "shadersGL2/shader_blur.frag");
        shader_sharpen.load("shadersGL2/vertpass.vert", "shadersGL2/shader_sharpen.frag");
		      
    }
#endif
    
   
    
 
    fbo_draw.allocate(ofGetWidth(), ofGetHeight());
    fbo_feedback.allocate(ofGetWidth(), ofGetHeight());
        syphonTexture.allocate(ofGetWidth(), ofGetHeight());
    fbo_blur.allocate(ofGetWidth(), ofGetHeight());
    
    fbo_feedback.begin();

    ofClear(0,0,0,255);
 
       fbo_feedback.end();
    
    fbo_draw.begin();
    ofClear(0,0,0,255);
    fbo_draw.end();
    
    fbo_blur.begin();
    ofClear(0,0,0,255);
    fbo_blur.end();
    
  
    
    
    //allocate and clear the variable delay final draw buffers
    //full resolution version
    
    for(int i=0;i<fbob;i++){
        
        pastFrames[i].allocate(ofGetWidth(), ofGetHeight());
        pastFrames[i].begin();
        ofClear(0,0,0,255);
        pastFrames[i].end();
        
    
    }//endifor
    
    
    
    
    
    
    for(int i=0;i<127;i++){
        midi_controls[i]=0;
    }
    
    
    
   
    
    //compressed by reducing the pixels by 1/4
    /*
    for(int i=0;i<fbob;i++){
        
        pastFrames_comp[i].allocate(ofGetWidth()/2, ofGetHeight()/2);
        pastFrames_comp[i].begin();
        ofClear(0,0,0,255);
        pastFrames_comp[i].end();
        
        
    }//endifor
    */
    
    
    //camerabiz
    //if u are having low framerates u can try lower resolution grabs
    //or if its all going smooth and u want to try higher resolutions here is where to fuck with that
    
    //add some optional UI interactivity with these
    //like a incrementer that cycles thru integers mod the lenght of the device list for selecting inputs cameras
    cam1.listDevices();
    cam1.setVerbose(true);
    cam1.setDeviceID(0);
     //cam1.initGrabber(1280, 960);
   cam1.initGrabber(640, 480);
   // cam1.initGrabber(320, 240);
    
    cam2.setDeviceID(1);
      // cam2.initGrabber(1920, 1080);
    cam2.initGrabber(640, 480);
    //cam2.initGrabber(320, 240);
    
   
    
    

    //setting up a tetrahedron
    
    ofVec3f tri1;
    ofVec3f tri2;
    ofVec3f tri3;
    ofVec3f tri4;
    tri1.set(1,1,1);
    tri2.set(-1,-1,1);
    tri3.set(-1,1,-1);
    tri4.set(1,-1,-1);
    float shapeScale=ofGetWidth()/8;
    
    tri1=tri1*shapeScale;
    tri2=tri2*shapeScale;
    tri3=tri3*shapeScale;
    tri4=tri4*shapeScale;
    
    
    tetrahedron.lineTo(tri1);
    tetrahedron.lineTo(tri2);
    tetrahedron.lineTo(tri4);
    tetrahedron.lineTo(tri1);
    tetrahedron.lineTo(tri3);
    tetrahedron.lineTo(tri2);
    tetrahedron.lineTo(tri3);
    tetrahedron.lineTo(tri4);
    //tetrahedron.lineTo(tri4);
    
    
    
    
   

    
}


//--------------------------------------------------------------
void ofApp::update() {
    
    //only update if active set a test for that too
    
    
    
    
    cam1.update();
    cam2.update();
    
    midibiz();
    
    //movie loop hack
    //uncomment this part to update the movie looping thing
    //theres another step so command f movie loop hack for the next one
    //loopMovie.update();
    
    
    
    //audiovisualizer biz
    /*
    frequencyLine0.clear();
      frequencyLine1.clear();
      frequencyLine2.clear();
      frequencyLine3.clear();
    float * val = ofSoundGetSpectrum(nBandsToGet);		// request 128 values for fft
    for (int i = 0;i < nBandsToGet; i++){
        
        // let the smoothed value sink to zero:
        fftSmoothed[i] *= 0.99f;
        
        // take the max, either the smoothed or the incoming:
        if (fftSmoothed[i] < val[i]) fftSmoothed[i] = val[i];
        int y=fftSmoothed[i] * ofGetHeight()/8*c2;
        int x=ofMap(i,0,nBandsToGet,0,ofGetWidth()/2);
        frequencyLine0.addVertex(x,y);
        frequencyLine1.addVertex(x,-y);
        frequencyLine2.addVertex(-x,-y);
        frequencyLine3.addVertex(-x,y);
        
    }
     
    
    */
    
    
   //  plane.set(ofGetWidth(), ofGetHeight(), ofGetWidth()/4, ofGetHeight()/4, OF_PRIMITIVE_TRIANGLE_STRIP);
    
    //perlin noise terrain biz
      //  pnt_img.update();
    
    
    
    //lfobiz
    //probably turnthis into a function
    //tho not like a legal function lol
    //unless i put everything into an array or something
    //and can just pass a pointer
    //is it still a function if yr passing pointer/?
    //i don't think so
    
    //either way this is where we will increment the thetas
    
    
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    

  
 
    
    /***shaderbix**/
    
    
   //bind the syphon input to a texture.  i'm pretty sure there's a better way to do this but here we hare
    syphonTexture.begin();
 
    mClient.draw(0,0,scale1*gui->syphon_scale*mClient.getWidth(),scale2*gui->syphon_scale*mClient.getHeight());
    
    syphonTexture.end();
    
 
   
    fbo_draw.begin();
    
    
    //try putting graphics up here instead and see if then we can key into opaqueness
    
    
    
    
    
    
    shader_mixer.begin();
    
    
    
    
    
    
    
    shader_mixer.setUniform1f("width", ofGetWidth());
    shader_mixer.setUniform1f("height", ofGetHeight());
	shader_mixer.setUniform2f("center", ofGetWidth()/2., ofGetHeight()/2.);
    shader_mixer.setUniform1f("cam1_scale", gui->cam1_scale);
    shader_mixer.setUniform1f("cam2_scale", gui->cam2_scale);
    
    
    
    
    
    
    //fb0 lfos
    fb0_theta_mix+=.01*(gui->fb0_mix_lfo_theta);
    fb0_theta_delay+=.01*(gui->fb0_delay_lfo_theta);
    fb0_theta_hue+=.01*(gui->fb0_hue_lfo_theta);
    fb0_theta_saturation+=.01*(gui->fb0_saturation_lfo_theta);
    fb0_theta_bright+=.01*(gui->fb0_bright_lfo_theta);
    
    fb0_theta_huexmod+=.01*(gui->fb0_huexmod_lfo_theta);
    fb0_theta_huexoffset+=.01*(gui->fb0_huexoffset_lfo_theta);
    fb0_theta_huexlfo+=.01*(gui->fb0_huexlfo_lfo_theta);
    
    fb0_theta_x+=.01*(gui->fb0_x_lfo_theta);
    fb0_theta_y+=.01*(gui->fb0_y_lfo_theta);
    fb0_theta_z+=.01*(gui->fb0_z_lfo_theta);
    fb0_theta_rotate+=.01*(gui->fb0_rotate_lfo_theta);
    
    //fb1 lfos
    fb1_theta_mix+=.01*(gui->fb1_mix_lfo_theta);
    fb1_theta_delay+=.01*(gui->fb1_delay_lfo_theta);
    fb1_theta_hue+=.01*(gui->fb1_hue_lfo_theta);
    fb1_theta_saturation+=.01*(gui->fb1_saturation_lfo_theta);
    fb1_theta_bright+=.01*(gui->fb1_bright_lfo_theta);
    
    fb1_theta_huexmod+=.01*(gui->fb1_huexmod_lfo_theta);
    fb1_theta_huexoffset+=.01*(gui->fb1_huexoffset_lfo_theta);
    fb1_theta_huexlfo+=.01*(gui->fb1_huexlfo_lfo_theta);
    
    fb1_theta_x+=.01*(gui->fb1_x_lfo_theta);
    fb1_theta_y+=.01*(gui->fb1_y_lfo_theta);
    fb1_theta_z+=.01*(gui->fb1_z_lfo_theta);
    fb1_theta_rotate+=.01*(gui->fb1_rotate_lfo_theta);
    
    //fb2 lfos
    fb2_theta_mix+=.01*(gui->fb2_mix_lfo_theta);
    fb2_theta_delay+=.01*(gui->fb2_delay_lfo_theta);
    fb2_theta_hue+=.01*(gui->fb2_hue_lfo_theta);
    fb2_theta_saturation+=.01*(gui->fb2_saturation_lfo_theta);
    fb2_theta_bright+=.01*(gui->fb2_bright_lfo_theta);
    
    fb2_theta_huexmod+=.01*(gui->fb2_huexmod_lfo_theta);
    fb2_theta_huexoffset+=.01*(gui->fb2_huexoffset_lfo_theta);
    fb2_theta_huexlfo+=.01*(gui->fb2_huexlfo_lfo_theta);
    
    fb2_theta_x+=.01*(gui->fb2_x_lfo_theta);
    fb2_theta_y+=.01*(gui->fb2_y_lfo_theta);
    fb2_theta_z+=.01*(gui->fb2_z_lfo_theta);
    fb2_theta_rotate+=.01*(gui->fb2_rotate_lfo_theta);
    
    //fb3 lfos
    
    fb3_theta_mix+=.01*(gui->fb3_mix_lfo_theta);
    fb3_theta_delay+=.01*(gui->fb3_delay_lfo_theta);
    fb3_theta_hue+=.01*(gui->fb3_hue_lfo_theta);
    fb3_theta_saturation+=.01*(gui->fb3_saturation_lfo_theta);
    fb3_theta_bright+=.01*(gui->fb3_bright_lfo_theta);
    
    fb3_theta_huexmod+=.01*(gui->fb3_huexmod_lfo_theta);
    fb3_theta_huexoffset+=.01*(gui->fb3_huexoffset_lfo_theta);
    fb3_theta_huexlfo+=.01*(gui->fb3_huexlfo_lfo_theta);
    
    fb3_theta_x+=.01*(gui->fb3_x_lfo_theta);
    fb3_theta_y+=.01*(gui->fb3_y_lfo_theta);
    fb3_theta_z+=.01*(gui->fb3_z_lfo_theta);
    fb3_theta_rotate+=.01*(gui->fb3_rotate_lfo_theta);

    ofVec3f hsb_x;
    ofVec3f hue_x;
    ofVec3f fb_modswitch;
    ofVec3f fb_rescale;
    //fb0
    
    
    shader_mixer.setUniform1f("fb0lumakeyvalue", gui->fb0_key_value+c6);
    shader_mixer.setUniform1f("fb0lumakeythresh", 1);
    shader_mixer.setUniform1i("fb0mix", 2);
    shader_mixer.setUniform1i("fb0_toroid_switch", gui->fb0_toroid_switch);
    
    
    
    shader_mixer.setUniform1f("fb0blend", gui->fb0_mix+lfo(gui->fb0_mix_lfo_amp,fb0_theta_mix,0)+c7*2.0f);
    
    hsb_x.set((gui->fb0_hue+lfo(gui->fb0_hue_lfo_amp,fb0_theta_hue,0))/10+c13*.2f,
              (gui->fb0_saturation+lfo(gui->fb0_saturation_lfo_amp,fb0_theta_saturation,0))/10+c14*.2f,
              (gui->fb0_bright+lfo(gui->fb0_bright_lfo_amp,fb0_theta_bright,0))/10+c15*.2f);
   
    shader_mixer.setUniform3f("fb0_hsb_x",hsb_x);
    
    hue_x.set((gui->fb0_huex_mod+lfo(gui->fb0_huexmod_lfo_amp,fb0_theta_huexmod,0))/10-c16,
              (gui->fb0_huex_offset+lfo(gui->fb0_huexoffset_lfo_amp,fb0_theta_huexoffset,0))/10+c17*2.0f,
              (gui->fb0_huex_lfo+lfo(gui->fb0_huexlfo_lfo_amp,fb0_theta_huexlfo,0))/10+c18*2.0f);
    shader_mixer.setUniform3f("fb0_hue_x",hue_x);
    
   
     
    
    fb_modswitch.set(gui->fb0_hue_invert,gui->fb0_saturation_invert,gui->fb0_bright_invert);
    shader_mixer.setUniform3f("fb0_modswitch",fb_modswitch);
    
    
    shader_mixer.setUniform3f("fb0_rescale",ofVec3f(gui->fb0_x_displace+lfo(gui->fb0_x_lfo_amp,fb0_theta_x,0)+c9*40.0f,
                                                    gui->fb0_y_displace+lfo(gui->fb0_y_lfo_amp,fb0_theta_y,0)+c10*40.0f,
                                                    gui->fb0_z_displace/100+lfo(gui->fb0_z_lfo_amp,fb0_theta_z,0))+c11);
    shader_mixer.setUniform1f("fb0_rotate",(gui->fb0_rotate)+lfo(gui->fb0_rotate_lfo_amp,fb0_theta_rotate,0)+c12*6.18f);
    
    //fb1
    
    shader_mixer.setUniform1f("fb1lumakeyvalue", gui->fb1_key_value+c19);
    shader_mixer.setUniform1f("fb1lumakeythresh", 1);
    shader_mixer.setUniform1i("fb1mix", 2);
    shader_mixer.setUniform1i("fb1_toroid_switch", gui->fb1_toroid_switch);
    
    
    shader_mixer.setUniform1f("fb1blend", gui->fb1_mix+lfo(gui->fb1_mix_lfo_amp,fb1_theta_mix,0)+c20*2.0f);
    
    hsb_x.set((gui->fb1_hue+lfo(gui->fb1_hue_lfo_amp,fb1_theta_hue,0))/10+c26*.2f,
              (gui->fb1_saturation+lfo(gui->fb1_saturation_lfo_amp,fb1_theta_saturation,0))/10+c27*.2f,
              (gui->fb1_bright+lfo(gui->fb1_bright_lfo_amp,fb1_theta_bright,0))/10+c28*.2f);
    
    shader_mixer.setUniform3f("fb1_hsb_x",hsb_x);
    
    hue_x.set((gui->fb1_huex_mod+lfo(gui->fb1_huexmod_lfo_amp,fb1_theta_huexmod,0))/10-c29,
              (gui->fb1_huex_offset+lfo(gui->fb1_huexoffset_lfo_amp,fb1_theta_huexoffset,0))/10+c30*2.0f,
              (gui->fb1_huex_lfo+lfo(gui->fb1_huexlfo_lfo_amp,fb1_theta_huexlfo,0))/10+c31*2.0f);
    shader_mixer.setUniform3f("fb1_hue_x",hue_x);
    
    fb_modswitch.set(gui->fb1_hue_invert,gui->fb1_saturation_invert,gui->fb1_bright_invert);
    shader_mixer.setUniform3f("fb1_modswitch",fb_modswitch);
   
    shader_mixer.setUniform3f("fb1_rescale",ofVec3f(gui->fb1_x_displace+lfo(gui->fb1_x_lfo_amp,fb1_theta_x,0)+c22*40.0f,
                                                    gui->fb1_y_displace+lfo(gui->fb1_y_lfo_amp,fb1_theta_y,0)+c23*40.0f,
                                                    gui->fb1_z_displace/100+lfo(gui->fb1_z_lfo_amp,fb1_theta_z,0))+c24);
    shader_mixer.setUniform1f("fb1_rotate",(gui->fb1_rotate)+lfo(gui->fb1_rotate_lfo_amp,fb1_theta_rotate,0)+c12*6.18f);
    
    //fb2
    
   
    shader_mixer.setUniform1f("fb2lumakeyvalue", gui->fb2_key_value);
    shader_mixer.setUniform1f("fb2lumakeythresh", 1);
    shader_mixer.setUniform1i("fb2mix", 2);
    shader_mixer.setUniform1i("fb2_toroid_switch", gui->fb2_toroid_switch);
    
    shader_mixer.setUniform1f("fb2blend", gui->fb2_mix+lfo(gui->fb2_mix_lfo_amp,fb2_theta_mix,0));
    
    hsb_x.set((gui->fb2_hue+lfo(gui->fb2_hue_lfo_amp,fb2_theta_hue,0))/10,
              (gui->fb2_saturation+lfo(gui->fb2_saturation_lfo_amp,fb2_theta_saturation,0))/10,
              (gui->fb2_bright+lfo(gui->fb2_bright_lfo_amp,fb2_theta_bright,0))/10);
    
    shader_mixer.setUniform3f("fb2_hsb_x",hsb_x);
    
    hue_x.set((gui->fb2_huex_mod+lfo(gui->fb2_huexmod_lfo_amp,fb2_theta_huexmod,0))/10,
              (gui->fb2_huex_offset+lfo(gui->fb2_huexoffset_lfo_amp,fb2_theta_huexoffset,0))/10,
              (gui->fb2_huex_lfo+lfo(gui->fb2_huexlfo_lfo_amp,fb2_theta_huexlfo,0))/10);
    shader_mixer.setUniform3f("fb2_hue_x",hue_x);
    
    fb_modswitch.set(gui->fb2_hue_invert,gui->fb2_saturation_invert,gui->fb2_bright_invert);
    shader_mixer.setUniform3f("fb2_modswitch",fb_modswitch);
    
    shader_mixer.setUniform3f("fb2_rescale",ofVec3f(gui->fb2_x_displace+lfo(gui->fb2_x_lfo_amp,fb2_theta_x,0),
                                                    gui->fb2_y_displace+lfo(gui->fb2_y_lfo_amp,fb2_theta_y,0),
                                                    gui->fb2_z_displace/100+lfo(gui->fb2_z_lfo_amp,fb2_theta_z,0)));
    shader_mixer.setUniform1f("fb2_rotate",(gui->fb2_rotate)+lfo(gui->fb2_rotate_lfo_amp,fb2_theta_rotate,0));
    
    
    //fb3
   
    shader_mixer.setUniform1f("fb3lumakeyvalue", gui->fb3_key_value);
    shader_mixer.setUniform1f("fb3lumakeythresh", 1);
    shader_mixer.setUniform1i("fb3mix", 2);
    shader_mixer.setUniform1i("fb3_toroid_switch", gui->fb3_toroid_switch);
    
    shader_mixer.setUniform1f("fb3blend", gui->fb3_mix+lfo(gui->fb3_mix_lfo_amp,fb3_theta_mix,0));
    
    hsb_x.set((gui->fb3_hue+lfo(gui->fb3_hue_lfo_amp,fb3_theta_hue,0))/10,
              (gui->fb3_saturation+lfo(gui->fb3_saturation_lfo_amp,fb3_theta_saturation,0))/10,
              (gui->fb3_bright+lfo(gui->fb3_bright_lfo_amp,fb3_theta_bright,0))/10);
    
    shader_mixer.setUniform3f("fb3_hsb_x",hsb_x);
    
    hue_x.set((gui->fb3_huex_mod+lfo(gui->fb3_huexmod_lfo_amp,fb3_theta_huexmod,0))/10,
              (gui->fb3_huex_offset+lfo(gui->fb3_huexoffset_lfo_amp,fb3_theta_huexoffset,0))/10,
              (gui->fb3_huex_lfo+lfo(gui->fb3_huexlfo_lfo_amp,fb3_theta_huexlfo,0))/10);
    shader_mixer.setUniform3f("fb3_hue_x",hue_x);
    
    fb_modswitch.set(gui->fb3_hue_invert,gui->fb3_saturation_invert,gui->fb3_bright_invert);
    shader_mixer.setUniform3f("fb3_modswitch",fb_modswitch);
    
    shader_mixer.setUniform3f("fb3_rescale",ofVec3f(gui->fb3_x_displace+lfo(gui->fb3_x_lfo_amp,fb3_theta_x,0),
                                                    gui->fb3_y_displace+lfo(gui->fb3_y_lfo_amp,fb3_theta_y,0),
                                                    gui->fb3_z_displace/100+lfo(gui->fb3_z_lfo_amp,fb3_theta_z,0)));
    shader_mixer.setUniform1f("fb3_rotate",(gui->fb3_rotate)+lfo(gui->fb3_rotate_lfo_amp,fb3_theta_rotate,0));
    
    
    
    
    shader_mixer.setUniform1f("ee",ee);
    
    
    //here is where controls from the gui get shunted
    
    //for now channel 1 and 2 can only b cam1 or cam2 input
    shader_mixer.setUniform1i("channel1", gui->channel1_select);
    shader_mixer.setUniform1i("channel2", gui->channel2_select);
    shader_mixer.setUniform1i("mix1", 2);//gui->mix1);
    //shader_mixer.setUniform1i("mix2", gui->mix2);
    
    
    //mix1 controls from the gui
    shader_mixer.setUniform1f("mix1blend1", gui->ch2_mix);
    
    
    
    //shader_mixer.setUniform1f("mix1keythresh", gui->mix1lumakeythresh);
     shader_mixer.setUniform1f("mix1keybright",gui->ch2_key_value);
    
    
    
    
    
    
    
    
    //h and v flip controls
    //no shader_mixer.setUniform1i("ch1_h_mirror", gui->ch1_h_mirror);
    
    
    //channel1 controls from the gui
    //vector these up
    ///asuming these will all stay
    
    //ch1
    //vectorize all this
    
    shader_mixer.setUniform1f("channel1hue_x", gui->ch1_hue+c1*5.0);
    shader_mixer.setUniform1f("channel1saturation_x", gui->ch1_saturation+c2*5.0);
    shader_mixer.setUniform1f("channel1bright_x", gui->ch1_bright+c3*5.0);
    
    shader_mixer.setUniform1i("channel1satwrap", gui->ch1_saturation_wrap);
    shader_mixer.setUniform1i("channel1brightwrap", gui->ch1_bright_wrap);
    
    shader_mixer.setUniform1i("ch1hue_powmaptoggle", gui->ch1_hue_powmap_toggle);
    shader_mixer.setUniform1i("ch1sat_powmaptoggle", gui->ch1_saturation_powmap_toggle);
    shader_mixer.setUniform1i("ch1bright_powmaptoggle", gui->ch1_bright_powmap_toggle);
    
    shader_mixer.setUniform1i("ch1hue_inverttoggle", gui->ch1_hue_alt_invert_toggle);
    shader_mixer.setUniform1i("ch1sat_inverttoggle", gui->ch1_saturation_alt_invert_toggle);
    shader_mixer.setUniform1i("ch1bright_inverttoggle", gui->ch1_bright_alt_invert_toggle);
    
    
    shader_mixer.setUniform1f("channel1bright_powmap", gui->ch1_bright_powmap);
    shader_mixer.setUniform1f("channel1hue_powmap", gui->ch1_hue_powmap);
    shader_mixer.setUniform1f("channel1sat_powmap", gui->ch1_saturation_powmap);
    
    
    
    //ch2
    shader_mixer.setUniform1f("channel2bright_x", gui->ch2_bright);
    shader_mixer.setUniform1f("channel2hue_x", gui->ch2_hue);
    shader_mixer.setUniform1f("channel2saturation_x", gui->ch2_saturation);
    
    
    
    shader_mixer.setUniform1i("channel2satwrap", gui->ch2_saturation_wrap);
    shader_mixer.setUniform1i("channel2brightwrap", gui->ch2_bright_wrap);
    
    
    shader_mixer.setUniform1i("ch2hue_powmaptoggle", gui->ch2_hue_powmap_toggle);
    shader_mixer.setUniform1i("ch2sat_powmaptoggle", gui->ch2_saturation_powmap_toggle);
    shader_mixer.setUniform1i("ch2bright_powmaptoggle", gui->ch2_bright_powmap_toggle);
    
    shader_mixer.setUniform1i("ch2hue_inverttoggle", gui->ch2_hue_alt_invert_toggle);
    shader_mixer.setUniform1i("ch2sat_inverttoggle", gui->ch2_saturation_alt_invert_toggle);
    shader_mixer.setUniform1i("ch2bright_inverttoggle", gui->ch2_bright_alt_invert_toggle);
    
    shader_mixer.setUniform1f("channel2bright_powmap", gui->ch2_bright_powmap);
    shader_mixer.setUniform1f("channel2hue_powmap", gui->ch2_hue_powmap);
    shader_mixer.setUniform1f("channel2sat_powmap", gui->ch2_saturation_powmap);
    
    
    
    
    
    
    
    
    
    
    
    
    shader_mixer.setUniform2f("cam1dimensions",ofVec2f(cam1.getWidth(),cam1.getHeight()));
    shader_mixer.setUniform2f("cam2dimensions",ofVec2f(cam2.getWidth(),cam2.getHeight()));
    
    
    
    
    
   
    
    
    
   
    
    
    
    
    
    
    //h and v flips
    shader_mixer.setUniform1i("cam1_hflip_switch", gui->cam1_hflip_switch);
    shader_mixer.setUniform1i("cam1_vflip_switch", gui->cam1_vflip_switch);
    shader_mixer.setUniform1i("cam2_hflip_switch", gui->cam2_hflip_switch);
    shader_mixer.setUniform1i("cam2_vflip_switch", gui->cam2_vflip_switch);
    shader_mixer.setUniform1i("fb0_hflip_switch", gui->fb0_hflip_switch);
    shader_mixer.setUniform1i("fb0_vflip_switch", gui->fb0_vflip_switch);
    shader_mixer.setUniform1i("fb1_hflip_switch", gui->fb1_hflip_switch);
    shader_mixer.setUniform1i("fb1_vflip_switch", gui->fb1_vflip_switch);
    shader_mixer.setUniform1i("fb2_hflip_switch", gui->fb2_hflip_switch);
    shader_mixer.setUniform1i("fb2_vflip_switch", gui->fb2_vflip_switch);
    shader_mixer.setUniform1i("fb3_hflip_switch", gui->fb3_hflip_switch);
    shader_mixer.setUniform1i("fb3_vflip_switch", gui->fb3_vflip_switch);
    
    
    
    
    //pixelations
    
    //cam1
    shader_mixer.setUniform1i("cam1_pixel_switch",gui->cam1_pixel_switch);
    shader_mixer.setUniform1i("cam1_pixel_scale",gui->cam1_pixel_scale);
    shader_mixer.setUniform1f("cam1_pixel_mix",gui->cam1_pixel_mix);
    shader_mixer.setUniform1f("cam1_pixel_brightscale",gui->cam1_pixel_brightscale);
    
    //cam2
    shader_mixer.setUniform1i("cam2_pixel_switch",gui->cam2_pixel_switch);
    shader_mixer.setUniform1i("cam2_pixel_scale",gui->cam2_pixel_scale);
    shader_mixer.setUniform1f("cam2_pixel_mix",gui->cam2_pixel_mix);
    shader_mixer.setUniform1f("cam2_pixel_brightscale",gui->cam2_pixel_brightscale);
    
    //fb0
    shader_mixer.setUniform1i("fb0_pixel_switch",gui->fb0_pixel_switch);
    shader_mixer.setUniform1i("fb0_pixel_scale",gui->fb0_pixel_scale);
    shader_mixer.setUniform1f("fb0_pixel_mix",gui->fb0_pixel_mix);
    shader_mixer.setUniform1f("fb0_pixel_brightscale",gui->fb0_pixel_brightscale);
    //fb1
    shader_mixer.setUniform1i("fb1_pixel_switch",gui->fb1_pixel_switch);
    shader_mixer.setUniform1i("fb1_pixel_scale",gui->fb1_pixel_scale);
    shader_mixer.setUniform1f("fb1_pixel_mix",gui->fb1_pixel_mix);
    shader_mixer.setUniform1f("fb1_pixel_brightscale",gui->fb1_pixel_brightscale);
    
    //fb2
    shader_mixer.setUniform1i("fb2_pixel_switch",gui->fb2_pixel_switch);
    shader_mixer.setUniform1i("fb2_pixel_scale",gui->fb2_pixel_scale);
    shader_mixer.setUniform1f("fb2_pixel_mix",gui->fb2_pixel_mix);
    shader_mixer.setUniform1f("fb2_pixel_brightscale",gui->fb2_pixel_brightscale);
    
    //fb3
    shader_mixer.setUniform1i("fb3_pixel_switch",gui->fb3_pixel_switch);
    shader_mixer.setUniform1i("fb3_pixel_scale",gui->fb3_pixel_scale);
    shader_mixer.setUniform1f("fb3_pixel_mix",gui->fb3_pixel_mix);
    shader_mixer.setUniform1f("fb3_pixel_brightscale",gui->fb3_pixel_brightscale);
    
    
    
    //--------------------------send the textures
    
    
    //this gets bound to tex0 when u do this way
    //for some reason i can't just erase this and move on
    //so replace this with a useful texture at some point
    fbo_feedback.draw(0,0);
    //for some reason i get weird glitches in acessing textures in gl2 when i don't bind a texture like this before sending the others in more formally by setUniformTexture so if you understand why that is happening please let me know
    
    
    //set a conditional statement of which fbo
    shader_mixer.setUniformTexture("syphon",syphonTexture.getTexture(),1);
    shader_mixer.setUniformTexture("cam1",cam1.getTexture(),2);
    shader_mixer.setUniformTexture("cam2",cam2.getTexture(),3);
    
    
    // int delay0=gui->fb0delayamnt;
    //int delay1=gui->fb1delayamnt;
    // int delay2=gui->fb2delayamnt;
    // int delay3=gui->fb3delayamnt;
    
    
    
   
    //  shader_mixer.setUniformTexture("fb0",pastFrames[(abs(framedelayoffset-fbob-delay0))%fbob].getTexture(),4);
    
    shader_mixer.setUniformTexture("fb0",pastFrames[(abs(framedelayoffset-fbob-gui->fb0_delay_amount-(int(fbob*c8))-abs(int(fbob*lfo(gui->fb0_delay_lfo_amp,fb0_theta_delay,0))))-1)%fbob].getTexture(),4);
    shader_mixer.setUniformTexture("fb1",pastFrames[(abs(framedelayoffset-fbob-gui->fb1_delay_amount-(int(fbob*c21))-abs(int(fbob*lfo(gui->fb1_delay_lfo_amp,fb1_theta_delay,0))))-1)%fbob].getTexture(),5);
    shader_mixer.setUniformTexture("fb2",pastFrames[(abs(framedelayoffset-fbob-gui->fb2_delay_amount-abs(int(fbob*lfo(gui->fb2_delay_lfo_amp,fb2_theta_delay,0))))-1)%fbob].getTexture(),6);
    shader_mixer.setUniformTexture("fb3",pastFrames[(abs(framedelayoffset-fbob-gui->fb3_delay_amount-abs(int(fbob*lfo(gui->fb3_delay_lfo_amp,fb3_theta_delay,0))))-1)%fbob].getTexture(),7);
    
    
    
    //testing syphon as a texture sent into a channel
    //currently replacing cam2
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    shader_mixer.setUniform1f("qq",qq);
    
    shader_mixer.end();
    
    
    //this bit is for just testing stuff with a pretty little rotating square
    //you can just put whatever graphical code you want in this section and it will draw over everything
    
    // pnt_img.draw(0,0,ofGetWidth(),ofGetHeight());
    
    //additive vector synth zones
    
    /*
     ofPushMatrix();
     ofTranslate(ofGetWidth()/2,ofGetHeight()/2);
     
     ofFill();
     ofColor c=(0);
     c.setHsb(127*(1+.5*sin(theta/100)),255,255);
     ofSetColor(c);
     // ofSetColor(255);
     float radius=ofGetWidth()/4;
     
     for(float i=0;i<TWO_PI;i+=.01){
     
     int numberOfHarmonics=16;
     float polar_x=0;
     float polar_y=0;
     float polar_z=0;
     for(int j=1;j<numberOfHarmonics;j++){
     //polar_x+=cos(j*i)*((1-amp)+amp*sin(theta-j));
     //polar_y+=sin(j*i)*((1-amp)+amp*cos(theta-j));
     
     //polar_x+=cos(2*j*i)/(2*j);
     //polar_y+=sin(2*j*i)/(2*j);
     
     polar_x+=cos(i*j-theta)*sin(theta);
     polar_y+=sin(i*j+theta)*cos(theta);
     // polar_z+=cos(theta*i);
     
     }//endjfor
     
     theta+=.001;
     polar_x=radius*polar_x/numberOfHarmonics;
     polar_y=radius*polar_y/numberOfHarmonics;
     
     ofPushMatrix();
     
     ofTranslate(polar_x,polar_y,polar_z);
     ofDrawRectangle(0,0,1,1);
     ofPopMatrix();
     }//endifor
     
     ofPopMatrix();
     
     */
    
    
    //perlin noise terrain experiment
    
    
    
    
    /*
     
     float noiseVel = ofGetElapsedTimef()/16;
     
     int scale=40;
     int w = ofGetWidth()/scale;
     int h = ofGetHeight()/scale;
     // ofNoFill();
     int xoff=0;
     int yoff=0;
     ofPushMatrix();
     ofTranslate(ofGetWidth()/2,ofGetHeight()/2);
     ofRotateXRad(PI/3);
     for(int y=0; y<h; y++) {
     yoff+=.1;
     for(int x=0; x<w; x++) {
     xoff+=.1;
     int i = y * w + x;
     float noiseValue = ofNoise(x * .1+xoff, y * .1+yoff, noiseVel);
     
     ofSetColor(255);
     pnt_line.lineTo(x*scale-ofGetWidth()/2,y*scale-ofGetHeight()/2,50*noiseValue);
     
     
     }//endxfor
     pnt_line.draw();
     pnt_line.clear();
     }//endyfor
     
     ofPopMatrix();
     
     */
    
    
    if(gui->tetrahedron_switch==1){
        
        ofSetColor(127+127*(sin(ofGetElapsedTimef())),127+127*(cos(ofGetElapsedTimef()/7)),127-127*(sin(ofGetElapsedTimef()/19)),255);
        ofNoFill();
        ofPushMatrix();
        ofTranslate(ofGetWidth()/2,ofGetHeight()/2);
        ofRotateZRad(ofGetElapsedTimef()/5);
        ofRotateYRad(ofGetElapsedTimef()/13);
        ofRotateXRad(ofGetElapsedTimef()/11);
        tetrahedron.draw();
        
        
        
        ofPopMatrix();
    }
    
    
    fbo_draw.end();
    
    //----------------------------------------------------------
    
    
    
    //sharpen and blur the composited image before it is drawn to screens and buffers
    
    fbo_blur.begin();
	shader_blur.begin();
	shader_blur.setUniform1f("blurAmnt", gui->blur_amount + c4 * 10);//+5*(midi_controls[25]+1)/2);
	fbo_draw.draw(0,0);
    shader_blur.end();
    
    
    fbo_blur.end();
    
    
    
    fbo_draw.begin();
    
    /*
     shader_blur.begin();
     fbo_blur.draw(0,0);
     shader_blur.setUniform1f("blurAmnt",gui->blur_amount);
    shader_blur.end();
    */
     
     
    //so add a radius and chi variable to this
    //then figure out how to switch on and off and route properly
   
    shader_sharpen.begin();
	shader_sharpen.setUniform1f("sharpAmnt", gui->sharpen_amount + c5 * .3);//+.3*(midi_controls[24]+1.0)/2.0);
	shader_sharpen.setUniform1f("sharpenStep", gui->sharpen_radius);
	shader_sharpen.setUniform1f("sharpen_boost", gui->sharpen_boost + c5);
	shader_sharpen.setUniform1f("chi", gui->sharpen_chi);

	fbo_blur.draw(0,0);
	
    shader_sharpen.end();
   
    
    //fbo_blur.draw(0,0);
    
    fbo_draw.end();
    
    
    //___--_------___-_-_______-----___-
    
    /*this part gets drawn to screen*/
    ofSetColor(ofColor::white);
    
    ofPushMatrix();
    //add seperate switchs for this and fbo
    ofTranslate(ofGetWidth()/2.0,ofGetHeight()/2.0);
    
  
    fbo_draw.draw(-ofGetWidth()/2.0,-ofGetHeight()/2.0);
    ofPopMatrix();
    //audiovisualizer biz
  //feed the previous frame into position 0 (index0)
    
    pastFrames[abs(fbob-framedelayoffset)-1].begin(); //eeettt
    
    
    ofPushMatrix();
  
    //recenter the coordinates so 0,0 is at the center of the screen
    ofTranslate(ofGetWidth()/2,ofGetHeight()/2,0);
   
    //  ofRotateZRad(.01);
    ofTranslate(ff,gg,hh);
    ofRotateYRad(ss+gui->y_skew);
    ofRotateXRad(aa+gui->x_skew);
    ofRotateZRad(dd);
    ofRotateZRad(oo*TWO_PI/ii);
    
    
    fbo_draw.draw(-ofGetWidth()/2,-ofGetHeight()/2);
    
    //switch for the dry feed
    //cam1.draw(-ofGetWidth()/2,-ofGetHeight()/2,2*cam1.getWidth(),2*cam1.getHeight());
    ofPopMatrix();
    
    
    pastFrames[abs(fbob-framedelayoffset)-1].end(); //eeettt
    //-----____---____---__-__---____-----_--_-
    

    ofSetHexColor(0xFFFFFF);
  //  ofDrawBitmapString(" delayhead: "+ofToString(delayhead),10, ofGetHeight()-5 );
   
 
    //add a switch for this
    //if(gui->syphonOutput==1){
        mainOutputSyphonServer.publishScreen();
    //}
    
    incIndex(); // increment framecount and framedelayoffset eeettt
}
//-------------------------------------------------------

float ofApp::lfo(float amp, float rate,int shape){
    
    return amp*sin(rate);
}

 
 /****************************************************/



//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key=='3'){
        aa=ss=dd=ff=gg=hh=0;
    }
    
    if(key=='='){
        alpha+=1;
    }
    if(key=='-'){
        alpha-=1;
    }
    
    if(key=='2'){
          //loop.play();
    }
    
 
    if(key=='1'){
        for(int i=0;i<fbob;i++){
            pastFrames[i].begin();
        
            ofClear(0,0,0,255);
        
            pastFrames[i].end();
        }
        
        fbo_draw.begin();
        ofClear(0,0,0,255);
        fbo_draw.end();
    }
    //if(key=='q'){sw1==0;}
    
    if(key=='a'){aa+=0.0001;}
    if(key=='z'){aa-=0.0001;}
    if(key=='s'){ss+=0.0001;}
    if(key=='x'){ss-=0.0001;}
    
    if(key=='d'){dd+=0.0001;}
    if(key=='c'){dd-=0.0001;}
    
    if(key=='f'){ff+=0.0001;}
    if(key=='v'){ff-=0.0001;}
    
    if(key=='g'){gg+=0.0001;}
    if(key=='b'){gg-=0.0001;}
    if(key=='h'){hh+=0.01;}
    if(key=='n'){hh-=0.01;}
    
    
    if(key=='j'){jj+=0.1;}
    if(key=='m'){jj-=0.1;}
    if(key=='k'){kk+=0.1;}
    if(key==','){kk-=0.1;}
    
    if(key=='l'){cam1_scale+=0.01;}
    if(key=='.'){cam1_scale-=0.01;}
    
    if(key==';'){scale1+=0.01;}
    if(key=='/'){scale1-=0.01;}
    
    if(key=='['){scale2+=0.01;}
    if(key==']'){scale2-=0.01;}
    
    if(key=='q'){qq+=0.001;cout << "qq"<<qq<< endl;}
    if(key=='w'){qq-=0.001;cout << "qq"<<qq<< endl;}
    if(key=='e'){ee+=.1;cout << "ee"<<ee<< endl;}
    if(key=='r'){ee-=.1;cout << "ee"<<ee<< endl;}
    
    
    if(key=='u'){ii+=1;}
    if(key=='i'){ii-=1;}
    if(key=='o'){oo+=.1;}
    if(key=='p'){oo-=.1;}
    
    if(key=='t'){tt+=.01;}
    if(key=='y'){tt-=.01;}
    
    
    if(key=='2'){amp+=.001;}
    if(key=='3'){amp-=.001;}
    


}

/************************/
//--------------------------------------------------------------
void ofApp::exit() {
	
	// clean up
	midiIn.closePort();
	midiIn.removeListener(this);
}

//_---------------------------
void ofApp::midibiz(){
    /*midimessagesbiz**/
    
    
    //ofTranslate(0,0,-100);
    for(unsigned int i = 0; i < midiMessages.size(); ++i) {
        
        ofxMidiMessage &message = midiMessages[i];
        
        
        ofSetColor(127);
        if(message.status < MIDI_SYSEX) {
            //text << "chan: " << message.channel;
            if(message.status == MIDI_CONTROL_CHANGE) {
                
                //How to Midi Map
                //uncomment the line that says cout<<message control etc
                //run the code and look down in the console
                //when u move a knob on yr controller keep track of the number that shows up
                //that is the cc value of the knob
                //then go down to the part labled 'MIDIMAPZONE'
                //and change the numbers for each if message.control== statement to the values
                //on yr controller
                
                 cout << "message.control"<< message.control<< endl;
                 cout << "message.value"<< message.value<< endl;
                //lets try defaulting to bipolar...
                //nice try but nope that sucks
                //midi_controls[message.control]=(message.value-63.0)/63.0;
                
                
                //ch1 hue
                if(message.control==20){c1=(message.value-63.0f)/63.0f;}
                
                //ch1 sat
                if(message.control==21){c2=(message.value-63.0f)/63.0f;}
                
                //ch1 bright
                if(message.control==22){c3=(message.value-63.0f)/63.0f;}
                
                //global blur
                if(message.control==25){c4=(message.value)/127.0f;}
                
                //global sharpen
                if(message.control==24){c5=(message.value)/127.0f;}
                
                //fb0 key value
                if(message.control==28){c6=(message.value)/127.0f;}
                
                //fb0 mix
                if(message.control==29){c7=(message.value-63.0f)/63.0f;}
                
                //fb0 delay amount
                if(message.control==30){c8=(message.value)/127.0f;}
                
                //fb0 x
                if(message.control==4){c9=(message.value-63.0f)/63.0f;}
                
                //fb0 y
                if(message.control==3){c10=(message.value-63.0f)/63.0f;}

                
                //fb0 z
                if(message.control==12){c11=(message.value-63.0f)/63.0f;}

                
                //fb0 theta
                if(message.control==11){c12=(message.value-63.0f)/63.0f;}
                
                //fb0 hue
                if(message.control==5){c13=(message.value-63.0f)/63.0f;}
                
                //fb0 sat
                if(message.control==2){c14=(message.value-63.0f)/63.0f;}
                
                //fb0 bright
                if(message.control==13){c15=(message.value-63.0f)/63.0f;}

                
                //fb0 huex_mod
                if(message.control==16){c16=(message.value)/127.0f;}
                
                //fb0 huex_offset
                if(message.control==10){c17=(message.value-63.0f)/63.0f;}
                
                //fb0 huex_lfo
                if(message.control==17){c18=(message.value-63.0f)/63.0f;}
                
                
                //fB!1
                //fb1 key value
                if(message.control==31){c19=(message.value)/127.0f;}
                
                //fb1 mix
                if(message.control==27){c20=(message.value-63.0f)/63.0f;}
                
                //fb1 delay amount
                if(message.control==26){c21=(message.value)/127.0f;}
                
                //fb1 x
                if(message.control==6){c22=(message.value-63.0f)/63.0f;}
                
                //fb1 y
                if(message.control==1){c23=(message.value-63.0f)/63.0f;}
                
                
                //fb1 z
                if(message.control==14){c24=(message.value-63.0f)/63.0f;}
                
                
                //fb1 theta
                if(message.control==9){c25=(message.value-63.0f)/63.0f;}
                
                //fb1 hue
                if(message.control==7){c26=(message.value-63.0f)/63.0f;}
                
                //fb1 sat
                if(message.control==0){c27=(message.value-63.0f)/63.0f;}
                
                //fb1 bright
                if(message.control==15){c28=(message.value-63.0f)/63.0f;}
                
                
                //fb1 huex_mod
                if(message.control==18){c29=(message.value)/127.0f;}
                
                //fb1 huex_offset
                if(message.control==8){c30=(message.value-63.0f)/63.0f;}
                
                //fb1 huex_lfo
                if(message.control==19){c31=(message.value-63.0f)/63.0f;}

                
                //MIDIMAPZONE
                //these are all set to output bipolor controls at this moment (ranging from -1.0 to 1.0)
                //if u uncomment the second line on each of these if statements that will switch thems to unipolor
                //controls (ranging from 0.0to 1.0) if  you prefer
                //then find the variable that youd like to control down in CAVARIABLEZONES or MIXERVARIBLEZONES
                //and substitute c1,c2, ..cn whichever control knob u wish the map
          
                
            }
           
            
            
            
        }//
        
        
    }
    
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {

	// add the latest message to the message queue
	midiMessages.push_back(msg);

	// remove any old messages if we have too many
	while(midiMessages.size() > maxMessages) {
		midiMessages.erase(midiMessages.begin());
	}
}

//--------------------------------------------------------------


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
   
}


//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
   
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
  
}

//--------------------------------------------------------------
void ofApp::mouseReleased() {
}
