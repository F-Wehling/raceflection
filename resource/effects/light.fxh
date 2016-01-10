//
///Light		
ConstantBuffer global Lights {
	uniform int i_NumLights;
	uniform vec4 v_lightData[100 * 6]; //the number N (f_lightData[N*M]) has to match the define MAX_LIGHTS
										 //the number M (f_lightData[N*M]) has to match the define LIGHT_ENTIRES
}

GLSLShader {
	#define MAX_LIGHTS 100
	#define LIGHT_ENTIRES 6
	
	#define LIGHT_POINT 1
	#define LIGHT_DIRECTIONAL 2
	#define LIGHT_SPOT 4
	#define LIGHT_AMBIENT 8
}


//
/// handle the light definition
GLSLShader {

	struct Light{
		uint type;
		vec3 position;
		vec3 direction;
		vec3 diffuse;
		vec3 specular;
		vec3 ambient;
		float constantAttenuation;
		float linearAttenuation;
		float quadraticAttenuation;
		float innerConeAngle;
		float outerConeAngle;
	};
/*
	uint light_type(int i);	
	vec3 light_position(int i);	
	vec3 light_direction(int i);	
	vec3 light_diffuse(int i);	
	vec3 light_specular(int i);	
	vec3 light_ambient(int i);	
	vec3 light_attenuation(int i);	
	float light_attenuationConstant(int i);	
	float light_attenuationLinear(int i);	
	float light_attenuationQuadratic(int i);	
	vec2 light_angleCone(int i);	
	float light_angleInnerCone(int i);	
	float light_angleOuterCone(int i);
*/
	void getLight(int lightIndex, out Light light);
}

GLSLShader global LightCalculation{
	uint light_type(int i){
		return uint(floatBitsToInt(v_lightData[i * LIGHT_ENTIRES + 0].x));
	}
	
	vec3 light_position(int i){
		return v_lightData[i * LIGHT_ENTIRES + 0].yzw;
	}
	
	vec3 light_direction(int i){	
		return v_lightData[i * LIGHT_ENTIRES + 1].xyz;
	}
	
	vec3 light_diffuse(int i){	
		return vec3(v_lightData[i * LIGHT_ENTIRES + 1].w, v_lightData[i * LIGHT_ENTIRES + 2].xy);
	}
	
	vec3 light_specular(int i){	
		return vec3(v_lightData[i * LIGHT_ENTIRES + 2].zw,	v_lightData[i * LIGHT_ENTIRES + 3].x);
	}
	
	vec3 light_ambient(int i){	
		return v_lightData[i * LIGHT_ENTIRES + 3].yzw;
	}
	
	vec3 light_attenuation(int i){
		return v_lightData[i * LIGHT_ENTIRES + 4].xyz;
	}
	
	float light_attenuationConstant(int i){
		return v_lightData[i * LIGHT_ENTIRES + 4].x;
	}
	
	float light_attenuationLinear(int i){
		return v_lightData[i * LIGHT_ENTIRES + 4].y;
	}
	
	float light_attenuationQuadratic(int i){
		return v_lightData[i * LIGHT_ENTIRES + 4].z;
	}
	
	vec2 light_angleCone(int i){
		return vec2(v_lightData[i * LIGHT_ENTIRES + 4].w, v_lightData[i * LIGHT_ENTIRES + 5].x);
	}
	
	float light_angleInnerCone(int i){
		return v_lightData[i * LIGHT_ENTIRES + 4].w;
	}
	
	float light_angleOuterCone(int i){
		return v_lightData[i * LIGHT_ENTIRES + 5].x;
	}
		
	
	void getLight(int i, out Light light) {
		light.type = light_type(i);
		light.position = (m4_View * vec4(light_position(i), 1.0)).xyz;
		light.direction = mat3(m4_View) * light_direction(i);
		light.diffuse = light_diffuse(i);
		light.specular = light_specular(i);
		light.ambient = light_ambient(i);
		light.constantAttenuation = light_attenuationConstant(i);
		light.linearAttenuation = light_attenuationLinear(i);
		light.quadraticAttenuation = light_attenuationQuadratic(i);
		light.innerConeAngle = light_angleInnerCone(i);
		light.outerConeAngle = light_angleOuterCone(i);
	}
}
