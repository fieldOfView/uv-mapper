uniform vec2 windowSize;
uniform float gridSize;

uniform vec3 colorA;
uniform vec3 colorB;

void main(void)
{
    vec2 windowPos = gl_TexCoord[0].xy * windowSize;
    float halfGrid = gridSize/2.;

    if ((mod(windowPos.x, gridSize) > halfGrid && mod(windowPos.y, gridSize) > halfGrid)
      ||(mod(windowPos.x, gridSize) < halfGrid && mod(windowPos.y, gridSize) < halfGrid))
      gl_FragColor.rgb = colorA;
    else
      gl_FragColor.rgb = colorB;

    gl_FragColor.a = 1.0;
}
