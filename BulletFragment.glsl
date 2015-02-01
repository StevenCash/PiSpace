#version 100
varying vec3 vColorV;
void main()
{
    gl_FragColor = vec4(vColorV,1.0);
//  gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);
}
