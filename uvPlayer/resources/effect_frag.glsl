#version 110
#extension GL_ARB_texture_rectangle : enable

uniform sampler2D map;

uniform sampler2DRect frame;
uniform vec2 frameSize;

uniform bool flipv;

void main()
{
	vec2 uv = texture2D( map, gl_TexCoord[0].xy ).rg;
	if (uv.x > 0.0 && uv.y > 0.0) {
		if ( flipv ) 
			uv.y = 1.0-uv.y;
		
		gl_FragColor.rgb = texture2DRect( frame, uv.xy * frameSize ).rgb;	
	}
	gl_FragColor.a = 1.0;	
}