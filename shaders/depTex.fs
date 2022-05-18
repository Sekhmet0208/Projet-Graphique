#version 330
uniform sampler2D tex;
in  vec2 vsoTexCoord;
in  vec3 vsoNormal;
out vec4 fragColor;

void main(void) {
  vec3 N = normalize(vsoNormal);
  vec3 L = normalize(vec3(-0.5, -0.5, -2.5)); 
  float diffuse = dot(N, -L);
  fragColor = vec4((texture(tex, vsoTexCoord).rgb * diffuse), 1);
}
