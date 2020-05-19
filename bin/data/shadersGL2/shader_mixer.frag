#version 120
#ifdef GL_ES
    precision highp float;
#endif

varying vec2 texCoordVarying;

uniform sampler2DRect syphon;
uniform sampler2DRect cam1;
uniform sampler2DRect cam2;

uniform sampler2DRect fb0;
uniform sampler2DRect fb1;
uniform sampler2DRect fb2;
uniform sampler2DRect fb3;

uniform vec2 center;

//fb0
uniform vec3 fb0_hsb_x;
uniform vec3 fb0_hue_x;
uniform vec3 fb0_rescale;
uniform vec3 fb0_modswitch;
uniform float fb0_rotate;

//fb1
uniform vec3 fb1_hsb_x;
uniform vec3 fb1_hue_x;
uniform vec3 fb1_rescale;
uniform vec3 fb1_modswitch;
uniform float fb1_rotate;

//fb2
uniform vec3 fb2_hsb_x;
uniform vec3 fb2_hue_x;
uniform vec3 fb2_rescale;
uniform vec3 fb2_modswitch;
uniform float fb2_rotate;

//fb3
uniform vec3 fb3_hsb_x;
uniform vec3 fb3_hue_x;
uniform vec3 fb3_rescale;
uniform vec3 fb3_modswitch;
uniform float fb3_rotate;



uniform float delaymix;


uniform float ch1_h_mirror;

//vidmixervariables

uniform float cam1_scale;
uniform float cam2_scale;
uniform float width;
uniform float height;



//variables from gui
uniform int channel1;
uniform int channel2;

uniform int mix1;
uniform int mix2;


//mix1 variables
uniform float mix1blend1;


uniform float mix1keybright;
uniform float mix1keythresh;


//fbmixvariables
uniform float fb0lumakeyvalue;
uniform float fb0lumakeythresh;
uniform int fb0mix;
uniform float fb0blend;

uniform float fb1lumakeyvalue;
uniform float fb1lumakeythresh;
uniform int fb1mix;
uniform float fb1blend;


uniform float fb2lumakeyvalue;
uniform float fb2lumakeythresh;
uniform int fb2mix;
uniform float fb2blend;

uniform float fb3lumakeyvalue;
uniform float fb3lumakeythresh;
uniform int fb3mix;
uniform float fb3blend;

//channel1 variablesfrom gui
uniform float channel1hue_x;
uniform float channel1saturation_x;
uniform float channel1bright_x;

uniform float channel1hue_powmap;
uniform float channel1sat_powmap;
uniform float channel1bright_powmap;


uniform int channel1satwrap;
uniform int channel1brightwrap;

uniform int ch1hue_powmaptoggle;
uniform int ch1sat_powmaptoggle;
uniform int ch1bright_powmaptoggle;

uniform int ch1hue_inverttoggle;
uniform int ch1sat_inverttoggle;
uniform int ch1bright_inverttoggle;


//channel2 variablesfrom gui
uniform float channel2hue_x;
uniform float channel2saturation_x;
uniform float channel2bright_x;

uniform float channel2hue_powmap;
uniform float channel2sat_powmap;
uniform float channel2bright_powmap;


uniform int channel2satwrap;
uniform int channel2brightwrap;

uniform int ch2hue_powmaptoggle;
uniform int ch2sat_powmaptoggle;
uniform int ch2bright_powmaptoggle;

uniform int ch2hue_inverttoggle;
uniform int ch2sat_inverttoggle;
uniform int ch2bright_inverttoggle;


uniform int cam1_hflip_switch;
uniform int cam1_vflip_switch;


uniform int cam2_hflip_switch;
uniform int cam2_vflip_switch;


uniform int fb0_hflip_switch;
uniform int fb0_vflip_switch;

uniform int fb1_hflip_switch;
uniform int fb1_vflip_switch;

uniform int fb2_hflip_switch;
uniform int fb2_vflip_switch;

uniform int fb3_hflip_switch;
uniform int fb3_vflip_switch;


