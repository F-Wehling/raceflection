//
//
// will create the texture resource
// can be then used with "SamplerResource(diffuseSampler) = NAME;"

// will lead to a creation of a texture in a specific repository. nvFX will keep track of the Id
RenderTexture rt_Color
{
    MSAA = {0,0};
    Size = ApplicationDefined;// a way to dynamically define the size of the resource to create
    Format = RGBA8;
}

RenderTexture rt_Normals
{
    MSAA = {0,0};
    Size = ApplicationDefined;
    Format = RGBA8;
}

RenderTexture rt_DepthStencil 
{
	MSAA = {0,0};
	Size = ApplicationDefined;
	Format = R32F;
}

RenderBuffer rb_DepthStencil
{
    MSAA = {0,0};
    Size = ApplicationDefined;
    Format = DEPTH24STENCIL8;
}
// will lead to a FBO creation in a specific repository. nvFX lib will keep track of the Id
FBO deferredRenderFBO
{
    Color = { rt_Color, rt_Normals, rt_DepthStencil };
    DepthStencil = rb_DepthStencil;
}


#include "predefines.fxh"
#include "fragOutput.fxh"

///////////////////////////////////////////////////////////////////////////////////////////////////
// sampler states are outside of GLSL.
//
SamplerState nearestSampler
{
    TEXTURE_MIN_FILTER = NEAREST;
    TEXTURE_MAG_FILTER = NEAREST;
}

//--------------------------------------------------------------------------------------
//    Fullscreen quad Vertex shader
//--------------------------------------------------------------------------------------
GLSLShader global fullscreenquadVS
{
    void main()
    {}
}

GLSLShader global fullscreenquadGS
{
	layout(points) in;
	layout(triangle_strip, max_vertices = 4) out;
	
	out vec2 texCoord;
	
	void main() {
		gl_Position = vec4(1.0, 1.0, 0.5, 1.0);
		texCoord = vec2(1.0 ,1.0);

		EmitVertex();

		gl_Position = vec4(-1.0, 1.0, 0.5, 1.0);
		texCoord = vec2(0.0 ,1.0);

		EmitVertex();

		gl_Position = vec4(1.0, -1.0, 0.5, 1.0);
		texCoord = vec2(1.0 , 0.0);

		EmitVertex();

		gl_Position = vec4(-1.0,-1.0, 0.5, 1.0);
		texCoord = vec2(0.0 ,0.0);

		EmitVertex();

		EndPrimitive();
	}
}
//--------------------------------------------------------------------------------------
//    Simple texture fetch
//--------------------------------------------------------------------------------------
GLSLShader global fullscreenquadPS
{
    // we can keep local some uniforms. So they aren't visible outside from others
    uniform sampler2D quadSampler;
    layout(location=1) in vec3 tc;
    layout(location=0) out vec4 color;
    void main()
    {
        color = /*vec4(0,0,0.2,0.0) + */texture(quadSampler, tc.xy);
    }
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
// Test of a scene-level Technique
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
Technique TwoPassesTwoFBOs
{
    Pass renderScene
    {
        DEPTH_TEST = true;
        CULL_FACE = true;
        BLEND = false;
        RenderMode = SceneShading;
        RenderGroup = 11;
        ClearMode = all;
        ClearColor[0] = {0,0.0,0.0,1};
        ClearColor[1] = {0.3,0.2,0.4,1};
        CurrentTarget = deferredRenderFBO;
        FragmentProgramOverride<"out"> = ColorAndNormals;
    }
    Pass blitme
    {
        BlitFBOToActiveTarget = deferredRenderFBO;
        //ViewPort = AppDefined;
        CurrentTarget = backbuffer;
    }
    // Example of a quad rendering
    Pass copy_to_texture
    {
        // Copy via a Fullscreen quad and texture fetch in a shader
        DEPTH_TEST = false;
        CULL_FACE = false;
        BLEND = true;
        BLEND_SRC = ONE_MINUS_CONSTANT_COLOR;
        BLEND_DST = CONSTANT_COLOR;
        BLEND_COLOR = {0.3,0.3,0.3,0.3};
        RenderMode = DebugQuad3;//render_fullscreen_quad; // Only draws a quad... Won't change any renderstate. So Depth test/Cullface would be needed
        VertexProgram = fullscreenquadVS;
		GeometryProgram = fullscreenquadGS;
        SamplerResource(quadSampler) = rt_Normals;
        SamplerState(quadSampler) = nearestSampler; // Strange: I can't use any other sampling filtering to not break the FBO
        FragmentProgram = fullscreenquadPS;
        CurrentTarget = backbuffer;
    }
}

// Simpler case
Technique NormalsAsColorToBackbuffer
{
    Pass renderScene
    {
        DEPTH_TEST = true;
        CULL_FACE = true;
        BLEND = false;
        RenderMode = SceneShading;
        RenderGroup = 11;
        ClearMode = all;
        ClearColor[0] = {0,0.0,0.0,1};
        ClearColor[1] = {0.3,0.2,0.4,1};
        CurrentTarget = backbuffer;
        FragmentProgramOverride<"out"> = NormalsAsColor;
    }
}

// display TC
Technique TcAsColorToBackbuffer
{
    Pass renderScene
    {
        DEPTH_TEST = true;
        CULL_FACE = true;
        BLEND = false;
        RenderMode = SceneShading;
        RenderGroup = 11;
        ClearMode = all;
        ClearColor[0] = {0,0.0,0.0,1};
        ClearColor[1] = {0.3,0.2,0.4,1};
        CurrentTarget = backbuffer;
        FragmentProgramOverride<"out"> = TcAsColor;
    }
}

