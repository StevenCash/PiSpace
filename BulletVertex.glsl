#version 110
attribute vec3 vPosition;
attribute vec3 vColor;
uniform mat4 mvpMatrix;
uniform float uFade;
varying vec3 vColorV;
varying float vFade;

void main()
{
   gl_Position = mvpMatrix * vec4(vPosition,1.0);
   vColorV = vColor;
   vFade=uFade;
}

