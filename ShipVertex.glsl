#version 100
attribute vec4 vPosition;
attribute vec4 vColor;
uniform mat4 mvpMatrix;
varying vec4 vColorV;

void main()
{
   gl_Position = mvpMatrix * vPosition;
   vColorV=vColor;

}

