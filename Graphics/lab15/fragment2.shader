out vec4 color;
in vec4 objColor;

in vec3 l;
in vec3 h;
in vec3 n;

uniform struct Material {
	vec4 diffuse;
	vec4 specular;
	float shiness;
} material;

void main()
{
	vec3 n2 = normalize(n);
	vec3 l2 = normalize(l);
    vec3 h2 = normalize(h);
    vec3 r = reflect(-h2,n2);
	vec4 diff = material.diffuse * max( dot(n2, l2), 0.0);
    vec4 spec = material.specular * pow(max(dot(l2,r),0.0),material.shiness);
    color = diff + spec;
    color *= objColor;
}
