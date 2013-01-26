uniform sampler2D mapTex;
uniform sampler2D displayTex;

void main()
{
    gl_FragColor = texture2D(mapTex, gl_TexCoord[0].xy);
}
