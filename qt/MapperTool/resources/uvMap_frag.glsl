uniform sampler2D mapTex;
uniform sampler2D displayTex;
uniform bool transparency;

void main()
{
    vec4 uv = texture2D(mapTex, gl_TexCoord[0].xy);

    gl_FragColor.rgb = texture2D(displayTex, uv.xy).rgb;
    gl_FragColor.a = (transparency)?uv.a:1.0;
}
