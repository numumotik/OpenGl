out vec4 color;

uniform struct ProjLight {
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 attenuation;
    vec3 spotDirection;
    float spotCosCutoff;
    float spotExponent;
} light;

uniform	sampler2D myTexture;

uniform struct Material {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emission;
	float shiness;
} material;

in Vertex{
vec2 texcoord;
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

    //расчёт угла отклонения от направления прожектора до текущей точки
    floatspotEffect = dot(normalize(light.spotDirection),−lightDir);
    //ограничим зону влияние прожектора
    floatspot = float(spotEffect>light.spotCosCutoff);
    //экспоненциальное затухание к краям зоны влияния
    spotEffect = max(pow(spotEffect,light.spotExponent), 0.0);
    //коэффициент затухания прожектора
    float attenuation = spot*spotEffect/(light.attenuation[0]+light.attenuation[1]*Vert.distance+light.attenuation[2]*Vert.distance*Vert.distance);

	color = material.emission;
	color += material.ambient*light.ambient*attenuation;
	float Ndot = max(dot(normal, lightDir), 0.0);
	color += material.diffuse*light.diffuse*Ndot*attenuation;
	float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), material.shiness), 0.0);
	color += material.specular*light.specular*RdotVpow*attenuation;

	color *= texture(myTexture, Vert.texcoord);
    
    
}