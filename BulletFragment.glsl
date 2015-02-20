#version 110
varying vec3 vColorV;
varying float vFade;
void main()
{
   vec4 color = vec4(vColorV,1.0);
   color.x*=vFade;
   color.y*=vFade;
   color.z*=vFade;
   gl_FragColor = color;
}
