#version 110
attribute vec3 aPosition;
attribute vec3 aColor;
uniform mat4 mvpMatrix;
varying vec3 vColor;

void main()
{
   gl_Position = mvpMatrix * vec4(aPosition,1.0);
   vColor = aColor;
}

