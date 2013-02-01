uniform vec3 colorBG;
uniform vec3 colorFG;
uniform float cells;
uniform float width;

void main(void)
{
    vec3 colorX = mix(colorFG, colorBG, step(fract(gl_TexCoord[0].x*cells - width/2.), 1.-width));
    vec3 colorY = mix(colorFG, colorX, step(fract(gl_TexCoord[0].y*cells - width/2.), 1.-width));
    gl_FragColor.rgb = mix(colorFG, colorY, step(fract(2.*sqrt(pow(gl_TexCoord[0].x-.5,2.)+pow(gl_TexCoord[0].y-.5,2.))), 1.-(2.*width/cells)));

    gl_FragColor.a = 1.0;
}
