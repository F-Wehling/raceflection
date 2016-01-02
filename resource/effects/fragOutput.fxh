GLSLShader 
{
    //
    // Prototypes
    //
    void finalColor(vec3 normal, vec4 color, vec3 tc, float s, vec4 generic_1, vec4 generic_2);
	void finalDeferredColor(vec4 colorTexValue, vec4 normalTexValue, float depthTexValue, vec4 materialParameterTexValue);
}
////////////////////////////////////////////////////////////////////
//
// Different ways of calculating the final color
//
GLSLShader global GreenColor
{
    layout(location=0) out vec4 color;
    // takes a generic set of arguments that could be used or not
    void finalColor(vec3 normal, vec4 colorSrc, vec3 tc, float s, vec4 generic_1, vec4 generic_2)
    {
        color = vec4(0.2,0.9,0.2, 1);
    }
}
GLSLShader global NormalsAsColor
{
    layout(location=0) out vec4 color;
    // takes a generic set of arguments that could be used or not
    void finalColor(vec3 normal, vec4 colorSrc, vec3 tc, float s, vec4 generic_1, vec4 generic_2)
    {
        color = vec4(normal*0.5+0.5, 1);
    }
}
GLSLShader global TcAsColor
{
    layout(location=0) out vec4 color;
    // takes a generic set of arguments that could be used or not
    void finalColor(vec3 normal, vec4 colorSrc, vec3 tc, float s, vec4 generic_1, vec4 generic_2)
    {
        color = vec4(tc, 1);
    }
}
GLSLShader global ColorAsColor
{
    layout(location=0) out vec4 color;
    // takes a generic set of arguments that could be used or not
    void finalColor(vec3 Normal, vec4 colorSrc, vec3 tc, float s, vec4 generic_1, vec4 generic_2)
    {
        color = colorSrc;
    }
}
GLSLShader global ScalarAsColor
{
    layout(location=0) out vec4 color;
    // takes a generic set of arguments that could be used or not
    void finalColor(vec3 Normal, vec4 colorSrc, vec3 tc, float s, vec4 generic_1, vec4 generic_2)
    {
        color = vec4(s,s,s,1.0);
    }
}
GLSLShader global ColorAndNormals
{
    layout(location=0) out vec4 outColor;
    layout(location=1) out vec4 outNormal;
    // takes a generic set of arguments that could be used or not
    void finalColor(vec3 normal, vec4 colorSrc, vec3 tc, float s, vec4 generic_1, vec4 generic_2)
    {
        outColor = colorSrc;
        outNormal = vec4(normal*0.5+0.5, 1);
    }
}
GLSLShader global DeferredGeometry {
	layout(location=0) out vec4 outColor;
	layout(location=1) out vec4 outNormal;
	layout(location=2) out float outDepth;
	layout(location=3) out vec4 outMaterialParameter;
	
	void finalColor(vec3 normal, vec4 colorSrc, vec3 tc, float s, vec4 generic_1, vec4 generic_2){
		outColor = colorSrc;
		outNormal = vec4(normal.xy, generic_1.xy);
		outDepth = s;
		outMaterialParameter = generic_2;
	}
}

GLSLShader global DeferredLighting {
	layout(location=0) out vec4 outColor;
	
	void finalDeferredColor(vec4 colorTexValue, vec4 normalTexValue, float depthTexValue, vec4 materialParameterTexValue){
		outColor = colorTexValue;
	
		if(i_NumLights > 2){
			outColor = vec4(1.0, 0.0, 1.0, 1.0);
		}
	
		for(int i = 0; i < i_NumLights; ++i){
			vec3 l_pos = light_position(i);
			vec3 l_dir = light_direction(i);
			vec3 l_diffuse = light_diffuse(i);
			vec3 l_specular = light_specular(i);
			vec3 l_ambient = light_ambient(i);
			vec3 l_attenuation = light_attenuation(i);
			vec2 l_angleCone = light_angleCone(i);
		}
	}
	
}