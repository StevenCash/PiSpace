#version 100
attribute vec3 vPosition;
attribute vec3 vColor;
uniform mat4 mvpMatrix;
varying vec3 vColorV;
void main()
{
   gl_Position = mvpMatrix * vec4(vPosition,1.0);
   vColorV = vColor;
}

