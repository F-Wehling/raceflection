//
///Material

uniform Sampler2D diffuseSampler;
uniform Sampler2D specularSampler;
uniform Sampler2D ambientSampler;
uniform Sampler2D emissiveSampler;
uniform Sampler2D heightSampler;
uniform Sampler2D normalSampler;
uniform Sampler2D shininessSampler;
uniform Sampler2D opacitySampler;
uniform Sampler2D displacementSampler;
uniform Sampler2D lightSampler;
uniform Sampler2D reflectionSampler;	
uniform int materialID;

ConstantBuffer global Materials {
	uniform int i_NumMaterials;
	uniform vec4 _materialParameter[100 * 8];
}

GLSLShader {
	#define MAX_MATERIALS 100
	#define MATERIAL_ENTRIES 8
	struct Material {
		vec4 v4_diffuseColor;
		vec4 v4_specularColor;
		vec4 v4_ambientColor;
		vec4 v4_emissiveColor;
		vec4 v4_transparentColor;
		vec4 v4_reflectiveColor;
		float f_opacity;
		float f_shininess;
		float f_shininessStrength;
		float f_refracti;
		float f_bumbScaling;
	};
	
	uniform vec3 transparentColor = vec3(1.0, 0.0, 1.0);
	
	void getMaterial(int materialIdx, out Material material);
}

GLSLShader global MaterialImpl {
	void getMaterial(int i, out Material material) {
		material.v4_diffuseColor = _materialParameter[i * MATERIAL_ENTRIES + 0];
		material.v4_specularColor = _materialParameter[i * MATERIAL_ENTRIES + 1];
		material.v4_ambientColor = _materialParameter[i * MATERIAL_ENTRIES + 2];
		material.v4_emissiveColor = _materialParameter[i * MATERIAL_ENTRIES + 3];
		material.v4_transparentColor = _materialParameter[i * MATERIAL_ENTRIES + 4];
		material.v4_reflectiveColor = _materialParameter[i * MATERIAL_ENTRIES + 5];
		material.f_opacity = _materialParameter[i * MATERIAL_ENTRIES + 6].x;
		material.f_shininess = _materialParameter[i * MATERIAL_ENTRIES + 6].y;
		material.f_shininessStrength = _materialParameter[i * MATERIAL_ENTRIES + 6].z;
		material.f_refracti = _materialParameter[i * MATERIAL_ENTRIES + 6].w;
		material.f_bumbScaling = _materialParameter[i * MATERIAL_ENTRIES + 7].x;
	}
	
}