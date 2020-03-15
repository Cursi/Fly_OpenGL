#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float ElapsedTime;

uniform int isSeaObject;

// Output values to fragment shader
out vec3 world_position;
out vec3 world_normal;

void main()
{
	// TODO: compute world space vertex position and normal
	// TODO: send world position and world normal to Fragment Shader

	world_position = (Model * vec4(v_position,1)).xyz;
	world_normal = normalize(mat3(Model) * v_normal);

	if(isSeaObject == 1)
	{
		vec4 vPoz = vec4(v_position, 1);
		vPoz = Model * vPoz;

		float wavlen = 8;
		float amp = 0.8;
		float speed = 4;
		float k = 2 * 3.14 / wavlen;
	
		vPoz.y += amp * sin(k * (vPoz.x + speed * ElapsedTime));
	
		gl_Position = Projection * View * vPoz;
	}
	else
	{
		gl_Position = Projection * View * Model * vec4(v_position, 1.0);
	}
}