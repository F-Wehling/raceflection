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

/* //ignore for non-debug
RenderTexture dbgPosition 
{
	MSAA = {0,0};
	Size = ApplicationDefined;
	Format = RGB32F;

}*/

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

FBO deferredRenderLightFBO 
{
	Color = { rt_DeferredResult };
	DepthStencil = rb_DepthStencil;
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


//
/// handle the light definition
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

