out vec4 color;

in vec3 l;
in vec3 v;
in vec3 n;
in vec3 h;

uniform vec4 diff;
uniform vec4 spec;
uniform float specPow;
uniform float rimPow;
uniform float bias;

void main()
{
	vec3 n2 = normalize(n);
	vec3 l2 = normalize(l);
    vec3 v2 = normalize(v);
    vec3 h2 = normalize(h);
    
    vec4 diffuse = diff * max(dot(n2,l2),0.0);
    vec4 specular = spec * pow(max(dot(n2,h2),0.0), specPow);
    float rim = pow(1.0+bias - max(dot(n2,v2),0.0),rimPow);
    
    color = diffuse + rim*vec4(0.5,0.0,0.2,1.0)+specular*spec;
}
