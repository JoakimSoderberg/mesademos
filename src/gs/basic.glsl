#version 120
#extension GL_ARB_geometry_shader4 : enable

const int KernelSize = 9;

void main()
{
  for(int i = 0; i < gl_VerticesIn; ++i)
  {
    gl_FrontColor = gl_FrontColorIn[i];
    gl_BackColor = gl_BackColorIn[i];
    gl_Position = gl_PositionIn[i];
    EmitVertex();
  }
  EndPrimitive();
}
