//////////////////////////////////////////////////////////

#include "predefines.fxh"
#include "constantBuffer.fxh"

uniform vec4 scaleBias = {0,0,0,1.0}; // x,y,z bias and w==scale
uniform vec3 lightPos[] = {{10,10,10},{10,10,-10}};
uniform vec3 lightColor[] = {{0.8,0.8,0.8},{0.5,0.5,0.7}};

//////////////////////////////////////////////////////////

RasterizationState rasterState1
{
    LINE_WIDTH = 1;
    POLYGON_MODE={FRONT_AND_BACK, FILL};
}
RasterizationState rasterStateWire
{
    LINE_WIDTH = 2;
    POLYGON_MODE={FRONT_AND_BACK, LINE};
}
DepthStencilState dstStateFloor
{
    DEPTH_TEST = true;
    DEPTH_WRITEMASK = true;
    DEPTH_FUNC = LEQUAL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Functions that we will use to output fragments in various ways
//
#include "fragOutput.fxh"
//////////////////////////////////////////////////////////

uniform sampler2D specSampler;
uniform sampler2D diffuseSampler;

///////////////////////////////////////////////////////////////////////////////////////////////////
// sampler states are outside of GLSL.
// names allows to make them match with the texture/sampler of GLSL (HLSL, too)
// or maybe we would require the runtime to connect them explicitly...
//
SamplerState specSamplerState
{
    TEXTURE_MIN_FILTER = LINEAR_MIPMAP_LINEAR;
    TEXTURE_MAG_FILTER = LINEAR;
    //AddressS = REPEAT;
    //AddressT = REPEAT;
    //AddressR = REPEAT;
}
SamplerState defaultSamplerState
{
    TEXTURE_MIN_FILTER = LINEAR_MIPMAP_LINEAR;
    TEXTURE_MAG_FILTER = LINEAR;
}
SamplerState nearestSampler
{
    TEXTURE_MIN_FILTER = NEAREST;
    TEXTURE_MAG_FILTER = NEAREST;
}
//
// Texture Resources
//
TextureResource2D Default_color <
    defaultFile = "gargoyleMossyDiffuse.dds";
> {
    // this is a possible alternate way to setup sampler-states...
    // in OpenGL, sampler-states are defined for each texture
    // sampler-states specified in the Pass override this one
    SamplerState = defaultSamplerState;
}
TextureResource2D Default_spec <
    defaultFile = "gargoyleMossySpec.dds";
> {
    SamplerState = defaultSamplerState;
}

////////////////////////////////////////////////////////////////////
/// VERTEX PROGRAM /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
GLSLShader ObjectVS
{
    layout(location=0) in vec3 Position;
    layout(location=1) in vec3 Normal;
	layout(location=2) in vec4 Color;
    layout(location=3) in vec3 TexCoord;
    layout(location=5) in vec3 Tangent;
    layout(location=6) in vec3 Binormal;

    layout(location=0) out vec3 outNormal;
    layout(location=1) out vec3 outTexCoord;
    layout(location=4) out vec3 outPosition;
    layout(location=5) out vec3 outViewDir;
	layout(location=6) out vec4 outColor;

    out gl_PerVertex {
        vec4 gl_Position;
    };

    void main(void)
    {
        vec3 N    = normalize(mat3(m4_ModelIT) * Normal);
        vec4 Pos    = vec4(Position, 1.0);
        Po.xyz += scaleBias.xyz;
        Po.xyz *= scaleBias.w;
        vec3 world_pos = (m4_World * Po).xyz;

        gl_Position = (m4_WorldViewProj * Po);
        // Here we do this on purpose to test the UBO use :
        //gl_Position = (m4_World * Po);
        //gl_Position = (m4_View * gl_Position); // UBO
        //gl_Position = (m4_Proj * gl_Position); // UBO

        outNormal       = N;
        outPosition     = world_pos;
        outTexCoord     = TexCoord;
        outViewDir		= normalize(eyePos - world_pos);
    }
}
////////////////////////////////////////////////////////////////////
/// FRAGMENT PROGRAM ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
GLSLShader ObjectPS
{
    layout(location=0) in vec3 inNormal;
    layout(location=1) in vec3 inTexCoord;
    layout(location=4) in vec3 inPosition;
    layout(location=5) in vec3 inViewDir;
	layout(location=6) in vec4 inColor;

    void main() 
    {
        vec4 texcolor = inColor;//texture(diffuseSampler, vec2(inTC.x, 1.0 - inTC.y));
        vec4 texspec = vec4(1.0, 0.0, 0.0, 1.0);//3.0 * texture(specSampler, vec2(inTC.x, 1.0 - inTC.y));
        vec3 normal = normalize(inNormal);
        vec3 viewDir = normalize(inViewDir);

        // see fragOutput.fxh
		vec2 _tmp = vec2(1.0,0.0);
		vec4 generic_1 = vec4(_tmp, 0.0,0.0);
		vec4 materialParameter = vec4(1.0, 0.0, 0.0, 0.0);
        finalColor(normal, vec4(result, 0.0), inTexCoord, gl_FragCoord.z, generic_1, materialParameter);
    }
}
////////////////////////////////////////////////////////////////////
/// Floor things
////////////////////////////////////////////////////////////////////
namespace floor
{
    ////////////////////////////////////////////////////////////////////
    /// FLOOR's VERTEX PROGRAM ///////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////
    GLSLShader VS
    {
        layout(location=0) in vec4 Position;
        out gl_PerVertex {
            vec4 gl_Position;
        };
        void main(void)
        {
            gl_Position = m4_ViewProj * Position;
        }
    }
    ////////////////////////////////////////////////////////////////////
    /// FLOOR's FRAGMENT PROGRAM ///////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////
    GLSLShader PS
    {
        layout(location=0) out vec4 color;
        void main() 
        {
            color = vec4(0.3, 0.5, 0.7, 1.0);
        }
    }
}
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
// Material Technique
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

Technique Init
{
    Pass init
    {
        PolygonOffset = {1.0,1.0};
        DEPTH_TEST = True;
        DEPTH_FUNC = LEQUAL;
        DEPTH_MASK = True;
    }
}

Technique Floor
{
    Pass p0
    {
        //RasterizationState = rasterStateWire;
        //DepthStencilState = dstStateFloor;

        VertexProgram = floor::VS;
        FragmentProgram = floor::PS;
    }
}
Technique Object
{
    Pass p0
    {
        RasterizationState = rasterState1;
        // we assume here that the resource will be set by the app
        SamplerState(diffuseSampler) = defaultSamplerState;
        SamplerResource(diffuseSampler) = Default_color;
        SamplerState(specSampler) = defaultSamplerState;
        SamplerResource(specSampler) = Default_spec;
        VertexProgram = ObjectVS;
        // Here we link 2 parts together to finally have a consistent shader
        FragmentProgram = ObjectPS;
        // See fragOutput.glslfxh for ColorAsColor module implementing finalColor()
        // the "out" is a group name, to allow later to find back this pass-state (and override it)
        FragmentProgram<"out"> = DeferredGeometry;
    }
}
