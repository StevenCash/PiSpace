attribute vec3 aPosition;
attribute vec3 aColor;
uniform mat4 mvpMatrix;
uniform float uTime;
varying vec3 vColor;
uniform vec2 uOffset;
void main()
{
  vec4 position = vec4(aPosition,1.0);
  //scale the position to fit the 10x10 world
  position = mvpMatrix * position;
  mat4 scale;
  scale[0][0]=1.0+(uTime*9.0);
  scale[1][1]=1.0+(uTime*9.0);
  scale[2][2]=1.0;
  scale[3][3]=1.0;
  
  position = scale * position;
  vec4 scaledOffset = mvpMatrix * vec4(uOffset,0.0,0.0);
  gl_Position = position + scaledOffset;
  vColor = aColor;
}

