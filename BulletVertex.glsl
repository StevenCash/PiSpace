attribute vec3 vPosition;
attribute vec3 aColor;
uniform mat4 mvpMatrix;
uniform float uFade;
varying vec3 vColor;
varying float vFade;

void main()
{
   gl_Position = mvpMatrix * vec4(vPosition,1.0);
   vColor = aColor;
   vFade=uFade;
}

