#version 100
attribute vec4 vPosition;
uniform mat4 mvpMatrix;
uniform vec4 color;
varying vec4 Color;
void main()
{
   gl_Position = mvpMatrix * vPosition;
   Color = color;
}

