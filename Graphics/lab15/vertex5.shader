layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

uniform struct Transform {
mat4 model;
mat4 viewProjection;
mat3 normal;
vec3 viewPosition;
} transform;

uniform struct PointLight {
vec4 position;
} light;

uniform vec4 diffColor;
uniform vec4 Warm;
uniform vec4 Cold;
uniform float diffWarm;
uniform float diffCold;

out vec4 objColor;
out vec3 l;
out vec3 v;
out vec3 n;
out vec3 r;


void main()
{
	vec4 vertex = transform.model * vec4(position, 1.0f);
    l = vec3(normalize(light.position - vertex ));
    v = normalize(transform.viewPosition - vec3(vertex));
    n = normalize(transform.normal * normal);
    r = - l - n*2*dot(n,-l); 
    gl_Position = transform.viewProjection * vertex;
    objColor = diffColor;
}