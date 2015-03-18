varying vec3 vColor;
varying float vFade;
void main()
{
   vec4 color = vec4(vColor,1.0);
   color.x*=vFade;
   color.y*=vFade;
   color.z*=vFade;
   gl_FragColor = color;
}
