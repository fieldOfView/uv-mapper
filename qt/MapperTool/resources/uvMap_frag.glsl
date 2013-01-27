uniform sampler2D mapTex;
uniform sampler2D displayTex;

void main()
{
    vec4 uv = texture2D(mapTex, gl_TexCoord[0].xy);

    gl_FragColor.rgb = texture2D(displayTex, uv.xy).rgb;
    gl_FragColor.a = uv.a;
}
