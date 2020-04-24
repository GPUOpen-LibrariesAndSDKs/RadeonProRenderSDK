attribute vec3 inPosition;
attribute vec2 inTexcoord;

varying vec2 Texcoord;

void main()
{
	Texcoord = inTexcoord;
	gl_Position = vec4(inPosition, 1.0);
}
