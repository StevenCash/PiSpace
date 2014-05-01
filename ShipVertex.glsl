#version 100
attribute vec4 vPosition;
attribute vec4 vColor;
uniform mat4 rotation;
varying vec4 vColorV;

void main()
{
   gl_Position = rotation * vPosition;
   vColorV=vColor;

}

