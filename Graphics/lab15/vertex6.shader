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

out vec3 l;
out vec3 v;
out vec3 n;
out vec3 h;


void main()
{
	vec4 vertex = transform.model * vec4(position, 1.0f);
    l = vec3(normalize(light.position - vertex ));
    v = normalize(transform.viewPosition - vec3(vertex));
    n = normalize(transform.normal * normal);
    h = normalize(l+v); 
    gl_Position = transform.viewProjection * vertex;
}