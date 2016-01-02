//
///Light		
ConstantBuffer global Lights {
	uniform int i_NumLights;	
	uniform float f_lightData[100 * 21]; //the number N (f_lightData[N*M]) has to match the define MAX_LIGHTS
										 //the number M (f_lightData[N*M]) has to match the define LIGHT_SIZE
}

GLSLShader {
	#define MAX_LIGHTS 100
	#define LIGHT_SIZE 21
}


//
/// handle the light definition
GLSLShader {
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
}

GLSLShader global LightCalculation{
	uint light_type(int i){
		return floatBitsToInt(f_lightData[i * LIGHT_SIZE + 0]);
	}
	
	vec3 light_position(int i){
		return vec3(
			f_lightData[i * LIGHT_SIZE + 1],
			f_lightData[i * LIGHT_SIZE + 2],
			f_lightData[i * LIGHT_SIZE + 3]
		);
	}
	
	vec3 light_direction(int i){	
		return vec3(
			f_lightData[i * LIGHT_SIZE + 4],
			f_lightData[i * LIGHT_SIZE + 5],
			f_lightData[i * LIGHT_SIZE + 6]
		);
	}
	
	vec3 light_diffuse(int i){	
		return vec3(
			f_lightData[i * LIGHT_SIZE + 7],
			f_lightData[i * LIGHT_SIZE + 8],
			f_lightData[i * LIGHT_SIZE + 9]
		);
	}
	
	vec3 light_specular(int i){	
		return vec3(
			f_lightData[i * LIGHT_SIZE + 10],
			f_lightData[i * LIGHT_SIZE + 11],
			f_lightData[i * LIGHT_SIZE + 12]
		);
	}
	
	vec3 light_ambient(int i){	
		return vec3(
			f_lightData[i * LIGHT_SIZE + 13],
			f_lightData[i * LIGHT_SIZE + 14],
			f_lightData[i * LIGHT_SIZE + 15]
		);
	}
	
	vec3 light_attenuation(int i){
		return vec3(
			f_lightData[i * LIGHT_SIZE + 16],
			f_lightData[i * LIGHT_SIZE + 17],
			f_lightData[i * LIGHT_SIZE + 18]
		);	
	}
	
	float light_attenuationConstant(int i){
		return f_lightData[i * LIGHT_SIZE + 16];
	}
	
	float light_attenuationLinear(int i){
		return f_lightData[i * LIGHT_SIZE + 17];
	}
	
	float light_attenuationQuadratic(int i){
		return f_lightData[i * LIGHT_SIZE + 18];
	}
	
	vec2 light_angleCone(int i){
		return vec2(
			f_lightData[i * LIGHT_SIZE + 19],
			f_lightData[i * LIGHT_SIZE + 20]
		);
	}
	
	float light_angleInnerCone(int i){
		return f_lightData[i * LIGHT_SIZE + 19];
	}
	
	float light_angleOuterCone(int i){
		return f_lightData[i * LIGHT_SIZE + 20];
	}
}