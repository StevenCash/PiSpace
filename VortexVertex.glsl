#version 110
attribute vec2 aPosition;
uniform mat4 mvpMatrix;
uniform float uFade;
varying float vFade;
void main()
{
   gl_Position = mvpMatrix * vec4(aPosition,0.0,1.0);
   vFade=uFade;
}

