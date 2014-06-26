#version 100
attribute vec4 vPosition;
uniform mat4 mvpMatrix;
void main()
{
   gl_Position = mvpMatrix * vPosition;
}