uniform int fb0_pixel_switch;
uniform int fb0_pixel_scale;
uniform float fb0_pixel_mix;
uniform float fb0_pixel_brightscale;

uniform int fb1_pixel_switch;
uniform int fb1_pixel_scale;
uniform float fb1_pixel_mix;
uniform float fb1_pixel_brightscale;

uniform int fb2_pixel_switch;
uniform int fb2_pixel_scale;
uniform float fb2_pixel_mix;
uniform float fb2_pixel_brightscale;

uniform int fb3_pixel_switch;
uniform int fb3_pixel_scale;
uniform float fb3_pixel_mix;
uniform float fb3_pixel_brightscale;

uniform int cam1_pixel_switch;
uniform int cam1_pixel_scale;
uniform float cam1_pixel_mix;
uniform float cam1_pixel_brightscale;

uniform int cam2_pixel_switch;
uniform int cam2_pixel_scale;
uniform float cam2_pixel_mix;
uniform float cam2_pixel_brightscale;

uniform int fb0_toroid_switch;
uniform int fb1_toroid_switch;
uniform int fb2_toroid_switch;
uniform int fb3_toroid_switch;

uniform float ps;

uniform vec2 cam1dimensions;
uniform vec2 cam2dimensions;

//uniform float pp=1.0;

//just some generice testing varibles
//uniform float qq;
//uniform float ee;

mat2 getRotMat(float theta)
{
    float s = sin(theta); float c = cos(theta);
    return mat2(c,-s,s,c);
}

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
    
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

mat3 rgb2yiq = mat3(  0.299,    0.596,		0.212,
						0.587,   	-0.275,  	-0.523,
 						0.114,  	-0.321,  	0.311		);

mat3 yiq2rgb = mat3(	1.0,     1.0 ,    1.0, 
						0.956,   -0.272,  -1.107,
						0.621,  -0.647,  1.704		);

vec3 fb_operations(vec3 color, vec3 hsbx, vec3 huex, vec3 modswitch, float bright_fold_switch, inout float luma)
{
    vec3 yiq = rgb2yiq * color ; /*
                                    yiq, luma + chroma type colourspace
                                    similar to ycbcr used in ntsc.
                                    solves some luma issues with hsb,
                                    and probably gets you authenticity points.
                                    (messing up the chroma gives gnarly results!)
                                    see: https://stackoverflow.com/questions/9234724/how-to-change-hue-of-a-texture-with-glsl 
                                    ps. make sure to always do matrix mults in correct order: mat * vec !
                                */                                    
    if (yiq.y == 0.) yiq.y += 1.0e-10; // avoid zero div on hue calc
    float chroma = length(yiq.yz);
    float hue = atan(yiq.z/yiq.y) / 6.28 + .5;  // scale from -pi - pi to 0 - 1
    hue = mod( abs( hue*hsbx.r + huex.z*sin(hue/3.14) ),
                                    huex.x) + huex.y;
    if (modswitch.r==1.0) hue = 1.-hue;
    hue = (hue - .5) * 6.28;
    
    chroma *= hsbx.g; // scale saturation by adjusting chroma
    if(modswitch.g==1.0) chroma=-chroma;    // flip saturation
    if(modswitch.b==1.0) yiq.x=1.0-yiq.x;   // flip brightness
    yiq = vec3(yiq.x, chroma * cos(hue), chroma * sin(hue));  // resynthesise yiq with new hue        
    yiq *= hsbx.b;  // scale brightness by adjusting all channels
    luma = yiq.x;
    vec3 c_rgb = yiq2rgb * yiq;
    c_rgb = clamp(c_rgb, vec3(0.), vec3(1.)); // brightness clamp, maybe not necessary?
    return c_rgb; 
                    
}                    

