uniform vec3 colorBG;
uniform vec3 colorFG;
uniform float cells;
uniform float width;

void main(void)
{
    vec3 colorX = mix(colorFG, colorBG, step(fract(gl_TexCoord[0].x*cells - width/2.), 1.-width));
    gl_FragColor.rgb = mix(colorFG, colorX, step(fract(gl_TexCoord[0].y*cells - width/2.), 1.-width));

    gl_FragColor.a = 1.0;
}
