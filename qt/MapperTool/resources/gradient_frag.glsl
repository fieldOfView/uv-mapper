uniform vec3 colorTL;
uniform vec3 colorTR;
uniform vec3 colorBL;
uniform vec3 colorBR;

void main(void)
{
    vec3 colorL = mix(colorTL, colorBL, gl_TexCoord[0].y);
    vec3 colorR = mix(colorTR, colorBR, gl_TexCoord[0].y);
    gl_FragColor.rgb = mix(colorL, colorR, gl_TexCoord[0].x);

    gl_FragColor.a = 1.0;
}
