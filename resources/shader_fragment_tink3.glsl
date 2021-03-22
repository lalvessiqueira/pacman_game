//#version 330 core
//out vec3 color;
//in vec3 vertex_pos;
//void main()
//{
////color = vec3(1,1,1);
//color = vec3(0, 0.6, 0.6);
//}

#version 330 core
out vec4 color;
in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;
uniform vec3 campos;

uniform sampler2D tex;
void main()
{
   vec2 tc = vertex_tex / 2;
   vec4 tcol = texture(tex, tc);
   float a = (tcol.x + tcol.y + tcol.z) / 2;
   color = tcol;
   color.a = a;
}
