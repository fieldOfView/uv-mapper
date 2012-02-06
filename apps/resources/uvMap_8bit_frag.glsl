#version 110
#extension GL_ARB_texture_rectangle : enable

uniform sampler2D map_MSB;
uniform sampler2D map_LSB;

uniform sampler2DRect frame;
uniform vec2 frameSize;

uniform bool flipv;

void main()
{
	vec4 uv = texture2D( map_MSB, gl_TexCoord[0].xy ) + texture2D( map_LSB, gl_TexCoord[0].xy ) / 256.;
	if ( flipv ) 
		uv.y = 1.0-uv.y;

    gl_FragColor = texture2DRect( frame, uv.xy * frameSize );	
    gl_FragColor.a *= uv.a;	
}