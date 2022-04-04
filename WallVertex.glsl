attribute vec2 aPosition;
uniform mat4 mvpMatrix;

void main()
{
   gl_Position = mvpMatrix * vec4(aPosition,0.0,1.0);
}

