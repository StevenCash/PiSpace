#version 100
varying vec4 vColorV;
void main()
{
    gl_FragColor = vColorV;
//   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
