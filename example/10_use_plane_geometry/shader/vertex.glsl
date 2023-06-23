#version 330 core
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;

out vec3 outPosition;
out vec2 outTexCoord;

uniform float factor;

/**
  * |  1   0      0     0|    |x|    |         x         |
  * |  0   cos0  -sin0  0|  * |y| =  |cos0 * y - sin0 * z|
  * |  0   sin0  cos0   0|    |z|    |sin0 * y + cos0 * z|
  * |  0   0      0     1|    |w|    |         1         |
  */
mat4 rotateX(float angle) {
  return mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cos(angle), -sin(angle), 0.0f, 0.0f, sin(angle), cos(angle), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

/**
  * |  cos0  0  sin0   0|    |x|    |x * cos0 + z * sin0|
  * |  0     0     1   0|  * |y| =  |         y         |
  * | -sin0  0  cos0   0|    |z|    |-sinθ *x + cosθ * z|
  * |  0     0     0   1|    |w|    |         1         |
  */
mat4 rotateY(float angle) {
  return mat4(cos(angle), 0.0f, sin(angle), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -sin(angle), 0.0f, cos(angle), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

/**
  * |  cos0  -sin0   0   0|    |x|    |x * cos0 - y * sin0|
  * |  sin0   cos0   0   0|  * |y| =  |x * sin0 + y * cos0|
  * |  0       0     1   0|    |z|    |         z         |
  * |  0       0     0   1|    |w|    |         1         |
  */
mat4 rotateZ(float angle) {
  return mat4(cos(angle), -sin(angle), 0.0f, 0.0f, sin(angle), cos(angle), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

mat4 rotateXYZ(float angle) {
  return rotateZ(angle) * rotateY(angle) * rotateX(angle);
}

void main() {
  // gl_Position = rotateXYZ(factor) * vec4(Position, 1.0f);
  gl_Position = vec4(Position, 1.0f);
  gl_PointSize = 10.0f;
  outTexCoord = TexCoords;
}