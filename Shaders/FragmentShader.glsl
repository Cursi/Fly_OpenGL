#version 330

// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform int spotlightWanted; 

uniform vec3 object_color;

layout(location = 0) out vec4 out_color;

void main()
{
	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);

	vec3 worldNormal = normalize(world_normal);

	float intensitateLumina = 0.9;
	float f_at = 1;
	// TODO: define ambient light component
	float ambient_light = 0.45;
	
	// TODO: compute diffuse light component
	float diffuse_light = material_kd * intensitateLumina * max(dot(worldNormal, L), 0);

	// TODO: compute specular light component
	float specular_light = 0;

	if (diffuse_light > 0)
	{
		specular_light = material_ks * intensitateLumina * pow(max(dot(worldNormal, H), 0), material_shininess);
	}

	float cut_off = radians(30);
	float spot_light = dot(-L, light_direction);
	float spot_light_limit = cos(cut_off);
 
	// Quadratic attenuation
	float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
	float light_att_factor = pow(linear_att, 2);

	f_at = 1 / (pow(distance(light_position, world_position), 2) + 1); 

	float computedLight;

	// TODO: compute light
	if(spotlightWanted == 1)
	{
		if (spot_light > spot_light_limit)
		{
			computedLight = ambient_light + (diffuse_light + specular_light) * light_att_factor;
		}
		else
		{	
			computedLight = ambient_light;
		}
	}
	else
	{
		computedLight = ambient_light + (diffuse_light + specular_light) * f_at;
	}

	// TODO: send color light output to fragment shader
	vec3 color = object_color * computedLight;

	// TODO: write pixel out color
	out_color = vec4(color, 1);
}