vec3 channel_operations(vec3 color,float hue_x,float sat_x, float bright_x
                           ,int hue_powmaptoggle, int sat_powmaptoggle, int bright_powmaptoggle
                           ,float hue_powmap, float sat_powmap, float bright_powmap
                           ,int satwrap,int brightwrap,int hueinvert,int satinvert,int brightinvert,
                           float luma)
{
    vec3 yiq = rgb2yiq * color;
    if (yiq.y == 0.) yiq.y += 1.0e-10; // avoid zero div on hue calc
    float chroma = length(yiq.yz);
    float hue = atan(yiq.z/yiq.y) / 6.28 + .5; // scale from -pi - pi to 0 - 1
        
    hue = hue * hue_x;
    if (hue_powmaptoggle==1)
        hue = pow(hue,hue_powmap);

    if(hue<0.){
        if(hueinvert==0) hue=1.0-abs(hue);
        else if(hueinvert==1) hue=abs(1.0-abs(hue));
    }
    hue = fract(hue);
    hue = (hue - .5) * 6.28;

    chroma = chroma * sat_x;
    if(sat_powmaptoggle==1)
        chroma=pow(chroma,sat_powmap);

    if(chroma<0.){
        if(satinvert==0) chroma=1.0-abs(chroma);
        else if(satinvert==1) chroma=abs(1.0-abs(chroma));        
    }
    
    if(satwrap==1.0){
        if(abs(chroma)>1.0){
            chroma=fract(chroma);
        }    
    }else if(satwrap==0.0){
        if(chroma>1.0){
            chroma=1;
        }
    }
    
            
    if(bright_powmaptoggle==1)
        yiq.x=pow(yiq.x,bright_powmap);
    
    if(brightwrap==1.0){
        if(abs(yiq.x)>1.0){
            yiq.x=fract(yiq.x);
        }
    }else if(brightwrap==0.0){
        if(yiq.x>1.0){
            yiq.x=1;
        }
    }
        
    if(yiq.x<0.){ // i don't believe many of these conditions can be hit;
                  // a pow() function will always return positive, for example
        if(brightinvert==0) yiq.x=1.0-abs(yiq.x);
        if(brightinvert==1) yiq.x=abs(1.0-abs(yiq.x));
    }
    luma = yiq.x;
    
    return yiq2rgb * yiq;
    
}


//lets instead rewrite this for rgb modes bc i dont think blend works without rgb
//can just send the brightness value from the earlier hsb version along for the ride to calculate the lumakey thingss

vec4 mix_rgb(vec4 ch1, vec4 ch2, int mixswitch,float blend, float lumavalue, float lumathresh, float bright1, float bright2){
    vec4 mixout=vec4(0.0,0.0,0.0,0.0);
    
    
    //pass ch1 it thru
    
    if(mixswitch==0){
        mixout=ch1;
        
    }//endpass
    
    
    //blendit
    if(mixswitch==1){
        mixout=mix(ch1,ch2,blend);
    }//endblend
    
    //keyit
    if(mixswitch==2){
        
        /*
         if((bright1<lumavalue+lumathresh)&&(bright1>lumavalue-lumathresh)){
         mixout=mix(ch1,ch2,blend);
         
         }//endifbright1
         else{
         mixout=ch2;
         }
         */
        
        if(bright1*bright1<=lumavalue){
            mixout=ch2;
        }
        else{
            mixout=mix(ch1,ch2,blend);
        }
        
    }//keyit
    
    //mixout=vec3(mixhue,mixsat,mixbright);
    return mixout;
}//endmixfunction




//pixelatefunction


vec4 pixelate(float scale, vec2 coord, sampler2DRect  pixelTex, float pixelMixxx, vec4 c, float brightscale){
    vec4 pixel_color=texture2DRect(pixelTex,coord);
    vec2 pixelScaleCoord= coord;
    
    //add a switch and control for how much brightness changes stuff up
    scale=floor(scale*((1-brightscale)+(brightscale)*(.33*pixel_color.r+.5*pixel_color.g+.16*pixel_color.b)));
    
    pixelScaleCoord.x=coord.x/width;
    pixelScaleCoord.y=coord.y/height;
    
    pixelScaleCoord.x=floor(scale*pixelScaleCoord.x)/scale;
    pixelScaleCoord.y=floor(scale*pixelScaleCoord.y)/scale;
    
    pixelScaleCoord.x=width*pixelScaleCoord.x;
    pixelScaleCoord.y=height*pixelScaleCoord.y;
    
    pixel_color=texture2DRect(pixelTex,pixelScaleCoord+.5);
    
    return mix(c,pixel_color,pixelMixxx);
    
}//endpixelatefunction


