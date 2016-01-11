//
/// Render Targets

// will lead to a creation of a texture in a specific repository. nvFX will keep track of the Id
//store normals
RenderTexture rt_Normal
{
    MSAA = {0,0};
    Size = ApplicationDefined;
    Format = RG16F;
}

//Store diffuse Color
RenderTexture rt_Color
{
    MSAA = {0,0};
    Size = ApplicationDefined;// a way to dynamically define the size of the resource to create
    Format = RGBA8;
}

RenderTexture rt_Depth
{
	MSAA = {0,0};
	Size = ApplicationDefined;
	Format = DEPTH32F_STENCIL8;
	//Format = DEPTH_COMPONENT32F;
        //Format = DEPTH24STENCIL8;
}

RenderTexture rt_Material 
{
	MSAA = {0,0};
	Size = ApplicationDefined;
	Format = RGBA8;
}

// will lead to a FBO creation in a specific repository. nvFX lib will keep track of the Id
FBO deferredRenderFBO
{
    Color = { rt_Normal, rt_Color, rt_Material /*, dbgPosition*/ };
    DepthStencil = rt_Depth;
}

RenderTexture rt_DeferredResult
{
	MSAA = {0,0};
	Size = ApplicationDefined;
	Format = RGBA8;
}

RenderBuffer rb_DepthStencil
{
    MSAA = {0,0};
    Size = ApplicationDefined;
    Format = DEPTH24STENCIL8;
}

// Post processing
RenderTexture rt_PostProcessInput
{
	MSAA = {0,0};
	Size = ApplicationDefined;
	Format = RGBA8;
}

RenderTexture rt_PostProcessOutput
{
	MSAA = {0,0};
	Size = ApplicationDefined;
	Format = RGBA8;
}

FBO PostProcessInput {
	Color = {rt_PostProcessInput};
}

FBO PostProcessOutput {
	Color = {rt_PostProcessOutput};
}

/// "cubemap" (self) for reflections
RenderTexture rt_ReflectionColor {
	MSAA = {0,0};
	Size = {512,512};
	Format = RGBA8;    	
}

RenderTexture rt_ReflectionNormals {
	MSAA = {0,0};
	Size = {512,512};
	Format = RG32F;    	
}

RenderTexture rt_ReflectionMaterial {
	MSAA = {0,0};
	Size = {512,512};
	Format = RGBA8;    	
}

RenderTexture rt_ReflectionDepth {
	MSAA = {0,0};
	Size = {512,512};
	Format = DEPTH32F_STENCIL8;    	
}

// final textures per side
RenderTexture rt_ReflectionsFront
{
	MSAA = {0,0};
	Size = {512,512};
	Format = RGBA8;      
}
RenderTexture rt_ReflectionsBack
{
	MSAA = {0,0};
	Size = {512,512};
	Format = RGBA8;      
}
RenderTexture rt_ReflectionsLeft
{
	MSAA = {0,0};
	Size = {512,512};
	Format = RGBA8;      
}
RenderTexture rt_ReflectionsRight
{
	MSAA = {0,0};
	Size = {512,512};
	Format = RGBA8;      
}
RenderTexture rt_ReflectionsTop
{
	MSAA = {0,0};
	Size = {512,512};
	Format = RGBA8;      
}

RenderTexture rt_ReflectionsDown
{
	MSAA = {0,0};
	Size = {512,512};
	Format = RGBA8;      
}

FBO ReflectionsDeferred
{
	Color = {rt_ReflectionNormals, rt_ReflectionColor, rt_ReflectionMaterial};
	DepthStencil = rt_ReflectionDepth; 
}


FBO ReflectionsLightFBOFront 
{
	Color = { rt_ReflectionsFront };
}


FBO ReflectionsLightFBOBack
{
	Color = { rt_ReflectionsBack };
}


FBO ReflectionsLightFBOLeft 
{
	Color = { rt_ReflectionsLeft };
}


FBO ReflectionsLightFBORight 
{
	Color = { rt_ReflectionsRight };
}


FBO ReflectionsLightFBOTop
{
	Color = { rt_ReflectionsTop };
}


FBO ReflectionsLightFBODown 
{
	Color = { rt_ReflectionsDown };
}


GLSLShader {
    vec3 reconstructPosition(in float depth, in vec2 coord);	
	vec3 getNormal(vec2 normalxy);
}

GLSLShader global deferredBufferInterpretation {
    vec3 reconstructPosition(in float depth, in vec2 coord){
		vec4 v = (m4_ProjectionI * (vec4(coord, depth, 1.0) * 2.0 - 1.0));
		return v.xyz / v.w;
    }
	
	vec3 getNormal(vec2 normalxy) {
		vec3 normal;
		normal.z = length(normalxy.xy)*2.0 - 1.0;
		normal.z = normal.z * normal.z;
		normal.xy = normalize(normalxy.xy) * sqrt(1-normal.z*normal.z);
		return normal;
	}
}

