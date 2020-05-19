#version 120


uniform sampler2DRect tex0;

varying vec2 texCoordVarying;

uniform float sharpAmnt;

uniform float sharpen_boost;

uniform float sharpenStep;

uniform float chi;


vec4 sharpen(vec2 co)
{
	vec4 centre = texture2DRect(tex0, co);
	vec2 o = vec2(sharpenStep, 0.);

	vec4 k =    texture2DRect(tex0, co-o.yx) +
                texture2DRect(tex0, co+o.yx) +
                texture2DRect(tex0, co+o.xy) +
                texture2DRect(tex0, co-o.xy);

	vec4 d = 	texture2DRect(tex0, co-o.xx)      +
                texture2DRect(tex0, co-o.yx+o.xy) +
                texture2DRect(tex0, co+o.xy-o.yx) +
                texture2DRect(tex0, co+o.xx);

						
	vec4 s = k+d;
	
	return centre+sharpAmnt*(chi*centre+(-chi*8.) * s)/9.;
	
}


void main()
{
    // flatten the sharpen process into a single function
    vec4 color_sharpen=sharpen(texCoordVarying);

    // this does the necessary multiplies to get luma as a single float,
    // hue and sat calcs are expensive and we don't use them
    float brightness = dot(color_sharpen.rgb, vec3(.299, .587, .114));
    // we could do max(r(max(g,b)) as well, should work out similar
    
    
    color_sharpen *= (sharpen_boost*1.)*brightness;
     
     gl_FragColor =color_sharpen;
}
