#version 410

out vec4 outColor;
uniform vec4 color;

void main() {
  float i = 1.0 - gl_FragCoord.z;
  outColor = color - i;
}