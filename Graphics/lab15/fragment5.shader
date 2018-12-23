out vec4 color;
in vec4 objColor;

in vec3 l;
in vec3 v;
in vec3 n;
in vec3 r;

uniform vec4 warm;
uniform vec4 cold;
uniform float diffwarm;
uniform float diffcold;

void main()
{
	vec3 n2 = normalize(n);
	vec3 l2 = normalize(l);
    vec3 v2 = normalize(v);
    vec3 r2 = normalize(r);
    
    vec3 o_Color = vec3(objColor);
    vec3 k_cold = min(vec3(cold) + diffcold * o_Color, 1.0);
	vec3 k_warm = min(vec3(warm) + diffwarm * o_Color, 1.0);
	vec3 k_fin  = mix(k_cold, k_warm, dot(n2, l2));
    
	float spec  = pow(max(dot(r2, v2), 0.0), 32.0);
	color = vec4(min(k_fin + spec, 1.0), 1.0);
}
