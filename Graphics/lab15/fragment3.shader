out vec4 color;
in vec4 objColor;

in vec3 l;
in vec3 v;
in vec3 n;

uniform float k;

void main()
{
	vec3 n2 = normalize(n);
	vec3 l2 = normalize(l);
    vec3 v2 = normalize(v);
    
    float d1 = pow(max(dot(n2, l2), 0.0), 1.0 + k);
    float d2 = pow(1.0 - dot(n2, v2), 1.0 - k);
    color = objColor * d1 * d2;
}
