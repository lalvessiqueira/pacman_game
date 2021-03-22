#version 330 core
out vec4 color;
in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;
uniform vec3 campos;

uniform vec2 texoff;
uniform sampler2D tex;
void main()
{
   vec2 tc = vertex_tex / 4. + texoff * 0.5;
   vec4 tcol = texture(tex, tc);
   float a = (tcol.x + tcol.y + tcol.z) / 3;
   color = tcol;
   color.a = a;
}
