#extension GL_ARB_uniform_buffer_object : enable

layout(std140) uniform colors0
{
    float DiffuseCool;
    float DiffuseWarm;
    vec3  SurfaceColor;
    vec3  WarmColor;
    vec3  CoolColor;
    vec4  some[8];
};

varying float NdotL;
varying vec3  ReflectVec;
varying vec3  ViewVec;

void main (void)
{

    vec3 kcool    = min(CoolColor + DiffuseCool * SurfaceColor, 1.0);
    vec3 kwarm    = min(WarmColor + DiffuseWarm * SurfaceColor, 1.0);
    vec3 kfinal   = mix(kcool, kwarm, NdotL);

    vec3 nreflect = normalize(ReflectVec);
    vec3 nview    = normalize(ViewVec);

    float spec    = max(dot(nreflect, nview), 0.0);
    spec          = pow(spec, 32.0);

    gl_FragColor = vec4 (min(kfinal + spec, 1.0), 1.0);
}