vec2 rotate(vec2 coord,float theta){

    vec2 center_coord=coord-center;
    vec2 rotate_coord=vec2(0.);
/*
    // unused?
    float spiral=abs(coord.x+coord.y)/2*width;
    coord.x=spiral+coord.x;
    coord.y=spiral+coord.y;
*/
    mat2 rot = getRotMat(theta);
    rotate_coord *= rot;
    
    rotate_coord=rotate_coord+center;
    
    return rotate_coord;
       
}//endrotate


vec2 wrapCoord(vec2 coord){
        
    if(abs(coord.x)>width){coord.x=abs(width-coord.x);}
    if(abs(coord.y)>height){coord.y=abs(height-coord.y);}
    
    coord.x=mod(coord.x,width);
    coord.y=mod(coord.y,height);
    
    return coord;
}

vec2 doTransforms(vec2 co, vec3 t, float a, int toroid, int hflip, int vflip){
    co = co - center; // offset from 0-n to -n/2 - n/2 range
    co = t.z * co;  // scale
    co = t.xy + co; // translate
    co *= getRotMat(a); // rotate
    co += center; // restore to 0-n range
        
    if(toroid==1)
        co=wrapCoord(co);
    
    // i might have these the wrong way round, try removing the negation from the abs if so
    if(hflip==1)
        co.x = -abs(co.x-center.x);
        //if(fb1_coord.x>width/2){fb1_coord.x=abs(width-fb1_coord.x);}
    
    if(vflip==1)
        co.y = -abs(co.y-center.y);
        //if(fb1_coord.y>height/2){fb1_coord.y=abs(height-fb1_coord.y);}

    
    return co;
}

