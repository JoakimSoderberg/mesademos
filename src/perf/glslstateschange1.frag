// Multi-texture fragment shader
// Brian Paul

// Composite second texture over first.
// We're assuming the 2nd texture has a meaningful alpha channel.

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform vec4 UniV1;
uniform vec4 UniV2;

void main()
{
   vec4 t3;
   vec4 t1 = texture2D(tex1, gl_TexCoord[0].xy);
   vec4 t2 = texture2D(tex2, gl_TexCoord[1].xy);
   t3 = mix(t1, t2, t2.w);
   gl_FragColor = t3 + UniV1 + UniV2;
}
