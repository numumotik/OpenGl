out vec4 color;
uniform struct PointLight {
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 attenuation;
} light;

uniform struct Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emission;
	float shiness;
} material;

in Vertex{
vec2 texcoord;
vec3 vertPos;
vec3 normal;
vec3 lightDir;
vec3 viewDir;
float distance;
} Vert;

void main()
{
	vec3 normal = normalize(Vert.normal);
	vec3 lightDir = normalize(Vert.lightDir);
	vec3 viewDir = normalize(Vert.viewDir);

	float attenuation = 1.0 / (light.attenuation[0] + light.attenuation[1] * Vert.distance + light.attenuation[2] * Vert.distance*Vert.distance);
	color = material.emission;
	color += material.ambient*light.ambient*attenuation;
	float Ndot = max(dot(normal, lightDir), 0.0);
	color += material.diffuse*light.diffuse*Ndot*attenuation;
	float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), material.shiness), 0.0);
	color += material.specular*light.specular*RdotVpow*attenuation;

    vec4 counted = vec4(1.0, 1.0, 1.0, 1.0);
    float num = 7.5;
    float gap = 0.03;
    vec4 c_insd = vec4(0.57, 0.43, 0.68, 1.0);
    vec4 c_back = vec4(0.75, 0.73, 0.73, 1.0);

    float reg = 0.5 / num;
    float rx = Vert.texcoord.x / reg;
    float x = fract(rx); 
    float ry = Vert.texcoord.y / reg;
    float y = fract(ry);
    
    if (x < gap || x > 1.0 - gap || y < gap || y > 1.0 - gap)
        counted = c_back;
    else
    {
    if (mod(floor(rx) + floor(ry), 2.0) == 0.0)
        counted = (y - x > 0.5 || x - y > 0.5) ? c_back : c_insd;
    else
      counted = (y + x < 0.5 || x + y > 1.5) ? c_back : c_insd;
    }
   
   color*=counted;
}