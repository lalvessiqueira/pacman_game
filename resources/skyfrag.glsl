#version 330 core
out vec4 color;
in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;
uniform vec3 campos;

uniform sampler2D tex;
uniform sampler2D tex2;

uniform float dn;
void main()
{
   vec3 n = normalize(vertex_normal);
   vec3 lp = vec3(0,-30,90);
   vec3 ld = normalize(lp - vertex_pos);
   float d = dot(n, ld);
   d = clamp(d, 0, 1);
   
   vec4 tcol = texture(tex,vec2(1. - vertex_tex.x,vertex_tex.y) *5.);
   vec4 tcoln = texture(tex2, vertex_tex);
   color = dn * tcoln + (1. - dn) * tcol;
   
   //specular light
   vec3 cd = normalize(campos - vertex_pos);
   vec3 h = normalize(cd + ld);
   float s = dot(h,n);
   s = clamp(s, 0, 1);
   s = s * pow(d,50);
   
   color.rgb = color.rgb * d;
   color.rgb += vec3(1,1,1) * s;
   
      float gamma = 2.0;
      color.rgb = pow(color.rgb, vec3(1.0/gamma));
   
//vec4 tcol = texture(tex, vertex_tex *5.);
//vec4 tcoln = texture(tex2, vertex_tex);
//color = dn * tcoln + (1.-dn) * tcol;
}
