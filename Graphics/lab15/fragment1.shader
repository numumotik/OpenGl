out vec4 color;
in vec4 objColor;

in vec3 l;
in vec3 n;

void main()
{
	vec3 n2 = normalize(n);
	vec3 l2 = normalize(l);

	vec4 diff = objColor;
    diff *= max( dot(n2, l2), 0.0);
    color = diff;
}