void main()
{
    //set up dummy variables for each channel
    vec4 channel1_color=vec4(0.0, 0.0, 0.0, 0.0);
    
    vec4 channel2_color=vec4(0.0, 0.0, 0.0, 0.0);
    
    /*
        i think the ideal situation here would be for these 4 channels to have their settings packed into mat4 types.
        
        if we pack the mat4 with settings like :
        fb0:vec4(rescale:[x,y,z],rotate_theta)
        fb1: etc 
        
        then the 4 sets of 4 values for the scale/translate/rotate transform become one variable.
       
        another mat4 can be used the same way to indicate vec4(toroidal, hflip, vflip , (empty column)) for each channel as well.
        
        mat4 breaks down as vec4[4] array, so current swizzles will still work with index provided, working like
        coord = coord * transforms[fbIndex].z;
        for example.
        following this, we can reuse a function that simply takes the index of the channel it is calculating a transform for,
        instead of many input variables.
        
        all of this will result in less uniforms/code on both shader + host, and more reusable functions!
        
        for now I have left the uniforms structure intact.
     */
    vec2 fb0_coord=doTransforms(texCoordVarying, fb0_rescale, fb0_rotate, fb0_toroid_switch, fb0_hflip_switch, fb0_vflip_switch);    
    vec4 fb0_color = texture2DRect(fb0,fb0_coord);

    // you can accomplish the same thing here by setting your texture extend condition to GL_ZERO on the host side
    // so this call can be totally removed with that addition, if you like.
    // it's also worth experimenting with GL_REPEAT and GL_MIRROR, in my opinion,
    // but it doesn't conform to how real world CRT feedback works.
    // for more information see: http://openframeworks.jp/documentation/gl/ofTexture.html#show_setTextureWrap
    if(abs(fb0_coord.x-width/2)>=width/2||abs(fb0_coord.y-height/2)>=height/2)
        fb0_color=vec4(0.);
    
    
    
    // similar data packing could occur for the pixelate filter below.
    
    
    ///testing the pixelation function
    //0 mix value is pure pixel
    //1 mix value is bypass
    //smaller values for pixel size make larger pixels
    //the way to calculate the actual pixel size is width/index
    if(fb0_pixel_switch==1)    
        fb0_color=pixelate(fb0_pixel_scale,fb0_coord,fb0,fb0_pixel_mix,fb0_color,fb0_pixel_brightscale);
    
    
    
    //original flavor
    
    vec2 fb1_coord=doTransforms(texCoordVarying, fb1_rescale, fb1_rotate, fb1_toroid_switch, fb1_hflip_switch, fb1_vflip_switch);
    vec4 fb1_color=texture2DRect(fb1,fb1_coord);
    
    if(abs(fb1_coord.x-center.x)>=center.x||abs(fb1_coord.y-center.y)>=center.y)
        fb1_color=vec4(0.);
    
    if(fb1_pixel_switch==1)
        fb1_color=pixelate(fb1_pixel_scale,fb1_coord,fb1,fb1_pixel_mix,fb1_color,fb1_pixel_brightscale);
    
    
    
    vec2 fb2_coord=doTransforms(texCoordVarying, fb2_rescale, fb2_rotate, fb2_toroid_switch, fb2_hflip_switch, fb2_vflip_switch);
    vec4 fb2_color=texture2DRect(fb2,fb2_coord);
    
    if(abs(fb2_coord.x-width/2)>=width/2||abs(fb2_coord.y-height/2)>=height/2)
        fb2_color=vec4(0.);
    
    
    if(fb2_pixel_switch==1)
        fb2_color=pixelate(fb2_pixel_scale,fb2_coord,fb2,fb2_pixel_mix,fb2_color,fb2_pixel_brightscale);
   
    
    vec2 fb3_coord=doTransforms(texCoordVarying, fb3_rescale, fb3_rotate, fb3_toroid_switch, fb3_hflip_switch, fb3_vflip_switch);
    vec4 fb3_color = texture2DRect(fb3,fb3_coord);
    
    if(abs(fb2_coord.x-width/2)>=width/2||abs(fb2_coord.y-height/2)>=height/2)
        fb2_color=vec4(0.);
    
    if(fb3_pixel_switch==1)
        fb3_color=pixelate(fb3_pixel_scale,fb3_coord,fb3,fb3_pixel_mix,fb3_color,fb3_pixel_brightscale);
    
    
    vec4 syphon_color=texture2DRect(syphon,texCoordVarying);
    
    
    //just like  spare dummy variabl for a color vector
    vec4 color = vec4(0.0);
    
    
    vec2 cam1_coord=texCoordVarying*cam1_scale;
    
    if(cam1_hflip_switch==1)
        if(texCoordVarying.x>width/2){cam1_coord.x=cam1_scale*abs(width-texCoordVarying.x);}    
    if(cam1_vflip_switch==1)
        if(texCoordVarying.y>height/2){cam1_coord.y=cam1_scale*abs(height-texCoordVarying.y);}
    vec4 cam1color=vec4(0.0);
    
    if(texCoordVarying.x*cam1_scale<cam1dimensions.x &&
        texCoordVarying.y*cam1_scale<cam1dimensions.y)
            cam1color=texture2DRect(cam1,vec2(cam1_coord.x,cam1_coord.y));

    if(cam1_pixel_switch==1)        
        cam1color=pixelate(cam1_pixel_scale,cam1_coord,cam1,cam1_pixel_mix,cam1color,cam1_pixel_brightscale);
    
    
    vec2 cam2_coord=texCoordVarying*cam2_scale;
    
    if(cam2_hflip_switch==1)
        if(texCoordVarying.x>width/2){cam2_coord.x=cam2_scale*abs(width-texCoordVarying.x);}    
    if(cam2_vflip_switch==1)
        if(texCoordVarying.y>height/2){cam2_coord.y=cam2_scale*abs(height-texCoordVarying.y);}
    vec4 cam2color=vec4(0.0);
    
    if(texCoordVarying.x*cam2_scale<cam2dimensions.x &&
        texCoordVarying.y*cam2_scale<cam2dimensions.y)
            cam2color=texture2DRect(cam2,vec2(cam2_coord.x,cam2_coord.y));

    if(cam2_pixel_switch==1)        
        cam2color=pixelate(cam2_pixel_scale,cam2_coord,cam2,cam2_pixel_mix,cam2color,cam2_pixel_brightscale);
    
    
    
    
    //select which input for channel / channel2
    
    if(channel1==1)
        channel1_color=cam1color;
    else if(channel1==2)
        channel1_color=cam2color;
    else if(channel1==3)
        channel1_color=syphon_color;
    //endifch1
    
    if(channel2==1)
        channel2_color=cam1color;
    else if(channel2==2)
        channel2_color=cam2color;
    else if(channel2==3)
        channel2_color=syphon_color;
    //endifch2
    
    float ch1luma, ch2luma;
    // since we need the luma as well, and can't return two objects,
    // we use inout keyword and pass luma variables to be filled

    channel1_color.rgb=channel_operations(channel1_color.rgb, channel1hue_x, channel1saturation_x, channel1bright_x
                                            ,ch1hue_powmaptoggle,ch1sat_powmaptoggle,ch1bright_powmaptoggle
                                            ,channel1hue_powmap,channel1sat_powmap,channel1bright_powmap
                                            ,channel1satwrap,channel1brightwrap,
                                            ch1hue_inverttoggle,ch1sat_inverttoggle,ch1bright_inverttoggle,
                                            ch1luma);
    
    channel2_color.rgb=channel_operations(channel2_color.rgb, channel2hue_x, channel2saturation_x, channel2bright_x
                                            ,ch2hue_powmaptoggle,ch2sat_powmaptoggle,ch2bright_powmaptoggle
                                            ,channel2hue_powmap,channel2sat_powmap,channel2bright_powmap
                                            ,channel2satwrap,channel2brightwrap,
                                            ch2hue_inverttoggle,ch2sat_inverttoggle,ch2bright_inverttoggle,
                                            ch2luma);
    
    
    float fb0luma, fb1luma, fb2luma, fb3luma;
    fb0_color.rgb = fb_operations(fb0_color.rgb,fb0_hsb_x,fb0_hue_x,fb0_modswitch,0.,fb0luma);
    fb1_color.rgb = fb_operations(fb1_color.rgb,fb1_hsb_x,fb1_hue_x,fb1_modswitch,0.,fb1luma);
    fb2_color.rgb = fb_operations(fb2_color.rgb,fb2_hsb_x,fb2_hue_x,fb2_modswitch,0.,fb2luma);
    fb3_color.rgb = fb_operations(fb3_color.rgb,fb3_hsb_x,fb3_hue_x,fb3_modswitch,0.,fb3luma);
    
        
    //switch on and off alpha in here... and test a lot more

    
    //next we do the mixxxing
    
    vec4 mixout_color=mix_rgb(channel1_color,channel2_color,mix1,mix1blend1,mix1keybright,mix1keythresh,ch1luma,ch2luma);
    vec3 lumacoef = vec3(0.299, 0.587, 0.114);
    float mixout_luma = dot(mixout_color.rgb*mixout_color.a, lumacoef);
    
    mixout_color=mix_rgb(mixout_color,fb0_color,fb0mix,fb0blend,fb0lumakeyvalue,fb0lumakeythresh, mixout_luma,fb0luma);
    
    mixout_luma = dot(mixout_color.rgb*mixout_color.a, lumacoef);
    
    mixout_color=mix_rgb(mixout_color,fb1_color,fb1mix,fb1blend,fb1lumakeyvalue,fb1lumakeythresh, mixout_luma,fb1luma);
    
    mixout_luma = dot(mixout_color.rgb*mixout_color.a, lumacoef);
    
    
    mixout_color=mix_rgb(mixout_color,fb2_color,fb2mix,fb2blend,fb2lumakeyvalue,fb2lumakeythresh, mixout_luma,fb2luma);
    
    mixout_luma = dot(mixout_color.rgb*mixout_color.a, lumacoef);
        
    mixout_color=mix_rgb(mixout_color,fb3_color,fb3mix,fb3blend,fb3lumakeyvalue,fb3lumakeythresh, mixout_luma,fb3luma);
    
    gl_FragColor = mixout_color;
    
    
}
