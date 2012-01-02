#version 110
#extension GL_ARB_texture_rectangle : enable

uniform sampler2D map;

uniform sampler2DRect frame;
uniform vec2 frameSize;

uniform bool flipv;

void main()
{
	vec4 uv = texture2D( map, gl_TexCoord[0].xy );
	if ( flipv ) 
		uv.y = 1.0-uv.y;

    gl_FragColor.rgb = texture2DRect( frame, uv.xy * frameSize ).rgb;	
    gl_FragColor.a = uv.a;	
}