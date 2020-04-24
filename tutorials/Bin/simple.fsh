uniform sampler2D g_Texture;

varying vec2 Texcoord;

void main()
{
	vec4 res = texture2D(g_Texture, vec2(Texcoord.x, 1.0 - Texcoord.y));
	gl_FragColor = res / res.w;
}