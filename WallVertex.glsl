#version 100
attribute vec2 vPosition;
uniform mat4 mvpMatrix;
void main()
{
   gl_Position = mvpMatrix * vec4(vPosition,0.0,1.0);
}

