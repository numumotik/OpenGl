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
   
   vec3 BrickColor = vec3(0.84, 0.61, 0.55); 
   vec3 BackColor = vec3(0.55,0.33,0.23); 
   vec3 BrickSize = vec3(4.0, 2.5, 2.5); 
   vec3 BrickPct = vec3(0.9, 0.9, 0.9); 
 
   vec3 Lpos = Vert.vertPos / BrickSize;
   Lpos.y += 0.5;
   if (fract (Lpos.y * .5)<0.5)
      Lpos.x +=0.5;
   if (fract (Lpos.y * .5)<0.5)
      Lpos.z += 0.5;
   Lpos = fract (Lpos);
 
    vec3 uses = step (Lpos, BrickPct);
    counted =vec4( mix (BackColor, BrickColor, uses.x*uses.y*uses.z ) ,1.0);
    
   color*=counted;
}