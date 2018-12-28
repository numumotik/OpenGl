out vec4 color;
uniform struct PointLight {
int type;
vec4 position;
vec4 ambient;
vec4 diffuse;
vec4 specular;
vec3 attenuation;
vec3 spotDirection;
float spotCutoff;
float spotExponent;
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

vec4 countColor()
{
    vec4 counted = vec4(1.0, 1.0, 1.0, 1.0);
    
    float num = 10;
    float wid = 0.2;
    vec4 c_back = vec4(0.57, 0.43, 0.68, 1.0);
    vec4 c_edge = vec4(0.46, 0.36, 0.6, 1.0);
    vec4 c_beam = vec4(0.7, 0.48, 0.8, 1.0);

    float y = mod(Vert.texcoord.t, 1.0 / (num - 1.0)) * (num - 1.0);
    float dy = (y < 0.5) ? y : 1.0 - y;
    counted = (dy > wid) ? c_back : mix(c_beam, c_edge, dy / wid);

    float x = mod(Vert.texcoord.s, 1.0 / (num - 1.0)) * (num - 1.0);
    float dx = (x < 0.5) ? x : 1.0 - x;
    if (dx <= wid) 
        counted = mix(c_beam, c_edge, dx / wid);
    
    return counted;
}

vec4 countBathroom()
{
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
    
    return counted;
}

vec4 countBricks()
{
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
    
    return counted;
}

vec4 colorPoint()
{
    vec4 counted = vec4(1.0,1.0,1.0,1.0);
    
    vec3 normal = normalize(Vert.normal);
	vec3 lightDir = normalize(Vert.lightDir);
	vec3 viewDir = normalize(Vert.viewDir);

    float attenuation = 1.0 / (light.attenuation[0] + light.attenuation[1] * Vert.distance + light.attenuation[2] * Vert.distance*Vert.distance);
	counted = material.emission;
	counted += material.ambient*light.ambient*attenuation;
	float Ndot = max(dot(normal, lightDir), 0.0);
	counted += material.diffuse*light.diffuse*Ndot*attenuation;
	float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), material.shiness), 0.0);
	counted += material.specular*light.specular*RdotVpow*attenuation;
    
    return counted;
}

vec4 colorDir()
{
    vec4 counted = vec4(1.0,1.0,1.0,1.0);
    vec3 normal = normalize(Vert.normal);
	vec3 lightDir = normalize(vec3(light.position));
	vec3 viewDir = normalize(Vert.viewDir);

	counted = material.emission;
	counted += material.ambient*light.ambient;
	float Ndot = max(dot(normal, lightDir), 0.0);
	counted += material.diffuse*light.diffuse*Ndot;
	float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), material.shiness), 0.0);
	counted += material.specular*light.specular*RdotVpow;
    
    return counted;
}

vec4 colorSpot()
{
    vec4 counted = vec4(1.0,1.0,1.0,1.0);
     
    vec3 normal = normalize(Vert.normal);
	vec3 lightDir = normalize(Vert.lightDir);
	vec3 viewDir = normalize(Vert.viewDir);

    float spotEffect = dot(normalize(light.spotDirection), -lightDir);
    float spot = spotEffect > cos(light.spotCutoff)? 1.0 : 0.0;
    spotEffect = max(pow(spotEffect, light.spotExponent),0.0);
    float attenuation = spot * spotEffect / (light.attenuation[0] + light.attenuation[1] * Vert.distance + light.attenuation[2] * Vert.distance*Vert.distance);
	counted = material.emission;
	counted += material.ambient*light.ambient*attenuation;
	float Ndot = max(dot(normal, lightDir), 0.0);
	counted += material.diffuse*light.diffuse*Ndot*attenuation;
	float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), material.shiness), 0.0);
	counted += material.specular*light.specular*RdotVpow*attenuation;
    
    return counted;
}

void main()
{
	if (light.type == 0)
        color = colorPoint();
    else 
        if (light.type == 1)
            color = colorSpot();
        else
            color = colorDir();
    
    color*=countColor();
}