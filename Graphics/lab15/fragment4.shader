out vec4 color;
in vec4 objColor;

in vec3 l;
in vec3 n;

void main()
{
	vec3 n2 = normalize(n);
	vec3 l2 = normalize(l);

	float diff = 0.2 + max( dot( n2, l2), 0.0);
    vec4 clr;
    
    if (diff < 0.4)
        clr = objColor * 0.2;
    else
    if (diff < 0.7)
        clr = objColor  * 0.5;
    else 
        clr = objColor;
    
    color = clr;
}