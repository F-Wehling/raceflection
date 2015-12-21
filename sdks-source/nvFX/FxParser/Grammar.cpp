/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         FxParserparse
#define yylex           FxParserlex
#define yyerror         FxParsererror
#define yydebug         FxParserdebug
#define yynerrs         FxParsernerrs

#define yylval          FxParserlval
#define yychar          FxParserchar

/* Copy the first part of user declarations.  */
#line 36 "Grammar.ypp" /* yacc.c:339  */

    /* Documentation on the keywords and grammar created in : ..\doxygen\Keywords.dxt */
    #include  <math.h>
    #include  <assert.h>
    #include  <stdio.h>
	#ifdef MEMORY_LEAKS_CHECK
	#pragma message("build will Check for Memory Leaks!")
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
    inline void* operator new(size_t size, const char *file, int line)
    {
       return ::operator new(size, 1, file, line);
    }

    inline void __cdecl operator delete(void *ptr, const char *file, int line) 
    {
       ::operator delete(ptr, _NORMAL_BLOCK, file, line);
    }

    #define DEBUG_NEW new( __FILE__, __LINE__)
    #define MALLOC_DBG(x) _malloc_dbg(x, 1, __FILE__, __LINE__);
    #define malloc(x) MALLOC_DBG(x)
    #define new DEBUG_NEW
	#else
	#include <stdlib.h>
	#endif
    #include  <string.h>
    #include  <vector>
    #include  <map>
    #include  <string>

    #include "FxLibEx.h"
    #include "FxParser.h"

    using namespace nvFX;
    #ifndef LOGD
    #define LOGD(...)  { /*nvFX::printf(__VA_ARGS__);*/ }
    #endif
    #ifndef LOGI
    #define LOGI(...)  { nvFX::printf(__VA_ARGS__); }
    #endif

    #ifndef OGLES2
    #   define USEGLSLCOMPUTE
    #endif
    /*
     * values taken from OpenGL
     *
     */
    #define GL_TRUE 1
    #define GL_FALSE 0


    /*
     * Here we store temporary pointers of current objects being processed by the grammar
     * the reason : the grammar executes from the deepest part to the highest part of the tree
     * and the FX library doesn the opposite : objects are created by parents
     */
    IContainerEx*       curContainer = NULL;
    ITechniqueEx*       curTechnique;
    IPassEx*            curPass;
    IShaderEx*          curShader;
    IProgramEx*         curProgram;
    ISamplerStateEx*    curSamplerState;
    IStateGroupRasterEx* curRState;
    IStateGroupCSEx*    curCSState;
    IStateGroupDSTEx*   curDSTState;
    IStateGroupPathEx*  curPRState;
    IResourceEx*        curResource;
    IFrameBufferObjectEx* curFBO;
    IUniformEx*         curUniform;
    IAnnotationEx*      curAnnotations;
    IAnnotationEx*      namespaceAnnotations;
    ICstBufferEx*       curCstBuffer;
    bool                bCurCstBufferAlreadyPopulated; // true to prevent creation of sub-uniforms
    std::string         namespaceName;
    IShaderModuleRepositoryEx* pShdRep;
    IFrameBufferObjectsRepositoryEx* pFboRep;
    IResourceRepositoryEx* pResRep;
    IUniformRepositoryEx* pURep;
    ICstBufferRepositoryEx* pCRep;
    //
    // Those functions are used to create the states if NULL
    // this case happens if states are defined in a Pass directly :
    // we will then create implicit stategroups for this pass
    //
    void check_curDSTState()
    {
        if(curDSTState) return;
        LOGD("no current DST State Group. We are in a Pass, Create one for this Pass\n");
        assert(curPass);
        curDSTState = curContainer->createStateGroup_DST(curPass->getName())->getExInterface();
        curPass->createState(curDSTState);
    }
    void check_curRState()
    {
        if(curRState) return;
        LOGD("no current Rasterization State Group. We are in a Pass, Create one for this Pass\n");
        assert(curPass);
        curRState = curContainer->createStateGroup_Raster(curPass->getName())->getExInterface();
        curPass->createState(curRState);
    }
    void check_curCSState()
    {
        if(curCSState) return;
        LOGD("no current Color sample State Group. We are in a Pass, Create one for this Pass\n");
        assert(curPass);
        curCSState = curContainer->createStateGroup_CS(curPass->getName())->getExInterface();
        curPass->createState(curCSState);
    }
#ifndef OGLES2
    void check_curPRState()
    {
        if(curPRState) return;
        LOGI("no current rendering-path State Group. We are in a Pass, Create one for this Pass\n");
        assert(curPass);
        curPRState = curContainer->createStateGroup_PR(curPass->getName())->getExInterface();
        curPass->createState(curPRState);
    }
#endif
    extern GLenum getGLEnumValue(GLenum BisonEnum);

    void createUniform(nvFX::IUniform::Type T, const char* name, const char* sem, bool global=true, IUniform::PrecisionType precision=IUniform::PHighP)
    {
        extern void yyerror(char  const  *);
        IUniform *p = NULL;
        if(curCstBuffer) // if a constant buffer is available, populate it
        {
            // only if allowed (if it came from an existing Global one, we don't want to add anything
            if(bCurCstBufferAlreadyPopulated) {
                LOGD("Skipping the sub-uniform of the constant\n");
                p = curCstBuffer->findUniform(name);
                if(p == NULL)
                    yyerror("Declaration of the CstBuffer differs from the one already in global area\n");
            } else {
                p = curCstBuffer->getExInterface()->createUniform(name, sem);
                p->getExInterface()->setType(T);
            }
        } else { // otherwise just make a regular uniform creation at the root of the effect
            // TODO: we should take into account the name-space !!
            p = curContainer->findUniform(name);
            if(!p)
            {
                p = curContainer->createUniform(name, namespaceName.c_str(), sem, global);
                if(p) {
                    p->getExInterface()->setType(T);
                    p->getExInterface()->setPrecision(precision);
                }
            }
        }
        curUniform = p ? p->getExInterface() : NULL;
    }
    inline int getTypeDim(IUniform::Type t)
    {
        switch(t)
        {
        case IUniform::TFloat:
        case IUniform::TInt:
        case IUniform::TBool:
            return 1;
        case IUniform::TVec2:
        case IUniform::TInt2:
        case IUniform::TBool2:
            return 2;
        case IUniform::TVec3:
        case IUniform::TInt3:
        case IUniform::TBool3:
            return 3;
        case IUniform::TVec4:
        case IUniform::TInt4:
        case IUniform::TBool4:
            return 4;
        }
        return 0;
    }

#line 252 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "Grammar.hpp".  */
#ifndef YY_FXPARSER_SERVERS_KRAG_HDD_HOME3_KANSTEEG_GP_PROJECT_RACEFLECTION_SDKS_SOURCE_NVFX_FXPARSER_GRAMMAR_HPP_INCLUDED
# define YY_FXPARSER_SERVERS_KRAG_HDD_HOME3_KANSTEEG_GP_PROJECT_RACEFLECTION_SDKS_SOURCE_NVFX_FXPARSER_GRAMMAR_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int FxParserdebug;
#endif
/* "%code requires" blocks.  */
#line 214 "Grammar.ypp" /* yacc.c:355  */

    #include  <vector>
    #include  <string>
    #include  "FxLibEx.h"
    using namespace nvFX;

    /*#define yyparse         FxParserparse
    #define yylex           FxParserlex
    #define yyerror         FxParsererror
    #define yylval          FxParserlval
    #define yychar          FxParserchar
    #define yydebug         FxParserdebug
    #define yynerrs         FxParsernerrs

    #define yy_create_buffer FxParser_create_buffer
    #define yy_delete_buffer FxParser_delete_buffer
    #define yy_scan_buffer FxParser_scan_buffer
    #define yy_scan_string FxParser_scan_string
    #define yy_scan_bytes FxParser_scan_bytes
    #define yy_flex_debug FxParser_flex_debug
    #define yy_init_buffer FxParser_init_buffer
    #define yy_flush_buffer FxParser_flush_buffer
    #define yy_load_buffer_state FxParser_load_buffer_state
    #define yy_switch_to_buffer FxParser_switch_to_buffer
    #define yyin FxParserin
    #define yyleng FxParserleng
    #define yylex FxParserlex
    #define yyout FxParserout
    #define yyrestart FxParserrestart
    #define yytext FxParsertext
    #define yylineno FxParserlineno
    #define yywrap FxParserwrap
    */
    #pragma warning(disable:4996)

    extern int  yydebug;
    extern int  yyparse ();
    extern void yyerror(char  const  *);
    extern int  yylex();
    extern void lex_pop_state();
    extern void lex_push_state();
    extern FILE *yyin;
    extern FILE *yyout;
    extern std::string file_name; // the name of the current file (when in #include, for example)
    extern int  line_num;
    extern int  bracket_level;

    extern IContainerEx*   curContainer;

    //typedef std::vector<CgShader*>      CgShaderList;
    typedef std::vector<IShader*>        ShaderList;
    typedef std::vector<float>          FloatList;
    typedef std::vector<FloatList* >    FloatVecList; ///<vector list
    typedef std::vector<int>            IntList;
    typedef std::vector<IntList* >      IntVecList; ///<vector list
    typedef std::vector</*bool*/int>    BoolList; // bool ran me into compile err in XCode
    typedef std::vector<BoolList* >     BoolVecList; ///<vector list
    typedef std::vector<std::string>    VarNameList;
    struct Argument {
        Argument(ArgumentType type_, std::string *name_) : type(type_), name(*name_) {};
        ArgumentType    type;
        std::string     name;
    };
    typedef std::vector<Argument*>          ArgList;
    typedef std::vector<ArgVal*>            ArgValList;
    struct SUniformQualifiers
    {
        bool hasUniformKeyword; // not really necessary... but hey...
        bool global;
        IUniform::PrecisionType precisionType;
    };
    struct ValueList
    {
        FloatVecList*   floatVecList;
        IntVecList*     intVecList;
        BoolVecList*    boolVecList;
    };
    struct SASzSAUva
    {
        int             arraySz;
        std::string*    semantic;
        ValueList*      valueList;
    };
    struct Sglobal_varname
    {
        bool            global;
        std::string*    varName;
    };

    /*struct Variable
    {
        std::string name;
        //...
    };*/

    #ifndef LOGD
    #define  LOGD(...)  { /*nvFX::printf(__VA_ARGS__);*/ }
    #endif
    #ifndef LOGI
    #define  LOGI(...)  { ::printf(__VA_ARGS__); }
    #endif
    extern float *flattenFVecList(int t, int vecDim, FloatVecList* pfloatVecList);
    extern int   *flattenIVecList(int t, int vecDim, IntVecList* pintVecList);
    extern bool  *flattenBVecList(int t, int vecDim, BoolVecList* pboolVecList);

    //
    // Check 
    //
    inline bool checkArgCompatibility(ArgVal *argval, IShaderEx::Argument &arg)
    {
        if(argval->type == IUniform::TUniform)
        {
            ArgVal argval2;
            argval2.type = argval->val.pUniform->getType();
            // recursive walk through to finally check the real time
            return checkArgCompatibility(&argval2, arg);
        }
        switch(arg.t)
        {
        case TInt:
        case TUInt:
            if(argval->type == IUniform::TInt)
                return true;
            break;
        case TInt2:
            if(argval->type == IUniform::TInt2)
                return true;
            break;
        case TInt3:
            if(argval->type == IUniform::TInt3)
                return true;
            break;
        case TInt4:
            if(argval->type == IUniform::TInt4)
                return true;
            break;
        //case TUInt2:        case TUInt3:        case TUInt4:
        case TFloat:
            if(argval->type == IUniform::TFloat)
                return true;
            if(argval->type == IUniform::TInt)
                return true;
            break;
        //case TVec2:        case TVec3:        case TVec4:
        case TIntPtr:
        case TUIntPtr:
        case TFloatPtr:
            switch(argval->type)
            {
            case IUniform::TCB:
            case IUniform::TTexture:
            case IUniform::TTexture1D:
            case IUniform::TTexture2D:
            case IUniform::TTexture2DRect:
            case IUniform::TTexture3D:
            case IUniform::TTextureCube:
                return true;
            default:
                break;
            }
            return false;
        //case TInt2Ptr:        case TInt3Ptr:        case TInt4Ptr:
        //case TUInt2Ptr:        case TUInt3Ptr:        case TUInt4Ptr:
        //case TVec2Ptr:        case TVec3Ptr:        case TVec4Ptr:
        //TBool, TBool2, TBool3, TBool4,
        //TMat2, TMat3, TMat4, 
            break;
        }
        return false;
    }

#line 454 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:355  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NAMESPACE = 258,
    FX_TRUE = 259,
    FX_FALSE = 260,
    FX_FLOAT = 261,
    FX_INT = 262,
    FX_BOOL = 263,
    FX_STR = 264,
    GLSLSHADER = 265,
    HLSL10SHADER = 266,
    HLSL11SHADER = 267,
    CGSHADER = 268,
    SAMPLER_STATE = 269,
    DST_STATE = 270,
    PR_STATE = 271,
    COLOR_SAMPLE_STATE = 272,
    RASTERIZATION_STATE = 273,
    TECHNIQUE = 274,
    PASS = 275,
    CODEBODY = 276,
    VARNAME = 277,
    TEXTURERESOURCE = 278,
    TEXTURERESOURCE1D = 279,
    TEXTURERESOURCE2D = 280,
    TEXTURERESOURCE3D = 281,
    TEXTURERESOURCERECT = 282,
    TEXTURERESOURCECUBE = 283,
    VOID_TYPE = 284,
    UNSIGNED = 285,
    HIGHP = 286,
    MEDIUMP = 287,
    LOWP = 288,
    UNIFORM = 289,
    CSTBUFFER = 290,
    FLOAT_TYPE = 291,
    FLOAT2_TYPE = 292,
    FLOAT3_TYPE = 293,
    FLOAT4_TYPE = 294,
    MAT2_TYPE = 295,
    MAT3_TYPE = 296,
    MAT4_TYPE = 297,
    BOOL_TYPE = 298,
    BOOL2_TYPE = 299,
    BOOL3_TYPE = 300,
    BOOL4_TYPE = 301,
    INT_TYPE = 302,
    INT2_TYPE = 303,
    INT3_TYPE = 304,
    INT4_TYPE = 305,
    TEXTURE1D_TYPE = 306,
    TEXTURE2D_TYPE = 307,
    TEXTURE2DRECT_TYPE = 308,
    TEXTURE3D_TYPE = 309,
    TEXTURECUBE_TYPE = 310,
    SAMPLER1D_TYPE = 311,
    SAMPLER2D_TYPE = 312,
    SAMPLER2DRECT_TYPE = 313,
    SAMPLER3D_TYPE = 314,
    SAMPLERCUBE_TYPE = 315,
    EXTENSION = 316,
    SEPARATE_SHADER = 317,
    DEPTHMASK = 100,
    DEPTHFUNC = 101,
    VERTEXPROGRAM = 318,
    FRAGMENTPROGRAM = 319,
    GEOMETRYPROGRAM = 320,
    HULLPROGRAM = 321,
    EVALPROGRAM = 322,
    SHDPROFILE = 323,
    SAMPLERRESOURCE = 324,
    SAMPLERTEXUNIT = 325,
    SETSAMPLERSTATE = 326,
    SETDSTSTATE = 327,
    SETRASTERIZATIONSTATE = 328,
    SETCOLORSAMPLESTATE = 329,
    IMAGERESOURCE = 330,
    IMAGEUNIT = 331,
    IMAGEACCESS = 332,
    IMAGELAYER = 333,
    IMAGELAYERED = 334,
    WRITE_ONLY = 335,
    READ_ONLY = 336,
    READ_WRITE = 337,
    VERTEXPROGRAMOVERRIDE = 338,
    FRAGMENTPROGRAMOVERRIDE = 339,
    GEOMETRYPROGRAMOVERRIDE = 340,
    HULLPROGRAMOVERRIDE = 341,
    EVALPROGRAMOVERRIDE = 342,
    IMAGEACCESSOVERRIDE = 343,
    IMAGELAYEROVERRIDE = 344,
    IMAGELAYEREDOVERRIDE = 345,
    IMAGERESOURCEOVERRIDE = 346,
    UNIFORMOVERRIDE = 347,
    IMAGEUNITOVERRIDE = 348,
    TEXTURERESOURCEOVERRIDE = 349,
    SAMPLERRESOURCEOVERRIDE = 350,
    SAMPLERTEXUNITOVERRIDE = 351,
    SETSAMPLERSTATEOVERRIDE = 352,
    GLACCUM = 353,
    GLLOAD = 354,
    GLRETURN = 355,
    GLMULT = 356,
    GLADD = 357,
    GLNEVER = 358,
    GLLESS = 359,
    GLEQUAL = 360,
    GLGREATER = 361,
    GLNOTEQUAL = 362,
    GLALWAYS = 363,
    GLLEQUAL = 364,
    GLGEQUAL = 365,
    GLKEEP = 366,
    GLREPLACE = 367,
    GLINCR = 368,
    GLDECR = 369,
    GLUPPERLEFT = 370,
    GLLOWERLEFT = 371,
    GLFRONTLEFT = 372,
    GLFRONTRIGHT = 373,
    GLBACKLEFT = 374,
    GLBACKRIGHT = 375,
    GLFRONT = 376,
    GLBACK = 377,
    GLLEFT = 378,
    GLRIGHT = 379,
    GLFRONTANDBACK = 380,
    GLCCW = 381,
    GLCW = 382,
    GLPOINT = 383,
    GLLINE = 384,
    GLFILL = 385,
    GLZERO = 386,
    GLONE = 387,
    GLSRCCOLOR = 388,
    GLONEMINUSSRCCOLOR = 389,
    GLSRCALPHA = 390,
    GLONEMINUSSRCALPHA = 391,
    GLDSTALPHA = 392,
    GLONEMINUSDSTALPHA = 393,
    GLDSTCOLOR = 394,
    GLONEMINUSDSTCOLOR = 395,
    GLSRCALPHASATURATE = 396,
    GLCONSTANTCOLOR = 397,
    GLONEMINUSCONSTANTCOLOR = 398,
    GLCONSTANTALPHA = 399,
    GLONEMINUSCONSTANTALPHA = 400,
    GLFUNCADD = 401,
    GLMIN = 402,
    GLMAX = 403,
    GLFUNCSUBTRACT = 404,
    GLFUNCREVERSESUBTRACT = 405,
    GLCLEAR = 406,
    GLSET = 407,
    GLCOPY = 408,
    GLCOPYINVERTED = 409,
    GLNOOP = 410,
    GLINVERT = 411,
    GLAND = 412,
    GLNAND = 413,
    GLOR = 414,
    GLNOR = 415,
    GLXOR = 416,
    GLEQUIV = 417,
    GLANDREVERSE = 418,
    GLANDINVERTED = 419,
    GLORREVERSE = 420,
    GLORINVERTED = 421,
    GLTEXTUREMAGFILTER = 422,
    GLTEXTUREMINFILTER = 423,
    GLTEXTUREWRAPS = 424,
    GLTEXTUREWRAPT = 425,
    GLTEXTUREWRAPR = 426,
    GLNEAREST = 427,
    GLLINEAR = 428,
    GLNEARESTMIPMAPNEAREST = 429,
    GLLINEARMIPMAPNEAREST = 430,
    GLNEARESTMIPMAPLINEAR = 431,
    GLLINEARMIPMAPLINEAR = 432,
    GLCLAMPTOEDGE = 433,
    GLCLAMP = 434,
    GLREPEAT = 435,
    GLMIRROREDREPEAT = 436,
    GLPOINTSIZE = 437,
    GLPOINTSMOOTH = 438,
    GLPOINTSPRITE = 439,
    GLPROGRAMPOINTSIZE = 440,
    GLPOINTSPRITECOORDORIGIN = 441,
    GLLINEWIDTH = 442,
    GLLINESMOOTH = 443,
    GLCOMBINEDLINESTIPPLE = 444,
    GLLINESTIPPLEPATTERN = 445,
    GLLINESTIPPLEREPEAT = 446,
    GLLINESTIPPLE = 447,
    GLCULLFACE = 448,
    GLCULLFACEMODE = 449,
    GLFRONTFACE = 450,
    GLPOLYGONSMOOTH = 451,
    GLPOLYGONMODE = 452,
    GLPOLYGONOFFSET = 453,
    GLPOLYGONOFFSETFACTOR = 454,
    GLPOLYGONOFFSETUNITS = 455,
    GLPOLYGONOFFSETPOINT = 456,
    GLPOLYGONOFFSETLINE = 457,
    GLPOLYGONOFFSETFILL = 458,
    GLPOLYGONSTIPPLEBIT = 459,
    GLRASTERIZERDISCARD = 460,
    GLALPHATEST = 461,
    GLALPHAFUNC = 462,
    GLALPHATESTFUNC = 463,
    GLALPHATESTREF = 464,
    GLBLENDFUNC = 465,
    GLBLENDFUNCSEPARATE = 466,
    GLBLEND = 467,
    GLBLENDSRC = 468,
    GLBLENDSRCRGB = 469,
    GLBLENDSRCALPHA = 470,
    GLBLENDDST = 471,
    GLBLENDDSTRGB = 472,
    GLBLENDDSTALPHA = 473,
    GLBLENDEQUATIONSEPARATE = 474,
    GLBLENDEQUATION = 475,
    GLBLENDEQUATIONRGB = 476,
    GLBLENDEQUATIONALPHA = 477,
    GLBLENDCOLOR = 478,
    GLFRAMEBUFFERSRGB = 479,
    GLDITHER = 480,
    GLCOLORLOGICOP = 481,
    GLLOGICOPMODE = 482,
    GLCOLORWRITEMASK = 483,
    GLMULTISAMPLE = 484,
    GLSAMPLEMASK = 485,
    GLSAMPLEMASKVALUE = 486,
    GLSAMPLEALPHATOONE = 487,
    GLSAMPLEALPHATOCOVERAGE = 488,
    GLDEPTHTEST = 489,
    GLDEPTHFUNC = 490,
    GLDEPTHWRITEMASK = 491,
    GLDEPTHCLAMP = 492,
    GLDEPTHBOUNDSTESTEXT = 493,
    GLDEPTHBOUNDSEXT = 494,
    GLSTENCILTEST = 495,
    GLSTENCILFUNC = 496,
    GLSTENCILFUNCSEPARATE = 497,
    GLSTENCILVALUEMASK = 498,
    GLSTENCILREF = 499,
    GLSTENCILFAIL = 500,
    GLSTENCILPASSDEPTHFAIL = 501,
    GLSTENCILPASSDEPTHPASS = 502,
    GLSTENCILOP = 503,
    GLSTENCILOPSEPARATE = 504,
    GLSTENCILBACKOP = 505,
    GLSTENCILWRITEMASK = 506,
    GLSTENCILBACKFUNC = 507,
    GLSTENCILBACKVALUEMASK = 508,
    GLSTENCILBACKREF = 509,
    GLSTENCILBACKFAIL = 510,
    GLSTENCILBACKPASSDEPTHFAIL = 511,
    GLSTENCILBACKPASSDEPTHPASS = 512,
    GLSTENCILBACKWRITEMASK = 513,
    ATTRIBUTE = 514,
    RENDERGROUP = 515,
    RENDERMODE = 516,
    RENDERMODE_ENUM = 517,
    BLITFBOTOACTIVETARGET = 518,
    CLEARMODE = 519,
    CLEAR_ENUM = 520,
    CURRENTTARGET = 521,
    CLEARCOLOR = 522,
    VIEWPORT = 523,
    FBO = 524,
    RENDERBUFFER = 525,
    RENDERTEXTURE = 526,
    RENDERTEXTURERECT = 527,
    DSTTARGET = 528,
    COLORTARGETS = 529,
    RESOURCEMSAA = 530,
    RESOURCESIZE = 531,
    RESOURCESIZEFACTOR = 532,
    RESOURCERESOLVETARGET = 533,
    RESOURCEFORMAT = 534,
    RESOURCEFORMAT_ENUM = 535,
    APPDEFINED = 536,
    SWAPRESOURCES = 537,
    LOOP = 538,
    ACTIVE = 539,
    OFF = 540,
    GLOBAL = 541,
    LOCAL = 542,
    CUDAKERNEL = 543,
    CUDACODE = 544,
    CUDAMODULE = 545,
    CUDASHAREDMEMORY = 546,
    CUDAGRID = 547,
    CUDABLOCK = 548,
    OPTIX_CONTEXT = 549,
    OPTIX_CONTEXT_ENTRY = 550,
    RAY_GENERATION_PROGRAM = 551,
    EXCEPTION_PROGRAM = 552,
    INTERSECTION_PROGRAM = 553,
    BOUNDING_BOX_PROGRAM = 554,
    MISS_PROGRAM = 555,
    DEFAULT_MATERIAL = 556,
    STACK_SIZE = 557,
    OUTPUT_BUFFER = 558,
    OPTIX_MATERIAL = 559,
    CLOSEST_HIT_PROGRAM = 560,
    ANY_HIT_PROGRAM = 561,
    OPTIX_BUFFER = 562,
    PTX_PROGRAM_FROM_FILE = 563,
    FILE_NAME = 564,
    ENTRY_POINT = 565,
    COMPUTEGROUPS = 566,
    COMPUTEPROGRAM = 567,
    COMPUTEPROGRAMOVERRIDE = 568,
    SVGPATH = 569,
    PSPATH = 570,
    PATHSOURCEOVERRIDE = 571,
    PATHSOURCE = 572,
    GLPATHSTENCILDEPTHOFFSET = 573,
    GLPATHSTENCILFUNC = 574,
    GLPATHFOGGEN = 575,
    GLPATHTEXGEN = 576,
    GLPATHCOLORGEN = 577,
    GLPATHCOVERDEPTHFUNC = 578,
    GLPATHSTROKEWIDTH = 579,
    GLPATHINITIALENDCAP = 580,
    GLPATHTERMINALENDCAP = 581,
    GLPATHINITIALDASHCAP = 582,
    GLPATHTERMINALDASHCAP = 583,
    GLPATHJOINSTYLE = 584,
    GLPATHMITERLIMIT = 585,
    GLPATHDASHOFFSET = 586,
    GLPATHDASHOFFSETRESET = 587,
    GLPATHCLIENTLENGTH = 588,
    GLPATHFILLMODE = 589,
    GLPATHFILLMASK = 590,
    GLPATHFILLCOVERMODE = 591,
    GLPATHSTROKEMASK = 592,
    GLPATHSTROKEREFERENCE = 593,
    GLPATHSTROKECOVERMODE = 594,
    GLPATHTRANSFORMTYPE = 595,
    GLPATHTRANSFORM = 596,
    GLFOGCOORDINATE = 597,
    GLFRAGMENTDEPTH = 598,
    GLOBJECTLINEAR = 599,
    GLPATHOBJECTBOUNDINGBOX = 600,
    GLEYELINEAR = 601,
    GLFLAT = 602,
    GLSQUARE = 603,
    GLROUND = 604,
    GLTRIANGULAR = 605,
    GLMITERREVERT = 606,
    GLMITERTRUNCATE = 607,
    GLBEVEL = 608,
    GLMOVETORESET = 609,
    GLMOVETOCONTINUES = 610,
    GLCOUNTUP = 611,
    GLCOUNTDOWN = 612,
    GLBOUNDINGBOX = 613,
    GLCONVEXHULL = 614,
    GLTRANSLATE2D = 615,
    GLTRANSLATEX = 616,
    GLTRANSLATEY = 617,
    GLTRANSLATE3D = 618,
    GLAFFINE2D = 619,
    GLAFFINE3D = 620,
    GLTRANSPOSEAFFINE2D = 621,
    GLTRANSPOSEAFFINE3D = 622,
    GLLUMINANCE = 623,
    GLALPHA = 624,
    GLINTENSITY = 625,
    GLLUMINANCEALPHA = 626,
    GLRGB = 627,
    GLRGBA = 628,
    NEG = 629
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 416 "Grammar.ypp" /* yacc.c:355  */

    bool            bval;
    bool*           pbval;
    int             ival;
    int*            pival;
    float           fval;
    float*          pfval;
    std::string*    s; // a std::string sub-class to help to deal with "namespace"
    VarNameList*    varNameList;
    //CgShaderList*   cgShaderList;
    ShaderList*     shaderList;
    FloatList*      floatList;
    FloatVecList*   floatVecList;
    IntList*        intList;
    IntVecList*     intVecList;
    BoolList*       boolList;
    BoolVecList*    boolVecList;
    GLenum          glenum;
    IPassState::Type passstateType;
    IUniform::Type  uniformType;
    ResourceType    resourceType;
    /*Variable*       var;*/
    RenderingMode   render_mode;
    ClearMode       clear_mode;
    ResourceFormat  resource_format;
    ArgList*        argList;
    Argument*       argument;
    ArgVal*         argVal;
    ArgValList*     argValList;
    SASzSAUva*      saszsauva;
    IUniform::PrecisionType precisionType;
    SUniformQualifiers uniformQualifiers;
    ValueList*      valueList;
    Sglobal_varname global_varname_annots;

#line 879 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE FxParserlval;

int FxParserparse (void);

#endif /* !YY_FXPARSER_SERVERS_KRAG_HDD_HOME3_KANSTEEG_GP_PROJECT_RACEFLECTION_SDKS_SOURCE_NVFX_FXPARSER_GRAMMAR_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 894 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   5006

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  396
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  125
/* YYNRULES -- Number of rules.  */
#define YYNRULES  748
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  2123

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   629

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     385,   386,   379,   378,   387,   377,   395,   380,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   384,   383,
     391,   376,   392,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   393,     2,   394,   382,     2,     2,     2,     2,     2,
      63,    64,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   388,   390,   389,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   365,   366,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   381
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  1005,  1005,  1006,  1007,  1008,  1010,  1012,  1014,  1016,
    1017,  1018,  1019,  1020,  1021,  1022,  1023,  1024,  1025,  1026,
    1027,  1028,  1029,  1031,  1033,  1034,  1035,  1036,  1037,  1040,
    1049,  1050,  1056,  1057,  1058,  1059,  1060,  1061,  1062,  1063,
    1064,  1065,  1066,  1067,  1068,  1069,  1070,  1071,  1072,  1073,
    1074,  1075,  1083,  1084,  1085,  1086,  1087,  1088,  1093,  1094,
    1100,  1101,  1104,  1108,  1109,  1113,  1116,  1117,  1118,  1136,
    1138,  1143,  1144,  1159,  1176,  1192,  1193,  1200,  1201,  1206,
    1218,  1223,  1231,  1235,  1239,  1243,  1247,  1253,  1257,  1261,
    1264,  1273,  1284,  1288,  1292,  1295,  1299,  1319,  1323,  1327,
    1356,  1361,  1362,  1363,  1367,  1371,  1388,  1389,  1390,  1391,
    1392,  1393,  1394,  1395,  1396,  1397,  1398,  1399,  1400,  1401,
    1402,  1403,  1404,  1405,  1406,  1407,  1408,  1409,  1410,  1411,
    1412,  1413,  1414,  1415,  1416,  1417,  1418,  1419,  1420,  1421,
    1422,  1423,  1424,  1425,  1426,  1427,  1428,  1429,  1430,  1431,
    1432,  1433,  1434,  1435,  1436,  1437,  1438,  1439,  1440,  1441,
    1442,  1443,  1444,  1445,  1446,  1447,  1448,  1449,  1450,  1451,
    1452,  1453,  1454,  1455,  1456,  1458,  1459,  1460,  1461,  1462,
    1463,  1464,  1465,  1466,  1467,  1468,  1469,  1470,  1471,  1472,
    1473,  1474,  1475,  1476,  1477,  1478,  1479,  1480,  1481,  1482,
    1483,  1484,  1485,  1486,  1487,  1488,  1489,  1493,  1494,  1501,
    1502,  1503,  1504,  1509,  1513,  1518,  1524,  1529,  1535,  1540,
    1546,  1551,  1557,  1562,  1568,  1573,  1579,  1584,  1590,  1595,
    1601,  1606,  1612,  1617,  1623,  1628,  1634,  1639,  1645,  1650,
    1656,  1661,  1667,  1682,  1691,  1748,  1767,  1795,  1804,  1809,
    1813,  1830,  1847,  1864,  1881,  1898,  1915,  1932,  1949,  1966,
    1983,  2000,  2013,  2026,  2039,  2052,  2065,  2078,  2099,  2120,
    2141,  2151,  2161,  2171,  2181,  2191,  2204,  2207,  2210,  2224,
    2227,  2241,  2251,  2255,  2262,  2283,  2286,  2292,  2307,  2317,
    2363,  2371,  2388,  2408,  2412,  2421,  2421,  2435,  2435,  2453,
    2457,  2461,  2472,  2490,  2491,  2492,  2498,  2498,  2506,  2506,
    2521,  2522,  2527,  2528,  2529,  2537,  2546,  2547,  2548,  2561,
    2585,  2606,  2627,  2665,  2703,  2704,  2706,  2707,  2712,  2717,
    2726,  2727,  2728,  2732,  2740,  2800,  2841,  2842,  2870,  2877,
    2878,  2879,  2880,  2881,  2882,  2884,  2890,  2890,  2906,  2907,
    2917,  2923,  2927,  2932,  2936,  2940,  2946,  2950,  2963,  2975,
    2982,  2975,  2990,  2997,  2990,  3005,  3012,  3005,  3024,  3025,
    3034,  3048,  3069,  3086,  3093,  3086,  3106,  3107,  3108,  3109,
    3110,  3111,  3112,  3115,  3116,  3117,  3118,  3119,  3126,  3127,
    3130,  3133,  3136,  3139,  3142,  3147,  3147,  3157,  3172,  3173,
    3174,  3175,  3176,  3177,  3178,  3180,  3181,  3182,  3183,  3184,
    3185,  3186,  3187,  3188,  3189,  3190,  3191,  3196,  3197,  3198,
    3200,  3201,  3202,  3203,  3204,  3209,  3210,  3215,  3222,  3223,
    3224,  3225,  3226,  3227,  3229,  3230,  3231,  3236,  3245,  3247,
    3248,  3249,  3250,  3251,  3258,  3259,  3261,  3262,  3263,  3264,
    3265,  3266,  3267,  3268,  3270,  3271,  3272,  3278,  3284,  3285,
    3286,  3287,  3288,  3289,  3290,  3297,  3310,  3311,  3317,  3318,
    3319,  3320,  3321,  3322,  3328,  3329,  3330,  3331,  3332,  3333,
    3334,  3335,  3337,  3338,  3339,  3340,  3341,  3342,  3343,  3344,
    3345,  3346,  3347,  3348,  3349,  3350,  3352,  3353,  3358,  3358,
    3368,  3373,  3374,  3375,  3376,  3377,  3378,  3379,  3380,  3381,
    3382,  3383,  3385,  3386,  3387,  3392,  3401,  3403,  3404,  3405,
    3406,  3407,  3414,  3415,  3417,  3418,  3419,  3424,  3424,  3434,
    3439,  3440,  3441,  3442,  3443,  3444,  3445,  3447,  3448,  3449,
    3450,  3451,  3452,  3453,  3454,  3462,  3463,  3464,  3465,  3466,
    3467,  3468,  3469,  3474,  3474,  3484,  3487,  3488,  3489,  3490,
    3491,  3492,  3493,  3494,  3495,  3496,  3497,  3498,  3499,  3500,
    3501,  3502,  3503,  3504,  3505,  3506,  3507,  3508,  3516,  3517,
    3518,  3523,  3528,  3533,  3538,  3543,  3548,  3553,  3558,  3563,
    3568,  3573,  3578,  3583,  3588,  3593,  3598,  3603,  3608,  3613,
    3618,  3629,  3635,  3635,  3651,  3661,  3662,  3669,  3675,  3681,
    3693,  3696,  3698,  3699,  3704,  3714,  3721,  3727,  3734,  3746,
    3756,  3762,  3763,  3764,  3771,  3776,  3781,  3786,  3791,  3800,
    3805,  3816,  3819,  3822,  3825,  3831,  3834,  3837,  3843,  3850,
    3853,  3864,  3868,  3872,  3877,  3882,  3887,  3892,  3898,  3904,
    3909,  3914,  3920,  3926,  3932,  3938,  3945,  3957,  3958,  3959,
    3960,  3961,  3962,  3963,  3964,  3965,  3966,  3967,  3968,  3969,
    3970,  3971,  3972,  3973,  3974,  3975,  3976,  3977,  3978,  3979,
    3980,  3981,  3982,  3988,  3988,  4072,  4073,  4077,  4078,  4079,
    4080,  4081,  4082,  4083,  4084,  4085,  4086,  4087,  4088,  4089,
    4090,  4091,  4092,  4093,  4097,  4098,  4111,  4111,  4130,  4131,
    4132,  4137,  4137,  4179,  4182,  4185,  4188,  4191,  4194,  4197,
    4199,  4202,  4209,  4214,  4230,  4231,  4230,  4240,  4241,  4244,
    4247,  4254,  4259,  4275,  4276,  4275,  4285,  4286,  4294,  4298,
    4306,  4308,  4306,  4317,  4318,  4321,  4326,  4327,  4326
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NAMESPACE", "FX_TRUE", "FX_FALSE",
  "FX_FLOAT", "FX_INT", "FX_BOOL", "FX_STR", "GLSLSHADER", "HLSL10SHADER",
  "HLSL11SHADER", "CGSHADER", "SAMPLER_STATE", "DST_STATE", "PR_STATE",
  "COLOR_SAMPLE_STATE", "RASTERIZATION_STATE", "TECHNIQUE", "PASS",
  "CODEBODY", "VARNAME", "TEXTURERESOURCE", "TEXTURERESOURCE1D",
  "TEXTURERESOURCE2D", "TEXTURERESOURCE3D", "TEXTURERESOURCERECT",
  "TEXTURERESOURCECUBE", "VOID_TYPE", "UNSIGNED", "HIGHP", "MEDIUMP",
  "LOWP", "UNIFORM", "CSTBUFFER", "FLOAT_TYPE", "FLOAT2_TYPE",
  "FLOAT3_TYPE", "FLOAT4_TYPE", "MAT2_TYPE", "MAT3_TYPE", "MAT4_TYPE",
  "BOOL_TYPE", "BOOL2_TYPE", "BOOL3_TYPE", "BOOL4_TYPE", "INT_TYPE",
  "INT2_TYPE", "INT3_TYPE", "INT4_TYPE", "TEXTURE1D_TYPE",
  "TEXTURE2D_TYPE", "TEXTURE2DRECT_TYPE", "TEXTURE3D_TYPE",
  "TEXTURECUBE_TYPE", "SAMPLER1D_TYPE", "SAMPLER2D_TYPE",
  "SAMPLER2DRECT_TYPE", "SAMPLER3D_TYPE", "SAMPLERCUBE_TYPE", "EXTENSION",
  "SEPARATE_SHADER", "DEPTHMASK", "DEPTHFUNC", "VERTEXPROGRAM",
  "FRAGMENTPROGRAM", "GEOMETRYPROGRAM", "HULLPROGRAM", "EVALPROGRAM",
  "SHDPROFILE", "SAMPLERRESOURCE", "SAMPLERTEXUNIT", "SETSAMPLERSTATE",
  "SETDSTSTATE", "SETRASTERIZATIONSTATE", "SETCOLORSAMPLESTATE",
  "IMAGERESOURCE", "IMAGEUNIT", "IMAGEACCESS", "IMAGELAYER",
  "IMAGELAYERED", "WRITE_ONLY", "READ_ONLY", "READ_WRITE",
  "VERTEXPROGRAMOVERRIDE", "FRAGMENTPROGRAMOVERRIDE",
  "GEOMETRYPROGRAMOVERRIDE", "HULLPROGRAMOVERRIDE", "EVALPROGRAMOVERRIDE",
  "IMAGEACCESSOVERRIDE", "IMAGELAYEROVERRIDE", "IMAGELAYEREDOVERRIDE",
  "IMAGERESOURCEOVERRIDE", "UNIFORMOVERRIDE", "IMAGEUNITOVERRIDE",
  "TEXTURERESOURCEOVERRIDE", "SAMPLERRESOURCEOVERRIDE",
  "SAMPLERTEXUNITOVERRIDE", "SETSAMPLERSTATEOVERRIDE", "GLACCUM", "GLLOAD",
  "GLRETURN", "GLMULT", "GLADD", "GLNEVER", "GLLESS", "GLEQUAL",
  "GLGREATER", "GLNOTEQUAL", "GLALWAYS", "GLLEQUAL", "GLGEQUAL", "GLKEEP",
  "GLREPLACE", "GLINCR", "GLDECR", "GLUPPERLEFT", "GLLOWERLEFT",
  "GLFRONTLEFT", "GLFRONTRIGHT", "GLBACKLEFT", "GLBACKRIGHT", "GLFRONT",
  "GLBACK", "GLLEFT", "GLRIGHT", "GLFRONTANDBACK", "GLCCW", "GLCW",
  "GLPOINT", "GLLINE", "GLFILL", "GLZERO", "GLONE", "GLSRCCOLOR",
  "GLONEMINUSSRCCOLOR", "GLSRCALPHA", "GLONEMINUSSRCALPHA", "GLDSTALPHA",
  "GLONEMINUSDSTALPHA", "GLDSTCOLOR", "GLONEMINUSDSTCOLOR",
  "GLSRCALPHASATURATE", "GLCONSTANTCOLOR", "GLONEMINUSCONSTANTCOLOR",
  "GLCONSTANTALPHA", "GLONEMINUSCONSTANTALPHA", "GLFUNCADD", "GLMIN",
  "GLMAX", "GLFUNCSUBTRACT", "GLFUNCREVERSESUBTRACT", "GLCLEAR", "GLSET",
  "GLCOPY", "GLCOPYINVERTED", "GLNOOP", "GLINVERT", "GLAND", "GLNAND",
  "GLOR", "GLNOR", "GLXOR", "GLEQUIV", "GLANDREVERSE", "GLANDINVERTED",
  "GLORREVERSE", "GLORINVERTED", "GLTEXTUREMAGFILTER",
  "GLTEXTUREMINFILTER", "GLTEXTUREWRAPS", "GLTEXTUREWRAPT",
  "GLTEXTUREWRAPR", "GLNEAREST", "GLLINEAR", "GLNEARESTMIPMAPNEAREST",
  "GLLINEARMIPMAPNEAREST", "GLNEARESTMIPMAPLINEAR", "GLLINEARMIPMAPLINEAR",
  "GLCLAMPTOEDGE", "GLCLAMP", "GLREPEAT", "GLMIRROREDREPEAT",
  "GLPOINTSIZE", "GLPOINTSMOOTH", "GLPOINTSPRITE", "GLPROGRAMPOINTSIZE",
  "GLPOINTSPRITECOORDORIGIN", "GLLINEWIDTH", "GLLINESMOOTH",
  "GLCOMBINEDLINESTIPPLE", "GLLINESTIPPLEPATTERN", "GLLINESTIPPLEREPEAT",
  "GLLINESTIPPLE", "GLCULLFACE", "GLCULLFACEMODE", "GLFRONTFACE",
  "GLPOLYGONSMOOTH", "GLPOLYGONMODE", "GLPOLYGONOFFSET",
  "GLPOLYGONOFFSETFACTOR", "GLPOLYGONOFFSETUNITS", "GLPOLYGONOFFSETPOINT",
  "GLPOLYGONOFFSETLINE", "GLPOLYGONOFFSETFILL", "GLPOLYGONSTIPPLEBIT",
  "GLRASTERIZERDISCARD", "GLALPHATEST", "GLALPHAFUNC", "GLALPHATESTFUNC",
  "GLALPHATESTREF", "GLBLENDFUNC", "GLBLENDFUNCSEPARATE", "GLBLEND",
  "GLBLENDSRC", "GLBLENDSRCRGB", "GLBLENDSRCALPHA", "GLBLENDDST",
  "GLBLENDDSTRGB", "GLBLENDDSTALPHA", "GLBLENDEQUATIONSEPARATE",
  "GLBLENDEQUATION", "GLBLENDEQUATIONRGB", "GLBLENDEQUATIONALPHA",
  "GLBLENDCOLOR", "GLFRAMEBUFFERSRGB", "GLDITHER", "GLCOLORLOGICOP",
  "GLLOGICOPMODE", "GLCOLORWRITEMASK", "GLMULTISAMPLE", "GLSAMPLEMASK",
  "GLSAMPLEMASKVALUE", "GLSAMPLEALPHATOONE", "GLSAMPLEALPHATOCOVERAGE",
  "GLDEPTHTEST", "GLDEPTHFUNC", "GLDEPTHWRITEMASK", "GLDEPTHCLAMP",
  "GLDEPTHBOUNDSTESTEXT", "GLDEPTHBOUNDSEXT", "GLSTENCILTEST",
  "GLSTENCILFUNC", "GLSTENCILFUNCSEPARATE", "GLSTENCILVALUEMASK",
  "GLSTENCILREF", "GLSTENCILFAIL", "GLSTENCILPASSDEPTHFAIL",
  "GLSTENCILPASSDEPTHPASS", "GLSTENCILOP", "GLSTENCILOPSEPARATE",
  "GLSTENCILBACKOP", "GLSTENCILWRITEMASK", "GLSTENCILBACKFUNC",
  "GLSTENCILBACKVALUEMASK", "GLSTENCILBACKREF", "GLSTENCILBACKFAIL",
  "GLSTENCILBACKPASSDEPTHFAIL", "GLSTENCILBACKPASSDEPTHPASS",
  "GLSTENCILBACKWRITEMASK", "ATTRIBUTE", "RENDERGROUP", "RENDERMODE",
  "RENDERMODE_ENUM", "BLITFBOTOACTIVETARGET", "CLEARMODE", "CLEAR_ENUM",
  "CURRENTTARGET", "CLEARCOLOR", "VIEWPORT", "FBO", "RENDERBUFFER",
  "RENDERTEXTURE", "RENDERTEXTURERECT", "DSTTARGET", "COLORTARGETS",
  "RESOURCEMSAA", "RESOURCESIZE", "RESOURCESIZEFACTOR",
  "RESOURCERESOLVETARGET", "RESOURCEFORMAT", "RESOURCEFORMAT_ENUM",
  "APPDEFINED", "SWAPRESOURCES", "LOOP", "ACTIVE", "OFF", "GLOBAL",
  "LOCAL", "CUDAKERNEL", "CUDACODE", "CUDAMODULE", "CUDASHAREDMEMORY",
  "CUDAGRID", "CUDABLOCK", "OPTIX_CONTEXT", "OPTIX_CONTEXT_ENTRY",
  "RAY_GENERATION_PROGRAM", "EXCEPTION_PROGRAM", "INTERSECTION_PROGRAM",
  "BOUNDING_BOX_PROGRAM", "MISS_PROGRAM", "DEFAULT_MATERIAL", "STACK_SIZE",
  "OUTPUT_BUFFER", "OPTIX_MATERIAL", "CLOSEST_HIT_PROGRAM",
  "ANY_HIT_PROGRAM", "OPTIX_BUFFER", "PTX_PROGRAM_FROM_FILE", "FILE_NAME",
  "ENTRY_POINT", "COMPUTEGROUPS", "COMPUTEPROGRAM",
  "COMPUTEPROGRAMOVERRIDE", "SVGPATH", "PSPATH", "PATHSOURCEOVERRIDE",
  "PATHSOURCE", "GLPATHSTENCILDEPTHOFFSET", "GLPATHSTENCILFUNC",
  "GLPATHFOGGEN", "GLPATHTEXGEN", "GLPATHCOLORGEN", "GLPATHCOVERDEPTHFUNC",
  "GLPATHSTROKEWIDTH", "GLPATHINITIALENDCAP", "GLPATHTERMINALENDCAP",
  "GLPATHINITIALDASHCAP", "GLPATHTERMINALDASHCAP", "GLPATHJOINSTYLE",
  "GLPATHMITERLIMIT", "GLPATHDASHOFFSET", "GLPATHDASHOFFSETRESET",
  "GLPATHCLIENTLENGTH", "GLPATHFILLMODE", "GLPATHFILLMASK",
  "GLPATHFILLCOVERMODE", "GLPATHSTROKEMASK", "GLPATHSTROKEREFERENCE",
  "GLPATHSTROKECOVERMODE", "GLPATHTRANSFORMTYPE", "GLPATHTRANSFORM",
  "GLFOGCOORDINATE", "GLFRAGMENTDEPTH", "GLOBJECTLINEAR",
  "GLPATHOBJECTBOUNDINGBOX", "GLEYELINEAR", "GLFLAT", "GLSQUARE",
  "GLROUND", "GLTRIANGULAR", "GLMITERREVERT", "GLMITERTRUNCATE", "GLBEVEL",
  "GLMOVETORESET", "GLMOVETOCONTINUES", "GLCOUNTUP", "GLCOUNTDOWN",
  "GLBOUNDINGBOX", "GLCONVEXHULL", "GLTRANSLATE2D", "GLTRANSLATEX",
  "GLTRANSLATEY", "GLTRANSLATE3D", "GLAFFINE2D", "GLAFFINE3D",
  "GLTRANSPOSEAFFINE2D", "GLTRANSPOSEAFFINE3D", "GLLUMINANCE", "GLALPHA",
  "GLINTENSITY", "GLLUMINANCEALPHA", "GLRGB", "GLRGBA", "'='", "'-'",
  "'+'", "'*'", "'/'", "NEG", "'^'", "';'", "':'", "'('", "')'", "','",
  "'{'", "'}'", "'|'", "'<'", "'>'", "'['", "']'", "'.'", "$accept",
  "input", "extension", "name", "fexp", "iexp", "bexp", "asbexp", "asiexp",
  "asfexp", "separator", "shaderlist", "one_or_more_shaders",
  "varnamelist", "liststart", "listend", "fexplist", "fexpveclist",
  "iexplist", "iexpveclist", "bexplist", "argval", "argvallist", "glenum",
  "clear_flags", "passstates", "pass", "$@1", "$@2", "passes", "tech",
  "$@3", "$@4", "global", "global_true", "nsvarname",
  "global_varname_annots", "glsl", "svgpath", "pspath", "hlsl11", "hlsl10",
  "unsigned", "pointer", "argument", "arglist", "cudak", "cudac",
  "texparams", "resourcetype", "texture", "$@5", "resourceparams",
  "resource", "$@6", "$@7", "$@8", "$@9", "$@10", "$@11", "fboparams",
  "fbo", "$@12", "$@13", "sfilter", "suvmapping", "sstates", "sstate",
  "$@14", "passglsstates", "rstates", "rstate", "$@15", "cstates",
  "csstate", "$@16", "dststates", "dststate", "$@17", "passprstates",
  "prstates", "prstate", "$@18", "annotation", "annotations2",
  "annotations", "valueassignment_f", "valueassignment_fv",
  "valueassignment_fvl", "valueassignment_i", "valueassignment_b",
  "valueassignment_bv", "precision", "uniform_qualifiers",
  "uniformvalueassigment", "semantic", "arraySz",
  "arraysz_semantic_annotations_uniformvalueassigment", "uniform_type",
  "uniform", "$@19", "uniforms", "namespaceitem", "namespaceitems",
  "namespace", "$@20", "cstbufferuniforms", "cstbuffer", "$@21",
  "optix_context_data", "optix_context", "$@22", "$@23",
  "optix_material_data", "optix_material", "$@24", "$@25",
  "optix_buffer_data", "optix_buffer", "$@26", "$@27",
  "ptx_program_from_file_data", "ptx_program_from_file", "$@28", "$@29", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   100,   101,   318,   319,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   374,   375,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,   392,
     393,   394,   395,   396,   397,   398,   399,   400,   401,   402,
     403,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   417,   418,   419,   420,   421,   422,
     423,   424,   425,   426,   427,   428,   429,   430,   431,   432,
     433,   434,   435,   436,   437,   438,   439,   440,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
     473,   474,   475,   476,   477,   478,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   497,   498,   499,   500,   501,   502,
     503,   504,   505,   506,   507,   508,   509,   510,   511,   512,
     513,   514,   515,   516,   517,   518,   519,   520,   521,   522,
     523,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,   539,   540,   541,   542,
     543,   544,   545,   546,   547,   548,   549,   550,   551,   552,
     553,   554,   555,   556,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,   570,   571,   572,
     573,   574,   575,   576,   577,   578,   579,   580,   581,   582,
     583,   584,   585,   586,   587,   588,   589,   590,   591,   592,
     593,   594,   595,   596,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,   628,    61,    45,    43,    42,
      47,   629,    94,    59,    58,    40,    41,    44,   123,   125,
     124,    60,    62,    91,    93,    46
};
# endif

#define YYPACT_NINF -1824

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-1824)))

#define YYTABLE_NINF -701

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1824,  2239, -1824, -1824,     0,    -4,    -4,    -4,    27,    27,
      27,    27,    27,    87, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824,   313,    -4,   -27,    27,    27,    27,    27,
   -1824, -1824,    -4,    -4,    -4,    -4,   142,    -4,    -4,    -4,
   -1824, -1824, -1824,   463, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824,   159, -1824, -1824, -1824,  2660,  3092,  3787,  3841,  4161,
   -1824,  2070, -1824,   309, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824,    27,  -187,  -308,  -119,  -114, -1824, -1824, -1824,
   -1824, -1824, -1824,  -187,   463, -1824, -1824,   -96,  -143,  -143,
    -143,  -143,   -94,   -84, -1824, -1824, -1824, -1824,   -67,   -57,
   -1824,   -50, -1824, -1824, -1824, -1824,   -17,    -6, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824,   317, -1824,  -187,  -187, -1824, -1824,
   -1824,   368,   370,   375,  -187,  -187,  -187,  -187,  -187,  -187,
      61, -1824,    72,   238, -1824, -1824, -1824, -1824, -1824, -1824,
     318,   492,  -187,  -187,  -187,  -187,   515,   538, -1824,  -187,
   -1824, -1824, -1824,   178, -1824,     7, -1824, -1824, -1824,   202,
     223,   234,   244,   245,   248,     6,   215, -1824, -1824, -1824,
   -1824,  -187,  -187,  -187,  -187, -1824, -1824,   224,   562, -1824,
     200, -1824, -1824, -1824, -1824, -1824, -1824, -1824,   249,     4,
    3155, -1824,   250, -1824, -1824, -1824, -1824,   326, -1824,     6,
     622,   624,   263, -1824,    12, -1824,   221, -1824, -1824, -1824,
   -1824, -1824,   642,   224,   277,   175,   286,   287,   301,   315,
   -1824, -1824,   134,   679,    67,    55, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824,    38,     9,   335, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824,   291,  1038,  2069,  2134,
    2201,   309, -1824,  2691, -1824,   323,    37,   587,  1057,   340,
     344,     5,  1028,    33,  -260, -1824,  -187,    92, -1824, -1824,
   -1824, -1824,   321,   336,   345,   358, -1824,   710,   724, -1824,
     452, -1824, -1824, -1824,    13, -1824, -1824, -1824,    88,    88,
   -1824,   573,   608, -1824,   147, -1824,    95,    95, -1824,   -20,
   -1824,    70, -1824,   106, -1824,    44, -1824, -1824, -1824,   369,
     573,   357,   372,   376,   378,   389,   390,   396,   398,   400,
   -1824,   397,   401,   405,   416,   417,   433,   439,   443,   444,
     445,   446,   461,   462,   475,   478,   482,   483,   485,   488,
     501,   503,   505, -1824,   428,   521,   525,   541,   438,   498,
     542,   543,   545,   548,   549,   550,   554,   556,   557,   558,
     563,   564,   565,   566,   567,   578,   580, -1824,   517,  4631,
     574,   582,   585,   590,   591,   592,   594,   595,   598,   599,
     601,   602,   603,   607,   619,   621,   623,   625,   626,   627,
     628,   629,   630, -1824,   615,   631,   632,   634,   636,   637,
     638,   640,   641,   645,   646,   649,   650,   651,   654,   655,
     656,   657,   658,   661,   662,   665, -1824, -1824,   617, -1824,
    -187,   540,   659,   660, -1824, -1824, -1824, -1824, -1824, -1824,
     666,   394,   207,  -223,  -195, -1824,   667, -1824,   669,   675,
     261,   333,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,   573,   608, -1824,    88,    29,    50,  -121, -1824,
      -8,    95,    95,    95, -1824, -1824, -1824,   117, -1824, -1824,
   -1824, -1824, -1824, -1824,   552,   552,   130,   130,   130, -1824,
     153,  4631,   153,   153,   153,   517,   153,  4631,   153,   153,
    4631,  4631,  4631,   153,  4631,   153,   153,  4631,  4631,  4631,
     153, -1824,   671,  4631,  4631,   153,   153,  4631,   153,  4631,
    4631,  4631,  4631,  4631,   153,   153,  4631,   153,  4631,   153,
    4631,  4631,  4631,  4631,  4631, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824,   664, -1824,   153,  4631,
     153,   153,  4631,  4631,  4631,  4631,  4631,  4631,  4631,  4631,
     517,   153,   153,   153,  4631,   517,   153,   153,   153,   153,
   -1824,   153,   153,   153,   153,  4631,   153,   153,   153,   153,
     153,   153,  4631,  4631,   153,  4204,   153,   153,   153,   153,
     153,   153, -1824,   680, -1824,   668,   691,    75,   265,   527,
     593,   517,   635,   698,   699,   705,   706,   707,   708,   711,
   -1824,   643,   712,   713, -1824,   715,   722, -1824,   723,   727,
   -1824,  1078, -1824, -1824,   237,   273,   237,   273,   669,   675,
     669,   675,   669,   675,   237,   273,   237,   273,   669,   675,
     669,   675,   669,   675,    29, -1824,    88, -1824, -1824, -1824,
     517, -1824,   238, -1824,   728,   728, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824,   729,   730, -1824, -1824, -1824,
   -1824, -1824,   731,   732,   733,   573,   608, -1824,   735,   737,
     738,   739,   741,   153,   742,   743,   573,   608, -1824,   744,
     745,   749,   751,   753,   756,   763,   764,   766,   768,   771,
     772,   773,   153,   775,   777,   717,   767,   779,   608, -1824,
     780,   781,   783,   784,   786,   787,   788,   789,   793,   794,
     795,   801,   808,   810,   811,   812,   813, -1824,   819,   821,
     823,   824,   825,   826,   828,   845,   851,   853,   854,   858,
     153,   859,   860,   861,   863,   153,   866,   867,   868,   870,
     872,   873,   874,   875,   876,   878,   879,   881,   882,   883,
     885,   886,   887,   889,  4631,   890,   891,   892,   893,   894,
     895,   896, -1824,  3419,   153,   153,   904,   905,   906, -1824,
     907,   910,   912,   913,   914,   915,   916, -1824, -1824, -1824,
    1273,  1275,   517,  1277,  1278,  1280,  1281,    95,  1282,  1283,
     517,   517,  -233,  1024,  1298,  1299,   933, -1824,   573,   608,
      88, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824,   930, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,   931, -1824,
   -1824,   943,   944, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
    4631, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824,   930, -1824, -1824, -1824, -1824,   930, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824,   930, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824,  3696,   938,   937,   939,  -348,
    -303,  -291,  -266,  -193,   947,   940,   945,   950,  -213,  -209,
    -183,   951,   952,  -111,   -56,   -42,   -39,   -23,   954,   956,
     957,   958,   959,   960,   970,   972,   973,   974,   975,   976,
     980,   981,   982,   983,   984,   985,   986,   987,   988,   989,
     990,   993,   994,   996,   998,   999,  1000,  1023,  1025,  1026,
    1027,  1029,  1030,  1031,  1032,  1034,  1035,  1036,  1037,  1039,
    1040,  1042,  1044,  1045,  1046,  1047,  1048,  1049,  1050,  1052,
    1053,  1054,  1055,  1056,  1058,  1060,  1061,  1062,  1063,  1064,
    1065,  1067,  1071,  1073,  1074,  1076,  1077,  1079,  1081,  1083,
    1084,  1085,  1086,  1087,  1088,  1089,  1090,  1015,  1091,  1094,
    1098,  1099,  1101,  -311,  1103,  1095,  1105,  1106,  1107,  1108,
    1110,  1111,  1112,  1113,  1115,  1117,   108,   136,   161,   164,
    1118,  1119,  1120,  1021,  1051,  1121,  1122,  1124,  1125,  1126,
    1127,  1128,  1129,  1130,  1131,  1132,  1133,  1137,  1138,  1140,
    1141,  1143, -1824, -1824, -1824,   949,  1148,  1426,  1498,    -5,
    1523,  1511,   183,  -220,   153,  1513,  1256, -1824,    99,  1153,
    1273,  1158,  1159,  1160,  1162,   284,  1163,  1161,  1273,  1273,
    1165,    95,  1169,  1171,  1172, -1824,    29, -1824,   153,   153,
    1170,  1173,  1168,   153,   153,  4631, -1824, -1824,  1537,  1538,
      15,   112,    15,   112,    15,   112,    15,   112,    15,   112,
    1553,  1541,  1542,  1543,  1544,  1547,  1548,  1549,  1551,  1554,
    1555,  1556,    15,   112,    15,   112,    15,   112,    15,   112,
      15,   112,  1558,  1559,  1560,  1561,  1564,  1566,   153,   153,
     153,   153,  4631,   153,   153,   153,   153,   153,  4135,  4631,
    4631,   153,   517,   517,   153,   153,   153,   153,   153,   153,
     153,   517,  4631,   153,  1201,  1202,   153,  4631,  4631,  4631,
    4631,  4631,  4631,   517,  4631,  4631,  4631,   517,   153,   153,
     153,  4631,   517,   153,   153,   153,   153,   153,  4631,   153,
     153,   153,   517,   153,  4493,  1203,   153,   153,  4631,  4631,
    4631,  1204,  1207,   153,  4562,   153,   153,  4631,  4631,  4631,
     153,   112,   153,  1333,  1577,  1334,  1578,   517,   153,   517,
    1580,   153,   153,  1215,    15,    95,   517,   517,  1582,    95,
     517,    15,   112,    15,   112,    15,   112,    15,   112,  1217,
    4631,  4631,   153,   153,  4631,   153,  4631,  4631,  4631,  4631,
    4631,   153,   153,  4631,   153,  4631,   153,  4631,  4631,  4631,
    4631, -1824, -1824,  1223,  1226,  1227,  1273,  1228,  1229,  1230,
    1231,    95,  1232,    95,  1233,  1234,  1237,  1599,  1240,     3,
      99, -1824, -1824, -1824, -1824, -1824, -1824,    22,    99,    99,
   -1824,   199,   167, -1824, -1824, -1824, -1824,  1168,  1235,  4631,
    4631,  1242,   930,   930,  1168,  1241,   219, -1824, -1824,  -169,
    1243,   114, -1824, -1824,  1236,  1246,  1238,  1248,  1244,  1249,
    1247,  1250,  1251,  1252,  1254,  1258,  1259,  1255,  1261,  1265,
    1266,  1268,  1267,  1270,  1271,  1276,  1262,  1279,  1269,  1284,
    1274,  1285,  1286,  1288,  1287,  1272,  1289,  1294,  1295,  1300,
    1301,  1290,  1291,  1302,  1305,  1306,  1310,  1311,  1313,  1316,
    1317,  1318,  1321,  1322,  1323,  1324,  4631,   153,  1325,  1327,
    1328,  1329,  1330,  1331,  1335,  4631,  1336,  1337,  4631,  4631,
    1338,  1339,  1340,  1341,  1342,  1343,  1344,  4631,  1345,  1348,
    1356,   153,  1364,  1365,  1366,  1368,   153,  1369,  1370,  1371,
    1372,  1373,  1374,  1375,  1376,  1377,   153,  1378,  4631,  1379,
    4631,  1380,  1381,  1382,  1384,  1385,  4631,  4631,  1386,  4631,
    1387,  1388,  1390,  1391,  1392,  1393,  1394,  1395,  1397,  1400,
    1401, -1824,  -198,  1404,    88,  1402,    95,  1407,  1405,  1406,
    1612,  1408,   385,    95,    32,    47,  1412,   408,    95,  1416,
    1409,  1417,  1415,  1425,  1418,  1428,  1421,   153,  1431,  1432,
    1422,  1423,  1435,  1436,  1437,  1438,  1439,  1440,  1442,  1443,
    1444,  1446,  1447,  1448,  1449,  1450,  1451,  1452,  1453, -1824,
   -1824, -1824,    99, -1824, -1824, -1824, -1824,   167, -1824,   167,
   -1824, -1824, -1824, -1824, -1824,    86,  1454,  1455,   143,  1456,
    1457,  1458,    95,  1460,  1461,  1463,  1464,  1466, -1824,   153,
     153,  1467,  1293,    56,    95,   613,  1620, -1824,    99,  1296,
   -1824,  1433, -1824,  1471, -1824,  1478, -1824,  1479, -1824,  1481,
    1482,  1483, -1824,  1477, -1824,  1480, -1824,  1484,  1486,  1488,
   -1824,  1489, -1824,  1490, -1824,  1492, -1824,  1493, -1824,  1494,
    1495,  1496,  1497,  1499,  1500,  1502, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824,   930,  1487, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
    1501, -1824, -1824,  1503,  1504, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824,  1507, -1824, -1824, -1824,   930, -1824, -1824, -1824,
   -1824,   930, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824,   930, -1824,  1508, -1824,  1509, -1824, -1824, -1824, -1824,
   -1824,  1514,  1515, -1824,  1516, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824,  1505, -1824, -1824, -1824, -1824,  1396, -1824,    29,
    1510,   167,  1638, -1824, -1824,  1517, -1824, -1824,   167, -1824,
   -1824,   517, -1824, -1824, -1824,   167, -1824,  1529, -1824,  1531,
   -1824,  1534, -1824,  1535,  1526, -1824, -1824,  1536,  1539, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824,  1506,  1533,  1540,  1545,
     257, -1824, -1824,  1546,   257, -1824, -1824, -1824,   199, -1824,
   -1824, -1824,   153,  4631,   930,   930, -1824,  1643,   173,  1552,
     195,  1528,  1532,  1550,  1562,  1563, -1824, -1824,    15,    15,
      15,    15,    15,     2,    95,  1694, -1824, -1824, -1824,    45,
      95,    15,    15,    15,    15,    15,  1756,    95,  1857,    81,
      95,  1858,  4631,   153,   153,  4631,  4631,  4631,   153,   153,
     153,   153,  4631,  4631,  4631,   153,   153, -1824,  1567,   517,
    1568,  1569,    98, -1824,    95,  1570,    15,    15,    15,    15,
     153,  1571,  1572, -1824, -1824, -1824, -1824,    99, -1824,    99,
    1574,  1575,   153,   153,  1573,  1581,   257, -1824,  1579,  1565,
    1587,  1590,  1591,  1592,  1588,  1589,  1595,  1596,  1597,  1598,
    1862,   411,  1600,  1601,  1863,   419,  1602,  1603,  1604,  1605,
    1607,  1608,   435,  1609,  1610,  1865,   507,  1611,  1168,  1168,
    1168,  1584,  1583,  1168,   930,   930,  1168,  1613,  1614,  1615,
    1617,  1618,  1616, -1824,    58, -1824,  1623, -1824, -1824, -1824,
     573,   608, -1824,    53,   167, -1824,  1624,  1626,  1627,  1628,
    1585,  4631,  4631,  1629,  1630,   153,   153,  1168,  1168, -1824,
   -1824,    99,  1606,  1898,    95,  1899,  1902,    95, -1824, -1824,
   -1824, -1824, -1824, -1824,  1631, -1824, -1824, -1824,  1632, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,  1633,
   -1824, -1824,  1639,  1640,  1641,  1642,  4631,  1644,   153,   153,
    1645,   153,   153,  4631,  4631,   153, -1824, -1824, -1824, -1824,
     190,  1646, -1824, -1824, -1824, -1824, -1824,  1647,  1634,  1651,
   -1824, -1824,  1656,  1658,  1648,  1650,  1667,   102,  1668,   529,
    1670,  1676,   532,    95,    95,    95, -1824, -1824, -1824, -1824,
    1673, -1824,   930,   930, -1824,  1675,  1685,  1684,  1687,  1686,
   -1824, -1824, -1824,   153,  4631,  1679,  1693, -1824, -1824, -1824,
    1695,    71,    60, -1824, -1824, -1824, -1824, -1824,   128,   206,
     246,  4631,   153,   153,  1699,   153,  1705,  4631,  1706,  1690,
    1703, -1824, -1824, -1824,    50,  -121, -1824,  1708,  1715,  1716,
    1742,  1168,  1168, -1824,  1743, -1824,  1744, -1824,   153,   153,
    1722,  1751, -1824, -1824, -1824,  1753,  1754,  1755,  1757,  1758,
    1750,  1764, -1824, -1824, -1824, -1824, -1824, -1824, -1824,  1759,
    1760, -1824, -1824
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     1,    28,     0,   316,   316,   316,     0,     0,
       0,     0,     0,   306,   339,   340,   341,   342,   343,   344,
     621,   622,   623,   624,   316,     0,     0,     0,     0,     0,
     311,   310,   316,   316,   316,   316,     0,   316,   316,   316,
       3,     4,     5,   628,     6,    22,    23,     8,     7,    10,
       9,     0,    11,    12,    13,     0,     0,     0,     0,     0,
     629,     0,   685,    21,    19,    20,    24,    25,    26,    27,
     706,   315,     0,   612,     0,     0,     0,   395,   553,   602,
     527,   498,   308,   612,   625,   626,   711,     0,   312,   312,
     312,   312,     0,     0,   724,   733,   740,   746,     0,     0,
     630,   315,   346,   397,   500,   529,     0,     0,   682,   657,
     658,   659,   660,   669,   670,   671,   665,   666,   667,   668,
     661,   662,   663,   664,   672,   673,   674,   675,   676,   677,
     678,   679,   680,   681,     0,   686,   612,   612,   605,   613,
     317,     0,     0,     0,   612,   612,   612,   612,   612,   612,
       0,   627,   708,     0,   314,   313,   373,   365,   359,   362,
     330,     0,   612,   612,   612,   612,     0,     0,   345,   612,
     555,   604,   683,     0,   318,     0,   319,   323,   322,     0,
       0,     0,     0,     0,     0,     0,     0,   712,    58,    59,
      29,   612,   612,   612,   612,   331,   325,   326,     0,   332,
       0,   335,   725,   734,   741,   747,   320,   321,     0,     0,
       0,   610,     0,   611,   388,   530,   578,   501,   474,     0,
       0,   295,     0,   303,     0,   709,     0,   374,   366,   360,
     363,   327,     0,   326,     0,   324,     0,     0,     0,     0,
     336,   637,     0,     0,     0,     0,   631,   632,   633,   634,
     635,   636,   643,     0,     0,     0,   687,   702,   703,   690,
     689,   692,   691,   693,   694,   695,     0,     0,     0,     0,
       0,   688,   704,     0,   701,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   297,   612,     0,   305,   307,
     304,   710,     0,     0,     0,     0,   328,     0,     0,   333,
       0,   727,   736,   743,     0,   618,    32,    52,     0,     0,
      80,   614,   617,   619,     0,   638,     0,     0,   639,     0,
     644,     0,   647,     0,   651,     0,   684,   707,   705,     0,
      66,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     396,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   554,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   603,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   528,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   499,   309,     0,   301,
     612,     0,     0,     0,   368,   348,   348,   348,   329,   334,
       0,     0,     0,     0,     0,   338,     0,   347,    46,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,    83,    95,     0,     0,     0,     0,    56,
       0,     0,     0,     0,   640,   648,   652,     0,   655,   606,
     608,   609,   607,   394,     0,     0,     0,     0,     0,   552,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   601,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     175,   176,   177,   178,   179,   180,     0,   526,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     497,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   302,     0,   209,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     726,     0,     0,     0,   735,     0,     0,   742,     0,     0,
     748,     0,    51,    57,    36,    38,    33,    35,    39,    41,
      42,    44,    47,    49,    37,    54,    34,    53,    40,    55,
      43,    45,    48,    50,     0,    86,     0,    81,   615,    89,
       0,   616,     0,   620,    54,    53,    55,   656,   382,   376,
     377,   378,   379,   380,   381,     0,     0,   387,   386,   384,
     383,   385,     0,     0,     0,    62,    61,    60,     0,     0,
       0,     0,     0,     0,     0,     0,    64,    63,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,    68,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   514,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   209,     0,     0,     0,     0,     0,     0,   375,
       0,     0,     0,     0,     0,     0,     0,   367,   361,   364,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    84,    85,
       0,    96,   390,   389,   391,   392,   393,   531,   532,   533,
     534,   535,    69,   537,   538,   539,   540,   541,   542,   543,
     544,   545,   546,   547,   548,   549,   550,   551,     0,   580,
     581,     0,     0,   584,   585,   586,   587,   588,   589,   590,
     591,   592,   593,   594,   595,   596,   597,   599,   600,   598,
       0,   502,   503,   504,   505,   506,   507,   508,   509,   510,
     511,   512,   513,    69,   517,   518,   519,   520,    69,   522,
     523,   524,   525,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,    69,   489,   491,
     492,   493,   494,   495,   496,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   296,   210,   211,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    78,    69,     0,
      77,     0,     0,     0,     0,     0,     0,     0,    77,    77,
       0,     0,     0,     0,     0,   337,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,   298,   294,     0,     0,
      74,     0,    74,     0,    74,     0,    74,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,    74,     0,    74,     0,    74,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,     0,     0,     0,     0,
       0,    74,     0,    74,     0,    74,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   300,   299,     0,     0,     0,    77,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      69,   715,   716,   717,   718,   719,   720,     0,    69,    69,
     738,    90,     0,   737,   744,   745,    88,     0,     0,     0,
       0,     0,    69,    69,     0,     0,     0,    71,    72,    69,
       0,     0,    30,    31,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   207,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   369,
     370,   372,    69,   350,   349,   351,   353,     0,   354,     0,
     356,   357,   358,    79,   713,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   515,     0,
       0,     0,     0,     0,     0,     0,     0,   214,    69,     0,
     216,     0,   222,     0,   224,     0,   226,     0,   249,     0,
       0,     0,   270,     0,   272,     0,   274,     0,     0,     0,
     228,     0,   230,     0,   236,     0,   238,     0,   240,     0,
       0,     0,     0,     0,     0,     0,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,    69,     0,   414,   415,   417,   418,   419,   420,   421,
       0,   422,   423,     0,     0,   425,   428,   429,   430,   431,
     432,   433,     0,   436,   434,   435,    69,   439,   440,   441,
     442,    69,   444,   445,   446,   447,   448,   449,   450,   451,
     452,    69,   454,     0,   455,     0,   458,   459,   461,   462,
     463,     0,     0,   460,     0,   466,   468,   469,   470,   471,
     472,   473,     0,   277,   276,   278,   279,     0,   280,     0,
       0,     0,     0,   285,   286,     0,   288,   290,     0,    94,
     291,     0,   292,   212,   213,     0,   218,     0,   232,     0,
     234,     0,   220,     0,     0,   557,   558,     0,     0,   561,
     562,   563,   564,   565,   566,   567,   568,   569,   570,   571,
     572,   573,   574,   576,   577,   575,     0,     0,     0,     0,
      77,   723,   714,     0,    77,   732,   729,   728,    91,   739,
     536,   579,     0,     0,    69,    69,   490,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    73,    76,    74,    74,
      74,    74,    74,     0,     0,     0,   271,   273,   275,     0,
       0,    74,    74,    74,    74,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   208,     0,     0,
       0,     0,     0,    92,     0,     0,    74,    74,    74,    74,
       0,     0,     0,   371,   352,   355,   722,    69,   731,    69,
       0,     0,     0,     0,     0,     0,    77,   244,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    69,    69,     0,     0,     0,     0,
       0,     0,     0,   282,     0,   283,     0,   100,    99,   102,
      98,    97,   103,     0,     0,   287,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   250,
     243,    69,     0,     0,     0,     0,     0,     0,   215,   217,
     223,   225,   227,   252,     0,   261,   267,   257,     0,   263,
     229,   231,   237,   239,   241,   258,   264,   251,   253,     0,
     262,   268,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   293,   281,   284,   105,
       0,     0,    93,   219,   233,   235,   221,     0,     0,     0,
     721,   730,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   413,   416,   424,   426,
       0,   437,    69,    69,   453,     0,     0,     0,     0,     0,
     104,   289,   556,     0,     0,     0,     0,   516,   521,   242,
       0,     0,     0,   254,   265,   269,   259,   266,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   582,   583,   245,     0,     0,   247,     0,     0,     0,
       0,     0,     0,   456,     0,   464,     0,   467,     0,     0,
       0,     0,   255,   260,   256,     0,     0,     0,     0,     0,
       0,     0,   246,   248,   427,   438,   443,   457,   465,     0,
       0,   559,   560
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1824, -1824, -1824,  -456,  1059,   -58,  -153,  -391,  -383,  1206,
    -901,   514, -1162, -1163,  -144,  -408,  -474, -1823, -1172, -1325,
    -135,   -83, -1824,  1525, -1824,  1066,  -177, -1824, -1824,  1711,
   -1824, -1824, -1824,   528,   652,  1068,   644,  1930,  1931,  1936,
    1938,  1941, -1824,  1721,  1723, -1824,  1956,  1995, -1824, -1824,
    1996, -1824,   123,  1997, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824,  2014, -1824, -1824,  1521,    96, -1824,  2033, -1824, -1824,
   -1824,  2146, -1824, -1824,  2153, -1824, -1824,  2154, -1824, -1824,
   -1824,  2155, -1824, -1824,  -125,   800, -1824, -1824, -1824, -1824,
   -1824, -1824,   455, -1824,  -242,  1909, -1824, -1824, -1824,   -44,
   -1824,    40,  1891, -1824, -1824, -1824, -1824,  2169, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    41,  1414,   775,   776,   777,   778,   789,   333,
    1377,  1409,  1410,  1168,  1411,   748,   476,   477,  1748,  1544,
     478,  1942,  1943,   646,  1532,   873,   223,   286,   440,   224,
      42,    83,   149,    72,   156,    73,    74,   256,   257,   258,
     259,   260,   198,   232,   199,   200,   261,   262,   304,    51,
     263,   169,   698,   264,   193,   294,   194,   295,   192,   293,
     697,   265,   191,   292,   765,   772,   276,   266,   144,  1153,
     282,   267,   148,   281,   268,   147,   277,   269,   145,  1154,
     278,   270,   146,   175,   139,   140,   246,   247,   248,   249,
     250,   251,    60,    61,   252,   253,   254,   255,   134,    62,
     209,   271,   272,   273,    64,   136,   187,   274,   152,   451,
      66,   162,   236,   452,    67,   163,   237,   453,    68,   164,
     238,   454,    69,   165,   239
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     190,   744,  1545,   320,   241,   241,   390,  1380,   211,  1392,
     241,   322,   324,   288,   455,  1388,  1389,  1365,    71,   135,
    1407,  1188,    70,   241,  1899,   220,   221,   438,  1200,   212,
     745,   220,   221,  1749,   288,    87,  1934,  1408,   334,   241,
    1415,    63,  1417,  1201,  1419,   241,  1421,   290,  1749,    71,
    1180,   749,   220,   221,  2009,   715,   241,   241,   716,   749,
    1435,  1749,  1437,  1372,  1439,  1317,  1441,  1903,  1443,   751,
     753,   241,  1193,  1202,   307,   188,   189,  1194,   307,   485,
     141,   486,  1318,   488,   245,  1204,   456,   305,  1203,   313,
     188,   189,   306,   307,   306,   307,  1195,   876,   314,  1937,
    1205,   442,   307,  1914,   306,   307,   290,   241,  1789,    82,
    1206,   780,   781,   782,   443,   784,   718,   719,   241,  1407,
    1938,  1412,   332,   439,  2060,  1207,   790,  1939,   321,   323,
     794,   767,   796,   797,  1413,   305,  1408,   801,   188,   189,
     306,   307,   805,   806,   305,   154,   155,   188,   189,   306,
     307,   188,   189,   306,   307,   310,   821,   188,   189,   306,
     307,   474,  1541,  1214,    96,  1793,   717,  1216,   310,  1549,
     475,  1551,  1215,  1553,   305,  1555,  1217,   188,   189,   306,
     307,   101,   135,  1208,   312,  1736,   319,   188,   189,   306,
     307,  1937,  1737,  1218,   720,  1885,   306,   307,  1209,  1587,
     487,  1589,  1219,  1582,   138,   196,   335,   336,   337,   338,
     339,   197,  1938,   391,   -75,   392,   393,   331,  1187,   394,
     395,   396,   397,   398,   399,   400,   226,   135,   401,   402,
     403,   404,   405,   406,   407,   408,   409,   410,  2084,   411,
     412,   711,   188,   189,   544,   757,    20,    21,    22,    23,
     459,   461,    20,    21,    22,    23,   473,   828,   479,   480,
     831,   188,   189,   306,   307,  1222,   752,   222,   747,   142,
     841,   842,   843,   222,   143,   846,   847,   848,   849,  1167,
    1223,   851,   852,   853,    30,    31,   856,   880,   153,   859,
     860,   160,   103,   863,   222,   857,   858,   868,   869,   870,
     871,  -696,  -696,  -696,   161,  -696,  -696,  -696,  -696,  -696,
     768,   769,   770,   771,  -696,  -696,  -696,  -696,  -696,  -696,
    1224,   166,  -696,  -696,  -696,  -696,  -696,   188,   189,   306,
     307,   167,   329,   168,  1226,  1225,   907,  1228,   881,   172,
      20,    21,    22,    23,    20,    21,    22,   195,   196,  1227,
     877,   878,  1229,  1230,   197,   788,   788,   481,   482,   483,
     788,   783,   788,   788,  1741,  -324,   170,   788,  1231,   481,
     482,   483,   788,   788,   484,   809,  1755,   171,   723,  1595,
     242,   809,   809,   310,   809,   242,   788,  -641,   243,   176,
    1900,   177,  -649,   243,   413,   138,   178,   244,  1598,   213,
     138,   289,   457,   310,   725,   727,   729,   731,   733,   735,
     737,   739,   741,   743,   242,  1750,   746,   473,   747,  1751,
     242,  -645,   437,   754,   755,   756,   340,  -653,   702,   138,
    1752,   242,  1808,  1904,  1751,   138,  1186,   750,  -642,   747,
    2010,  2007,  2011,  2086,   316,   750,   242,  1751,   316,   185,
     787,   787,   317,  -646,   978,   787,   317,   787,   787,   310,
     186,   318,   787,   308,   879,   308,  1369,   787,   787,  1915,
     808,   309,   316,   309,   310,   308,   808,   808,    85,   808,
     317,   787,   242,   309,  1331,  -101,  1187,  -101,   747,  -650,
     310,  1155,  1156,   242,    20,    21,    22,   809,   100,  1332,
    -654,  1609,  1610,    30,    31,   481,   482,   483,  1616,    30,
      31,   308,  1333,   201,   712,   713,   840,  2087,   809,   309,
     308,   845,   310,   809,   308,   788,   788,  1334,   309,    43,
     308,   310,   309,   809,   809,   310,   206,  1335,   309,   151,
    1337,   864,   882,   883,   884,   885,   886,   279,   280,   880,
     308,    84,  1336,   758,  1602,  1338,   747,   890,   309,   207,
     308,   310,  -696,  -696,  -696,  -696,   210,   308,   309,   699,
     700,   310,   481,   482,   483,   309,   481,   482,   483,  -696,
    -696,  -696,  -696,   481,   482,   483,   234,   235,   341,  1888,
     214,    43,   808,   773,   774,  2088,   714,    30,    31,   911,
     881,    30,    31,   231,   225,  1613,   910,  -696,  -696,   233,
     291,   215,  1614,   808,  1615,   880,   464,   465,   808,   466,
     787,   787,   216,   481,   482,   483,   275,  1877,   808,   808,
     809,  1879,   217,   218,   308,  2089,   219,   240,   462,   463,
     464,   465,   309,   466,   284,   310,   285,   722,   287,   809,
      75,    76,   469,   470,   887,   471,  1894,  1895,  1896,  1897,
    1898,   481,   482,   483,   296,   298,   881,  1385,    86,  1906,
    1907,  1908,  1909,  1910,   300,   301,    92,    93,    94,    95,
    -696,    97,    98,    99,  1811,  1812,  1813,   809,   909,   302,
    1814,  1815,  1944,   703,   704,   705,   706,   707,   708,   709,
     308,   315,  2062,   303,  1946,  1947,  1948,  1949,   309,   444,
     467,   468,   469,   470,    43,   471,   388,  1616,   326,   723,
     389,   788,   788,  1961,   445,   808,   759,   760,   761,   762,
     763,   764,   448,   446,   467,   468,   469,   470,    43,   471,
     490,   157,   158,   159,   808,   449,   447,  1416,  1170,  1418,
     450,  1420,   489,  1422,    43,   491,  1178,  1179,  1181,   492,
    1378,   493,   481,   482,   483,   494,   495,  1436,  1747,  1438,
    1842,  1440,   496,  1442,   497,  1444,   498,   500,  1396,  1370,
     499,   501,   808,   710,  1401,   481,   482,   483,   481,   482,
     483,  1754,   502,   503,  1975,  1848,   481,   482,   483,    43,
    1849,    43,  1979,  1403,   882,   883,   884,   885,   886,   504,
    1850,   521,   481,   482,   483,   505,   787,   787,  1986,   506,
     507,   508,   509,   342,   343,   344,   345,   346,   347,   348,
     349,   525,   350,   351,   352,   353,   354,   510,   511,  1175,
     355,   356,   357,   358,   359,   360,   361,   362,  1452,  1453,
    1454,   512,   473,  1457,   513,  1527,  1460,  1461,   514,   515,
    1465,   516,  1458,  1459,   517,  1470,  1471,  1472,  1473,  1474,
     882,   883,   884,   885,   886,  1480,  1550,   518,  1552,   519,
    1554,   520,  1556,   150,   481,   482,   483,  1492,  1493,  1494,
    1990,   526,  1497,  1498,  1499,  1500,  1501,   522,  1503,  1504,
    1505,   523,  1507,  1882,  1883,   310,   481,   482,   483,   481,
     482,   483,  2064,  1511,  1512,  2067,   888,   524,   527,   528,
    1518,   529,  1521,  1522,   530,   531,   532,  1526,   694,  1528,
     533,  1539,   534,   535,   536,  1535,   173,   174,  1538,   537,
     538,   539,   540,   541,   179,   180,   181,   182,   183,   184,
     462,   463,   464,   465,   542,   466,   543,   647,   648,  1560,
    1561,   649,   202,   203,   204,   205,   650,   651,   652,   208,
     653,   654,  1597,  1574,   655,   656,   363,   657,   658,   659,
    1600,  1601,   889,   660,  1603,   467,   468,   469,   470,  1604,
     471,   227,   228,   229,   230,   661,  1611,   662,   670,   663,
     692,   664,   665,   666,   667,   668,   669,   671,   672,   788,
     673,   809,   674,   675,   676,  1366,   677,   678,  1371,  1373,
     891,   679,   680,  1998,  1999,   681,   682,   683,   899,   414,
     684,   685,   686,   687,   688,   809,   809,   689,   690,   104,
     809,   691,   701,   721,   874,   695,   696,   827,  -700,  -700,
    -700,   466,  -700,  -700,  -700,  -700,  -700,   471,   364,   802,
    1739,  -700,  -700,  -700,  -700,  -700,  -700,   875,   872,  -700,
    -700,  -700,  -700,  -700,   892,   893,    77,    78,    79,    80,
      81,   894,   895,   896,   897,   809,   441,   898,   900,   901,
     809,   902,   788,   788,    88,    89,    90,    91,   903,   904,
     906,   809,   809,   905,   787,  1701,   808,   483,  1466,  1467,
     809,   941,   912,   913,   914,   915,   916,  1475,   917,   102,
     918,   919,   920,  1391,   921,   923,   924,   925,   926,  1487,
     808,   808,   927,  1491,   928,   808,   929,  1596,  1496,   930,
     137,  2072,  2073,   788,   788,  1599,   931,   932,  1506,   933,
     788,   934,   788,   788,   935,   936,   937,   788,   939,   788,
     940,   942,   943,   944,   945,   788,   946,   947,   788,   948,
     949,   950,   951,  1534,  1786,  1536,   952,   953,   954,  1787,
     808,  1788,  1543,  1543,   955,   808,  1548,   787,   787,   788,
     788,   956,   809,   957,   958,   959,   808,   808,   809,   809,
     960,   809,   961,   788,   962,   808,   963,   964,   965,   966,
    1817,   967,   415,   416,   417,   418,   419,   420,   421,  1805,
     422,   423,   424,   425,   426,   427,   428,   429,   968,   430,
     431,   432,   433,   434,   969,   435,   970,   971,   787,   787,
     693,   972,   974,   975,   976,   787,   977,   787,   787,   979,
     980,   981,   787,   982,   787,   983,   984,   985,   986,   987,
     787,   988,   989,   787,   990,   991,   992,  1542,   993,   994,
     995,  1547,   996,   998,   999,  1000,  1001,  1002,  1003,  1004,
    1157,  1158,  1159,  1160,   787,   787,  1161,   808,  1162,  1163,
    1164,  1165,  1166,   808,   808,  1167,   808,  1169,   787,  1171,
    1172,   311,  1173,  1174,  1176,  1177,  1182,  1183,  1184,  -700,
    -700,  -700,  -700,  1391,   809,  1391,  1185,  1187,  1189,  1190,
    1191,  1197,  1198,  1210,  1199,  1211,  -700,  -700,  -700,  -700,
    1212,  1858,  1361,  1860,   330,  1213,  1220,  1221,   809,  1232,
    1863,  1233,  1234,  1235,  1236,  1237,  1238,  1865,  1239,  1240,
    1241,  1242,  1243,   809,  -700,  -700,  1244,  1245,  1246,  1247,
    1248,  1249,  1250,  1251,  1252,  1253,  1254,   458,   460,  1255,
    1256,  1809,  1257,   472,  1258,  1259,  1260,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,  1261,
    1311,  1262,  1263,  1264,   809,  1265,  1266,  1267,  1268,   808,
    1269,  1270,  1271,  1272,  1342,  1273,  1274,   436,  1275,  1880,
    1276,  1277,  1278,  1279,  1280,  1281,  1282,  -700,  1283,  1284,
    1285,  1286,  1287,   808,  1288,  1363,  1289,  1290,  1291,  1292,
    1293,  1294,   313,  1295,  1343,   313,   387,  1296,   808,  1297,
    1298,  1790,  1299,  1300,  1794,  1301,   809,  1302,  1925,  1303,
    1304,  1305,  1306,  1307,  1308,  1309,  1310,  1312,  1927,  1953,
    1313,  1954,  1931,  1932,  1314,  1315,   473,  1316,  1391,  1319,
    1320,  1321,  1322,  1323,  1324,  1391,  1325,  1326,  1327,  1328,
    1391,  1329,  1958,  1330,  1339,  1340,  1341,  1344,  1345,   808,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,  1353,  1354,  1355,
    1992,  1993,  1994,  1356,  1357,  1997,  1358,  1359,  2000,  1360,
    1364,   724,   726,   728,   730,   732,   734,   736,   738,   740,
     742,  1362,  1367,  1368,   472,  1375,  2012,   312,  1376,  1379,
     312,  1381,  1382,  1383,  1798,  1384,  1386,  1387,  1390,  2024,
    2025,   808,  1393,  2026,  1394,  1395,  1810,   747,  1399,  1405,
    1406,  1400,  1423,  1424,  1425,  1426,  1427,   786,   786,  1428,
    1429,  1430,   786,  1431,   786,   786,  1432,  1433,  1434,   786,
    1445,  1446,  1447,  1448,   786,   786,  1449,   330,  1450,  1478,
    1479,  1510,  1516,   330,   330,  1517,   330,  1529,   786,  1530,
    1533,  1531,  1537,  1540,  1546,  1557,  1579,  1864,  2043,  1580,
    1581,  1583,  1584,  1585,  1586,  1588,  1590,  1591,  2045,  2046,
    1592,  1593,  2049,  1594,  1605,  1608,  1617,  1612,  1619,  1620,
    1621,  1622,  1624,  1626,  1745,  1628,  1623,   474,  1632,  1625,
    1629,   474,  1816,  1627,  1630,  1631,   475,  1633,  1634,   788,
     475,  1636,  1635,  1637,  1641,   313,  1638,  1639,  1650,  1640,
    1861,  1643,  1642,  1857,  1886,  1884,  1645,  1644,  1646,  1807,
    2079,  1648,  1818,  1656,  1657,  1651,  2100,  2101,  1647,  1649,
    1652,  1653,  2092,  2106,  2107,  1658,  1654,  1655,  1659,  1660,
     809,   809,  2094,  1661,  1662,   809,  1663,   809,   788,  1664,
    1665,  1666,   788,   788,  1667,  1668,  1669,  1670,  1673,   330,
    1674,  1675,  1676,  1677,  1678,   475,  1902,   809,  1679,  1681,
    1682,  1685,  1686,  1687,  1688,  1689,  1690,  1691,  1693,   809,
     330,  1694,   473,   474,   810,   330,   473,   786,   786,  1695,
     816,   817,   475,   819,   787,   330,   330,  1697,  1698,  1699,
     312,  1700,  1702,  1703,  1704,  1705,  1706,  1707,  1708,  1709,
    1710,  1712,  1714,  1716,  1717,  1718,  1901,  1719,  1720,  1723,
    1725,  1726,  1905,  1727,  1728,  1729,  1730,  1731,  1911,  1912,
    1733,  1732,  1916,  1734,  1735,   808,   808,  1738,  1743,  1744,
     808,  1746,   808,   787,  1742,  1753,  1740,   787,   787,  1756,
    1758,  1757,   809,   809,  1941,   908,  1391,  1759,  1760,  1819,
    1761,  1762,   808,  1763,  1765,  1766,  1767,  1768,  1769,  1770,
    1771,  1772,  1773,  1774,   808,  1775,  1776,  1777,   473,  1778,
    1779,  1780,  1781,  1782,  1783,  1784,  1785,  1791,  1792,  1795,
    1796,  1797,   330,  1799,  1800,   809,  1801,  1820,   788,   788,
    1806,  1802,   788,  1803,  1821,  1822,   830,  1823,  1824,  1825,
    1826,   330,  1829,  1827,  1830,  1831,  1832,  1828,  1833,  1834,
    1835,  1836,  1837,  1838,  1843,  1839,  1840,   850,  1841,  1913,
    1917,  1856,   855,  2061,  1974,  1978,  1859,  1989,  1844,  1873,
    1845,  1846,   866,   867,  1847,  1851,  1852,   808,   808,   330,
     788,  1853,  1854,  1855,  1862,  1866,  2029,  1867,   474,  2032,
    1868,  1869,  1871,  1870,  1889,  1872,  1874,   475,  1890,   809,
    2028,  2030,   788,  1875,  2031,  1618,  2085,  2050,  1876,  1878,
     283,    44,    45,   786,   786,  1887,  1891,    46,  1005,    47,
     808,  1963,    48,   787,   787,   809,   809,   787,  1892,  1893,
    1933,  1935,  1941,  1945,   297,  1936,  1959,    49,   299,  1951,
    1952,  1955,  1956,  1964,  1960,  1962,  1965,  1966,  1967,   472,
    1996,  1968,  1969,  1995,  2017,  2068,  2069,  2070,  1970,  1971,
    1972,  1973,  2027,  1976,  1977,  1980,  1981,  1982,  1983,   922,
    1984,  1985,  1987,  1988,  1991,   787,    50,    52,    53,  2006,
    2001,  2002,  2003,  1391,  2004,  2005,  2008,  2013,   938,  2014,
    2015,  2016,  2020,  2021,   808,    54,   766,   787,  2033,  2034,
    2035,  2053,  2036,  2037,  2038,  2039,   779,  2041,  2044,  2051,
    2052,  2057,   785,  2058,    55,   791,   792,   793,  2054,   795,
     808,   808,   798,   799,   800,  2055,   973,  2056,   803,   804,
    2059,  2063,   807,  2065,   811,   812,   813,   814,   815,  2066,
    2071,   818,  2081,   820,  2074,   822,   823,   824,   825,   826,
     105,   108,  2075,  2076,  2077,  2078,  2082,  2098,  2083,  -699,
    -699,  -699,  2093,  -699,  -699,  -699,  -699,  -699,  2095,  2097,
    2099,  2102,  -699,  -699,  -699,  -699,  -699,  -699,  2103,  2104,
    -699,  -699,  -699,  -699,  -699,  2112,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,  2105,  2108,  2109,  2113,   106,  2114,  2115,  2116,  2119,
    2117,  2118,  2121,  2122,  -697,  -697,  -697,    56,  -697,  -697,
    -697,  -697,  -697,  2120,    57,    58,    59,  -697,  -697,  -697,
    -697,  -697,  -697,   325,   328,  -697,  -697,  -697,  -697,  -697,
      65,     0,     0,     0,   829,     0,     0,   832,   833,   834,
     835,   836,   837,   838,   839,     0,     0,     0,     0,   844,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     854,     0,   107,     0,     0,     0,     0,   861,   862,     0,
     865,  -698,  -698,  -698,     0,  -698,  -698,  -698,  -698,  -698,
       0,   786,     0,   330,  -698,  -698,  -698,  -698,  -698,  -698,
       0,     0,  -698,  -698,  -698,  -698,  -698,     0,     0,     2,
       3,     0,     4,     0,     0,     0,     0,   330,   330,     5,
       6,     7,   330,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,    15,    16,    17,    18,    19,     0,     0,
      20,    21,    22,    23,    24,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   330,     0,     0,
      25,     0,   330,     0,   786,   786,     0,     0,     0,     0,
       0,     0,     0,   330,   330,     0,     0,     0,     0,     0,
       0,     0,   330,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -699,  -699,  -699,  -699,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   786,   786,  -699,  -699,  -699,
    -699,     0,   786,     0,   786,   786,     0,     0,     0,   786,
    1374,   786,     0,     0,     0,     0,     0,   786,     0,     0,
     786,     0,     0,     0,     0,  -699,  -699,     0,     0,   997,
       0,     0,     0,     0,  1397,  1398,     0,     0,     0,  1402,
       0,   786,   786,     0,   330,  -697,  -697,  -697,  -697,     0,
     330,   330,     0,   330,     0,   786,     0,     0,     0,     0,
       0,     0,  -697,  -697,  -697,  -697,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1451,     0,     0,     0,     0,  1456,
    -697,  -697,     0,     0,     0,     0,     0,     0,  -699,     0,
    1468,  1469,     0,     0,     0,     0,     0,     0,     0,  1477,
       0,     0,  -698,  -698,  -698,  -698,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1192,     0,     0,     0,  -698,
    -698,  -698,  -698,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      26,    27,    28,    29,     0,     0,     0,  -698,  -698,     0,
       0,     0,     0,  -697,     0,     0,   330,    30,    31,    32,
      33,     0,     0,     0,     0,    34,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    35,     0,     0,    36,    37,
     330,  1563,     0,     0,     0,    38,    39,  1569,  1570,     0,
    1572,     0,     0,     0,     0,   330,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -698,     0,     0,   472,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   330,     0,     0,     0,
       0,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   311,     0,     0,   311,     0,     0,
     -14,   103,     0,   -14,     0,     0,     0,     0,   330,     0,
     -14,   -14,   -14,  1672,   -14,   -14,   -14,   -14,   -14,   -14,
       0,     0,     0,   -14,   -14,   -14,   -14,   -14,   -14,     0,
       0,   -14,   -14,   -14,   -14,   -14,     0,  1696,     0,     0,
       0,     5,     6,     7,     0,     8,     9,    10,    11,    12,
       0,     0,  1711,     0,    14,    15,    16,    17,    18,    19,
    1404,   -14,    20,    21,    22,    23,    24,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1764,     0,     0,     0,  1455,     0,     0,
       0,     0,     0,  1462,  1463,  1464,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1476,     0,     0,
       0,     0,  1481,  1482,  1483,  1484,  1485,  1486,     0,  1488,
    1489,  1490,     0,     0,     0,     0,  1495,     0,     0,     0,
       0,     0,     0,  1502,     0,  1804,     0,     0,     0,  1509,
       0,     0,     0,  1513,  1514,  1515,     0,     0,     0,  1520,
       0,     0,  1523,  1524,  1525,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   472,
       0,     0,     0,   472,     0,     0,     0,     0,     0,     0,
       0,   786,     0,     0,     0,  1558,  1559,   311,     0,  1562,
       0,  1564,  1565,  1566,  1567,  1568,     0,     0,  1571,     0,
    1573,     0,  1575,  1576,  1577,  1578,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   330,   330,     0,     0,     0,   330,     0,   330,
     786,     0,     0,     0,   786,   786,     0,     0,     0,     0,
       0,  1940,     0,     0,  1606,  1607,     0,     0,     0,   330,
       0,   -14,   -14,   -14,   -14,     0,     0,     0,     0,     0,
       0,   330,     0,     0,     0,   472,     0,     0,   -14,   -14,
     -14,   -14,     0,     0,     0,     0,   -14,     0,     0,     0,
       0,     0,    26,    27,    28,    29,   -14,     0,     0,   -14,
     -14,     0,     0,     0,     0,     0,   -14,   -14,     0,    30,
      31,    32,    33,     0,     0,     0,     0,     0,     0,     0,
       0,  1671,     0,     0,     0,     0,     0,     0,     0,     0,
    1680,     0,     0,  1683,  1684,     0,     0,    38,    39,     0,
       0,     0,  1692,     0,   330,   330,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1713,     0,  1715,     0,     0,     0,     0,
       0,  1721,  1722,   -14,  1724,     0,     0,     0,     0,  1919,
    1920,     0,     0,     0,  1924,     0,  1926,   330,     0,     0,
     786,   786,     0,     0,   786,     0,     0,     0,     0,  1940,
       0,     0,     0,     0,     0,     0,  1950,     0,     0,     0,
     327,     0,     0,     0,     0,     0,     0,     0,  1957,     0,
       0,     0,   -18,   104,     0,   -18,     0,     0,     0,     0,
       0,     0,   -18,   -18,   -18,     0,   -18,   -18,   -18,   -18,
     -18,   -18,   786,     0,     0,   -18,   -18,   -18,   -18,   -18,
     -18,     0,     0,   -18,   -18,   -18,   -18,   -18,     0,     0,
       0,   330,     0,     0,   786,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   -18,     0,     0,     0,   330,   330,     0,
       0,  2022,  2023,     0,     0,     5,     6,     7,     0,     8,
       9,    10,    11,    12,     0,     0,     0,     0,    14,    15,
      16,    17,    18,    19,     0,     0,    20,    21,    22,    23,
      24,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  2042,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  2091,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  2110,  2111,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1881,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   -18,   -18,   -18,   -18,  1918,     0,     0,
    1921,  1922,  1923,     0,     0,     0,     0,  1928,  1929,  1930,
     -18,   -18,   -18,   -18,     0,     0,     0,     0,   -18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   -18,     0,
       0,   -18,   -18,     0,     0,     0,     0,     0,   -18,   -18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1006,     0,     0,     0,     0,     0,    26,    27,    28,    29,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1007,    30,    31,    32,    33,     0,     0,     0,
       0,     0,     0,  1008,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    38,    39,     0,     0,   -18,  2018,  2019,     0,     0,
       0,     0,     0,     0,  1009,  1010,  1011,  1012,  1013,  1014,
    1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,     0,     0,
       0,     0,     0,     0,  1023,  1024,  1025,  1026,  1027,     0,
       0,     0,  1028,     0,  1029,  1030,  1031,  1032,  1033,     0,
       0,  2040,     0,     0,     0,     0,     0,     0,  2047,  2048,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  2080,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  2090,     0,     0,     0,
       0,     0,  2096,  1034,  1035,  1036,  1037,  1038,  1039,  1040,
       0,  1041,  1042,  1043,  1044,  1045,  1046,  1047,  1048,  1049,
    1050,  1051,  1052,  1053,  1054,     0,  1055,  1056,  1057,  1058,
    1059,  1060,  1061,  1062,  1063,  1064,  1065,  1066,  1067,  1068,
    1069,  1070,  1071,  1072,  1073,  1074,  1075,  1076,  1077,  1078,
    1079,  1080,     0,  1081,  1082,  1083,  1084,  1085,  1086,  1087,
    1088,  1089,  1090,  1091,  1092,  1093,  1094,  1095,  1096,  1097,
    1098,     0,  1099,  1100,  1101,  1102,  1103,  1104,  1105,  1106,
    1107,  1108,  1109,     0,  1110,  1111,     0,  1112,  1113,  1114,
       0,     0,     0,     0,     0,     0,     0,  1006,     0,     0,
       0,     0,     0,  1115,  1116,  1117,     0,     0,     0,  1118,
       0,  1119,  1120,  1121,  1122,  1123,  1124,     0,     0,  1007,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1008,     0,  1125,  1126,  1127,     0,     0,  1128,  1129,  1130,
    1131,  1132,  1133,  1134,  1135,  1136,  1137,  1138,  1139,  1140,
    1141,  1142,  1143,  1144,  1145,  1146,  1147,  1148,  1149,  1150,
    1151,  1009,  1010,  1011,  1012,  1013,  1014,  1015,  1016,  1017,
    1018,  1019,  1020,  1021,  1022,     0,     0,     0,     0,     0,
       0,  1023,  1024,  1025,  1026,  1027,     0,   -17,   105,  1028,
     -17,  1029,  1030,  1031,  1032,  1033,     0,   -17,   -17,   -17,
       0,   -17,   -17,   -17,   -17,   -17,   -17,     0,  1152,     0,
     -17,   -17,   -17,   -17,   -17,   -17,     0,     0,   -17,   -17,
     -17,   -17,   -17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   -15,   106,     0,   -15,     0,     0,     0,   -17,     0,
       0,   -15,   -15,   -15,     0,   -15,   -15,   -15,   -15,   -15,
     -15,     0,     0,     0,   -15,   -15,   -15,   -15,   -15,   -15,
       0,     0,   -15,   -15,   -15,   -15,   -15,     0,     0,     0,
    1034,  1035,  1036,  1037,  1038,  1039,  1040,     0,  1041,  1042,
    1043,  1044,  1045,  1046,  1047,  1048,  1049,  1050,  1051,  1052,
    1053,  1054,   -15,  1055,  1056,  1057,  1058,  1059,  1060,  1061,
    1062,  1063,  1064,  1065,  1066,  1067,  1068,  1069,  1070,  1071,
    1072,  1073,  1074,  1075,  1076,  1077,  1078,  1079,  1080,     0,
    1081,  1082,  1083,  1084,  1085,  1086,  1087,  1088,  1089,  1090,
    1091,  1092,  1093,  1094,  1095,  1096,  1097,  1098,     0,  1099,
    1100,  1101,  1102,  1103,  1104,  1105,  1106,  1107,  1108,  1109,
       0,  1110,  1111,     0,  1112,  1113,  1114,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1115,  1116,  1117,     0,     0,     0,  1118,     0,  1119,  1120,
    1121,  1122,  1123,  1124,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1125,
    1126,  1127,     0,     0,  1128,  1129,  1130,  1131,  1132,  1133,
    1134,  1135,  1136,  1137,  1138,  1139,  1140,  1141,  1142,  1143,
    1144,  1145,  1146,  1147,  1148,  1149,  1150,  1151,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   -17,   -17,
     -17,   -17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   -17,   -17,   -17,   -17,     0,
       0,     0,     0,   -17,     0,  1196,     0,     0,     0,     0,
       0,     0,     0,   -17,     0,     0,   -17,   -17,     0,     0,
       0,     0,     0,   -17,   -17,     0,     0,     0,     0,     0,
       0,     0,   -15,   -15,   -15,   -15,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   -15,
     -15,   -15,   -15,     0,     0,     0,     0,   -15,     0,   188,
     189,   306,   307,     0,     0,     0,     0,   -15,     0,     0,
     -15,   -15,     0,     0,     0,     0,     0,   -15,   -15,     0,
       0,   -16,   107,     0,   -16,     0,     0,     0,     0,     0,
     -17,   -16,   -16,   -16,     0,   -16,   -16,   -16,   -16,   -16,
     -16,     0,     0,     0,   -16,   -16,   -16,   -16,   -16,   -16,
       0,     0,   -16,   -16,   -16,   -16,   -16,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   -16,     0,   -15,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   545,   546,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,   561,   562,   563,   564,   565,   566,   567,   568,   569,
     570,   571,   572,   573,   574,   575,   576,   577,   578,   579,
     580,   581,   582,   583,   584,   585,   586,   587,   588,   589,
     590,   591,   592,   593,   594,   595,   596,   597,   598,   599,
     600,   601,   602,   603,   604,   605,   606,   607,   608,   609,
     610,   611,   612,   613,   545,   546,   547,   548,   549,   550,
     551,   552,   553,   554,   555,   556,   557,   558,   559,   560,
     561,   562,   563,   564,   565,   566,   567,   568,   569,   570,
     571,   572,   573,   574,   575,   576,   577,   578,   579,   580,
     581,   582,   583,   584,   585,   586,   587,   588,   589,   590,
     591,   592,   593,   594,   595,   596,   597,   598,   599,   600,
     601,   602,   603,   604,   605,   606,   607,   608,   609,   610,
     611,   612,   613,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   -16,   -16,   -16,   -16,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   -16,
     -16,   -16,   -16,     0,     0,     0,     0,   -16,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   -16,     0,     0,
     -16,   -16,     0,     0,     0,     0,     0,   -16,   -16,   614,
     615,   616,   617,   618,   619,   620,   621,   622,   623,   624,
     625,   626,   627,   628,   629,   630,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,   641,   642,   643,   644,
     645,     0,   308,     0,     0,     0,     0,     0,     0,     0,
     309,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   -16,     0,     0,     0,   614,   615,
     616,   617,   618,   619,   620,   621,   622,   623,   624,   625,
     626,   627,   628,   629,   630,   631,   632,   633,   634,   635,
     636,   637,   638,   639,   640,   641,   642,   643,   644,   645,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   310,   545,   546,   547,   548,   549,   550,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,   561,
     562,   563,   564,   565,   566,   567,   568,   569,   570,   571,
     572,   573,   574,   575,   576,   577,   578,   579,   580,   581,
     582,   583,   584,   585,   586,   587,   588,   589,   590,   591,
     592,   593,   594,   595,   596,   597,   598,   599,   600,   601,
     602,   603,   604,   605,   606,   607,   608,   609,   610,   611,
     612,   613,   545,   546,   547,   548,   549,   550,   551,   552,
     553,   554,   555,   556,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,   570,   571,   572,
     573,   574,   575,   576,   577,   578,   579,   580,   581,   582,
     583,   584,   585,   586,   587,   588,   589,   590,   591,   592,
     593,   594,   595,   596,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
     613,   545,   546,   547,   548,   549,   550,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,   571,   572,   573,
     574,   575,   576,   577,   578,   579,   580,   581,   582,   583,
     584,   585,   586,   587,   588,   589,   590,   591,   592,   593,
     594,   595,   596,   597,   598,   599,   600,   601,   602,   603,
     604,   605,   606,   607,   608,   609,   610,   611,   612,   613,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   614,   615,   616,
     617,   618,   619,   620,   621,   622,   623,   624,   625,   626,
     627,   628,   629,   630,   631,   632,   633,   634,   635,   636,
     637,   638,   639,   640,   641,   642,   643,   644,   645,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1508,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   614,   615,   616,   617,
     618,   619,   620,   621,   622,   623,   624,   625,   626,   627,
     628,   629,   630,   631,   632,   633,   634,   635,   636,   637,
     638,   639,   640,   641,   642,   643,   644,   645,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1519,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   614,   615,   616,   617,   618,
     619,   620,   621,   622,   623,   624,   625,   626,   627,   628,
     629,   630,   631,   632,   633,   634,   635,   636,   637,   638,
     639,   640,   641,   642,   643,   644,   645
};

static const yytype_int16 yycheck[] =
{
     153,   475,  1327,   245,     1,     1,     1,  1170,     1,  1181,
       1,   253,   254,     1,     1,  1178,  1179,    22,    22,    63,
       5,   922,    22,     1,    22,    19,    20,   287,   376,    22,
       1,    19,    20,     1,     1,    62,  1859,    22,     1,     1,
    1202,     1,  1204,   391,  1206,     1,  1208,   224,     1,    22,
     283,     1,    19,    20,     1,   278,     1,     1,   281,     1,
    1222,     1,  1224,   283,  1226,   376,  1228,    22,  1230,   477,
     478,     1,   973,   376,     7,     4,     5,   978,     7,   321,
     388,   323,   393,   325,   209,   376,    73,     1,   391,   242,
       4,     5,     6,     7,     6,     7,   997,    22,   242,     1,
     391,     9,     7,    22,     6,     7,   283,     1,    22,    22,
     376,   502,   503,   504,    22,   506,   311,   312,     1,     5,
      22,     9,   275,   383,    22,   391,   509,    29,   253,   254,
     513,     1,   515,   516,    22,     1,    22,   520,     4,     5,
       6,     7,   525,   526,     1,   288,   289,     4,     5,     6,
       7,     4,     5,     6,     7,   388,   539,     4,     5,     6,
       7,   314,  1324,   376,    22,    22,   389,   376,   388,  1331,
     314,  1333,   385,  1335,     1,  1337,   385,     4,     5,     6,
       7,    22,   226,   376,   242,   383,   244,     4,     5,     6,
       7,     1,   390,   376,   389,    22,     6,     7,   391,  1371,
     325,  1373,   385,  1366,   391,    30,   169,   170,   171,   172,
     173,    36,    22,   208,   383,   210,   211,   275,   387,   214,
     215,   216,   217,   218,   219,   220,   186,   271,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,  2061,   234,
     235,    34,     4,     5,   388,   487,    31,    32,    33,    34,
     308,   309,    31,    32,    33,    34,   314,   648,   316,   317,
     651,     4,     5,     6,     7,   376,   387,   261,   389,   388,
     661,   662,   663,   261,   388,   666,   667,   668,   669,    22,
     391,   672,   673,   674,   288,   289,   677,    22,   384,   680,
     681,   385,     1,   684,   261,   678,   679,   688,   689,   690,
     691,    10,    11,    12,   388,    14,    15,    16,    17,    18,
     180,   181,   182,   183,    23,    24,    25,    26,    27,    28,
     376,   388,    31,    32,    33,    34,    35,     4,     5,     6,
       7,   388,     9,   383,   376,   391,   744,   376,    73,    22,
      31,    32,    33,    34,    31,    32,    33,    29,    30,   391,
     275,   276,   391,   376,    36,   508,   509,   377,   378,   379,
     513,   505,   515,   516,  1536,    47,   383,   520,   391,   377,
     378,   379,   525,   526,   394,   528,  1548,   383,   386,   376,
     376,   534,   535,   388,   537,   376,   539,   383,   384,    21,
     388,    21,   383,   384,   389,   391,    21,   393,   376,   392,
     391,   389,   389,   388,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,   376,   383,   387,   475,   389,   387,
     376,   383,   389,   481,   482,   483,   389,   383,    34,   391,
     383,   376,   376,   388,   387,   391,   910,   387,   383,   389,
     387,   383,   389,   383,   377,   387,   376,   387,   377,   388,
     508,   509,   385,   383,   845,   513,   385,   515,   516,   388,
     388,   394,   520,   377,   389,   377,   283,   525,   526,   388,
     528,   385,   377,   385,   388,   377,   534,   535,    23,   537,
     385,   539,   376,   385,   376,   387,   387,   389,   389,   383,
     388,   874,   875,   376,    31,    32,    33,   650,    43,   391,
     383,  1402,  1403,   288,   289,   377,   378,   379,  1409,   288,
     289,   377,   376,    21,   307,   308,   660,   389,   671,   385,
     377,   665,   388,   676,   377,   678,   679,   391,   385,     1,
     377,   388,   385,   686,   687,   388,    21,   376,   385,    84,
     376,   685,   277,   278,   279,   280,   281,   221,   222,    22,
     377,    23,   391,     1,   387,   391,   389,   701,   385,    21,
     377,   388,   271,   272,   273,   274,   388,   377,   385,   446,
     447,   388,   377,   378,   379,   385,   377,   378,   379,   288,
     289,   290,   291,   377,   378,   379,   386,   387,     1,   394,
     388,    63,   650,   497,   498,   389,   389,   288,   289,   752,
      73,   288,   289,   379,   389,   386,   750,   316,   317,    47,
     389,   388,   393,   671,   395,    22,   379,   380,   676,   382,
     678,   679,   388,   377,   378,   379,   376,  1790,   686,   687,
     783,  1794,   388,   388,   377,   389,   388,   388,   377,   378,
     379,   380,   385,   382,    22,   388,    22,   386,   385,   802,
       6,     7,   379,   380,   389,   382,  1818,  1819,  1820,  1821,
    1822,   377,   378,   379,    22,   388,    73,   383,    24,  1831,
    1832,  1833,  1834,  1835,   388,   388,    32,    33,    34,    35,
     389,    37,    38,    39,    71,    72,    73,   840,   746,   388,
      77,    78,  1864,   299,   300,   301,   302,   303,   304,   305,
     377,    22,  2027,   388,  1866,  1867,  1868,  1869,   385,   388,
     377,   378,   379,   380,   186,   382,   376,  1618,   383,   386,
     376,   874,   875,  1886,   388,   783,   174,   175,   176,   177,
     178,   179,    22,   388,   377,   378,   379,   380,   210,   382,
     383,    89,    90,    91,   802,    21,   388,  1203,   892,  1205,
     298,  1207,   383,  1209,   226,   383,   900,   901,   902,   383,
    1168,   383,   377,   378,   379,   376,   376,  1223,   383,  1225,
    1671,  1227,   376,  1229,   376,  1231,   376,   376,  1186,  1162,
     383,   376,   840,   389,  1192,   377,   378,   379,   377,   378,
     379,   383,   376,   376,   383,  1696,   377,   378,   379,   271,
    1701,   273,   383,  1194,   277,   278,   279,   280,   281,   376,
    1711,   383,   377,   378,   379,   376,   874,   875,   383,   376,
     376,   376,   376,   236,   237,   238,   239,   240,   241,   242,
     243,   393,   245,   246,   247,   248,   249,   376,   376,   897,
     253,   254,   255,   256,   257,   258,   259,   260,  1239,  1240,
    1241,   376,   910,  1244,   376,  1311,  1247,  1248,   376,   376,
    1251,   376,  1245,  1246,   376,  1256,  1257,  1258,  1259,  1260,
     277,   278,   279,   280,   281,  1266,  1332,   376,  1334,   376,
    1336,   376,  1338,    83,   377,   378,   379,  1278,  1279,  1280,
     383,   393,  1283,  1284,  1285,  1286,  1287,   376,  1289,  1290,
    1291,   376,  1293,  1804,  1805,   388,   377,   378,   379,   377,
     378,   379,   383,  1296,  1297,   383,   389,   376,   376,   376,
    1303,   376,  1305,  1306,   376,   376,   376,  1310,   388,  1312,
     376,  1322,   376,   376,   376,  1318,   136,   137,  1321,   376,
     376,   376,   376,   376,   144,   145,   146,   147,   148,   149,
     377,   378,   379,   380,   376,   382,   376,   383,   376,  1342,
    1343,   376,   162,   163,   164,   165,   376,   376,   376,   169,
     376,   376,  1380,  1356,   376,   376,   389,   376,   376,   376,
    1388,  1389,   389,   376,  1392,   377,   378,   379,   380,  1397,
     382,   191,   192,   193,   194,   376,  1404,   376,   383,   376,
     383,   376,   376,   376,   376,   376,   376,   376,   376,  1162,
     376,  1164,   376,   376,   376,  1159,   376,   376,  1162,  1163,
     385,   376,   376,  1924,  1925,   376,   376,   376,   385,     1,
     376,   376,   376,   376,   376,  1188,  1189,   376,   376,     1,
    1193,   376,   376,   376,   376,   386,   386,   383,    10,    11,
      12,   382,    14,    15,    16,    17,    18,   382,     1,   388,
    1534,    23,    24,    25,    26,    27,    28,   376,   388,    31,
      32,    33,    34,    35,   376,   376,     8,     9,    10,    11,
      12,   376,   376,   376,   376,  1238,   286,   376,   376,   376,
    1243,   376,  1245,  1246,    26,    27,    28,    29,   376,   376,
      22,  1254,  1255,   376,  1162,  1496,  1164,   379,  1252,  1253,
    1263,   394,   383,   383,   383,   383,   383,  1261,   383,    51,
     383,   383,   383,  1181,   383,   383,   383,   383,   383,  1273,
    1188,  1189,   383,  1277,   383,  1193,   383,  1379,  1282,   383,
      72,  2042,  2043,  1296,  1297,  1387,   383,   383,  1292,   383,
    1303,   383,  1305,  1306,   383,   383,   383,  1310,   383,  1312,
     383,   394,   383,   383,   383,  1318,   383,   383,  1321,   383,
     383,   383,   383,  1317,  1582,  1319,   383,   383,   383,  1587,
    1238,  1589,  1326,  1327,   383,  1243,  1330,  1245,  1246,  1342,
    1343,   383,  1345,   383,   383,   383,  1254,  1255,  1351,  1352,
     387,  1354,   383,  1356,   383,  1263,   383,   383,   383,   383,
    1618,   383,   184,   185,   186,   187,   188,   189,   190,  1610,
     192,   193,   194,   195,   196,   197,   198,   199,   383,   201,
     202,   203,   204,   205,   383,   207,   383,   383,  1296,  1297,
     440,   383,   383,   383,   383,  1303,   383,  1305,  1306,   383,
     383,   383,  1310,   383,  1312,   383,   383,   383,   383,   383,
    1318,   383,   383,  1321,   383,   383,   383,  1325,   383,   383,
     383,  1329,   383,   383,   383,   383,   383,   383,   383,   383,
     376,   376,   376,   376,  1342,  1343,   376,  1345,   376,   376,
     376,   376,   376,  1351,  1352,    22,  1354,    22,  1356,    22,
      22,   242,    22,    22,    22,    22,   282,     9,     9,   271,
     272,   273,   274,  1371,  1467,  1373,   383,   387,   387,   376,
     376,   383,   385,   376,   385,   385,   288,   289,   290,   291,
     385,  1739,   383,  1741,   275,   385,   385,   385,  1491,   385,
    1748,   385,   385,   385,   385,   385,   376,  1755,   376,   376,
     376,   376,   376,  1506,   316,   317,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   308,   309,   376,
     376,  1613,   376,   314,   376,   376,   376,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   376,
     385,   376,   376,   376,  1557,   376,   376,   376,   376,  1467,
     376,   376,   376,   376,   393,   376,   376,   389,   376,  1802,
     376,   376,   376,   376,   376,   376,   376,   389,   376,   376,
     376,   376,   376,  1491,   376,     9,   376,   376,   376,   376,
     376,   376,  1595,   376,   393,  1598,   389,   376,  1506,   376,
     376,  1595,   376,   376,  1598,   376,  1609,   376,  1849,   376,
     376,   376,   376,   376,   376,   376,   376,   376,  1851,  1877,
     376,  1879,  1855,  1856,   376,   376,  1534,   376,  1536,   376,
     385,   376,   376,   376,   376,  1543,   376,   376,   376,   376,
    1548,   376,  1883,   376,   376,   376,   376,   376,   376,  1557,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
    1918,  1919,  1920,   376,   376,  1923,   376,   376,  1926,   376,
      22,   462,   463,   464,   465,   466,   467,   468,   469,   470,
     471,   383,     9,    22,   475,    22,  1944,  1595,   282,   386,
    1598,   383,   383,   383,  1602,   383,   383,   386,   383,  1957,
    1958,  1609,   383,  1961,   383,   383,  1614,   389,   388,    22,
      22,   388,     9,    22,    22,    22,    22,   508,   509,    22,
      22,    22,   513,    22,   515,   516,    22,    22,    22,   520,
      22,    22,    22,    22,   525,   526,    22,   528,    22,   388,
     388,   388,   388,   534,   535,   388,   537,   264,   539,    22,
      22,   267,    22,   388,    22,   388,   383,  1751,  1999,   383,
     383,   383,   383,   383,   383,   383,   383,   383,  2001,  2002,
     383,    22,  2005,   383,   389,   383,   383,   386,   392,   383,
     392,   383,   383,   383,    22,   383,   392,  1790,   383,   392,
     386,  1794,    22,   392,   386,   386,  1790,   386,   383,  1802,
    1794,   383,   386,   386,   392,  1808,   386,   386,   386,   383,
      22,   392,   383,   267,  1808,    22,   392,   383,   383,   376,
    2053,   383,   376,   383,   383,   386,  2084,  2085,   392,   392,
     386,   386,  2073,  2091,  2092,   383,   386,   386,   383,   383,
    1843,  1844,  2075,   383,   383,  1848,   383,  1850,  1851,   383,
     383,   383,  1855,  1856,   383,   383,   383,   383,   383,   650,
     383,   383,   383,   383,   383,  1859,    22,  1870,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,  1882,
     671,   383,  1790,  1886,   528,   676,  1794,   678,   679,   383,
     534,   535,  1886,   537,  1802,   686,   687,   383,   383,   383,
    1808,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,  1824,   383,   383,   383,
     383,   383,  1830,   383,   383,   383,   383,   383,    22,  1837,
     383,   386,  1840,   383,   383,  1843,  1844,   383,   383,   383,
    1848,   383,  1850,  1851,   387,   383,   394,  1855,  1856,   383,
     383,   392,  1955,  1956,  1862,   746,  1864,   392,   383,   376,
     392,   383,  1870,   392,   383,   383,   394,   394,   383,   383,
     383,   383,   383,   383,  1882,   383,   383,   383,  1886,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   783,   383,   383,  1998,   383,   376,  2001,  2002,
     383,   387,  2005,   387,   376,   376,   650,   376,   376,   376,
     383,   802,   376,   383,   376,   376,   376,   383,   376,   376,
     376,   376,   376,   376,   387,   376,   376,   671,   376,    22,
      22,   376,   676,  2027,    22,    22,   376,    22,   387,   383,
     387,   387,   686,   687,   387,   387,   387,  1955,  1956,   840,
    2053,   387,   387,   387,   387,   376,  1964,   376,  2061,  1967,
     376,   376,   376,   387,   386,   376,   383,  2061,   386,  2072,
      22,    22,  2075,   383,    22,  1411,  2061,  2010,   383,   383,
     219,     1,     1,   874,   875,   383,   386,     1,   872,     1,
    1998,   376,     1,  2001,  2002,  2098,  2099,  2005,   386,   386,
     383,   383,  2010,   383,   233,   386,   383,     1,   235,   388,
     388,   387,   387,   376,   383,   386,   376,   376,   376,   910,
     387,   383,   383,   389,   389,  2033,  2034,  2035,   383,   383,
     383,   383,   376,   383,   383,   383,   383,   383,   383,   783,
     383,   383,   383,   383,   383,  2053,     1,     1,     1,   383,
     387,   387,   387,  2061,   387,   387,   383,   383,   802,   383,
     383,   383,   383,   383,  2072,     1,   495,  2075,   387,   387,
     387,   387,   383,   383,   383,   383,   501,   383,   383,   383,
     383,   383,   507,   383,     1,   510,   511,   512,   387,   514,
    2098,  2099,   517,   518,   519,   389,   840,   389,   523,   524,
     383,   383,   527,   383,   529,   530,   531,   532,   533,   383,
     387,   536,   383,   538,   389,   540,   541,   542,   543,   544,
       1,     1,   387,   389,   387,   389,   383,   387,   383,    10,
      11,    12,   383,    14,    15,    16,    17,    18,   383,   383,
     387,   383,    23,    24,    25,    26,    27,    28,   383,   383,
      31,    32,    33,    34,    35,   383,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,   389,   389,   389,   383,     1,   383,   383,   383,   389,
     383,   383,   383,   383,    10,    11,    12,     1,    14,    15,
      16,    17,    18,   389,     1,     1,     1,    23,    24,    25,
      26,    27,    28,   254,   273,    31,    32,    33,    34,    35,
       1,    -1,    -1,    -1,   649,    -1,    -1,   652,   653,   654,
     655,   656,   657,   658,   659,    -1,    -1,    -1,    -1,   664,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     675,    -1,     1,    -1,    -1,    -1,    -1,   682,   683,    -1,
     685,    10,    11,    12,    -1,    14,    15,    16,    17,    18,
      -1,  1162,    -1,  1164,    23,    24,    25,    26,    27,    28,
      -1,    -1,    31,    32,    33,    34,    35,    -1,    -1,     0,
       1,    -1,     3,    -1,    -1,    -1,    -1,  1188,  1189,    10,
      11,    12,  1193,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    23,    24,    25,    26,    27,    28,    -1,    -1,
      31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1238,    -1,    -1,
      61,    -1,  1243,    -1,  1245,  1246,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1254,  1255,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1263,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     271,   272,   273,   274,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1296,  1297,   288,   289,   290,
     291,    -1,  1303,    -1,  1305,  1306,    -1,    -1,    -1,  1310,
    1164,  1312,    -1,    -1,    -1,    -1,    -1,  1318,    -1,    -1,
    1321,    -1,    -1,    -1,    -1,   316,   317,    -1,    -1,   864,
      -1,    -1,    -1,    -1,  1188,  1189,    -1,    -1,    -1,  1193,
      -1,  1342,  1343,    -1,  1345,   271,   272,   273,   274,    -1,
    1351,  1352,    -1,  1354,    -1,  1356,    -1,    -1,    -1,    -1,
      -1,    -1,   288,   289,   290,   291,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1238,    -1,    -1,    -1,    -1,  1243,
     316,   317,    -1,    -1,    -1,    -1,    -1,    -1,   389,    -1,
    1254,  1255,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1263,
      -1,    -1,   271,   272,   273,   274,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   960,    -1,    -1,    -1,   288,
     289,   290,   291,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     271,   272,   273,   274,    -1,    -1,    -1,   316,   317,    -1,
      -1,    -1,    -1,   389,    -1,    -1,  1467,   288,   289,   290,
     291,    -1,    -1,    -1,    -1,   296,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   306,    -1,    -1,   309,   310,
    1491,  1345,    -1,    -1,    -1,   316,   317,  1351,  1352,    -1,
    1354,    -1,    -1,    -1,    -1,  1506,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     389,    -1,    -1,  1534,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1557,    -1,    -1,    -1,
      -1,    -1,   383,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1595,    -1,    -1,  1598,    -1,    -1,
       0,     1,    -1,     3,    -1,    -1,    -1,    -1,  1609,    -1,
      10,    11,    12,  1467,    14,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    23,    24,    25,    26,    27,    28,    -1,
      -1,    31,    32,    33,    34,    35,    -1,  1491,    -1,    -1,
      -1,    10,    11,    12,    -1,    14,    15,    16,    17,    18,
      -1,    -1,  1506,    -1,    23,    24,    25,    26,    27,    28,
    1195,    61,    31,    32,    33,    34,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1557,    -1,    -1,    -1,  1242,    -1,    -1,
      -1,    -1,    -1,  1248,  1249,  1250,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1262,    -1,    -1,
      -1,    -1,  1267,  1268,  1269,  1270,  1271,  1272,    -1,  1274,
    1275,  1276,    -1,    -1,    -1,    -1,  1281,    -1,    -1,    -1,
      -1,    -1,    -1,  1288,    -1,  1609,    -1,    -1,    -1,  1294,
      -1,    -1,    -1,  1298,  1299,  1300,    -1,    -1,    -1,  1304,
      -1,    -1,  1307,  1308,  1309,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1790,
      -1,    -1,    -1,  1794,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1802,    -1,    -1,    -1,  1340,  1341,  1808,    -1,  1344,
      -1,  1346,  1347,  1348,  1349,  1350,    -1,    -1,  1353,    -1,
    1355,    -1,  1357,  1358,  1359,  1360,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1843,  1844,    -1,    -1,    -1,  1848,    -1,  1850,
    1851,    -1,    -1,    -1,  1855,  1856,    -1,    -1,    -1,    -1,
      -1,  1862,    -1,    -1,  1399,  1400,    -1,    -1,    -1,  1870,
      -1,   271,   272,   273,   274,    -1,    -1,    -1,    -1,    -1,
      -1,  1882,    -1,    -1,    -1,  1886,    -1,    -1,   288,   289,
     290,   291,    -1,    -1,    -1,    -1,   296,    -1,    -1,    -1,
      -1,    -1,   271,   272,   273,   274,   306,    -1,    -1,   309,
     310,    -1,    -1,    -1,    -1,    -1,   316,   317,    -1,   288,
     289,   290,   291,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1466,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1475,    -1,    -1,  1478,  1479,    -1,    -1,   316,   317,    -1,
      -1,    -1,  1487,    -1,  1955,  1956,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1508,    -1,  1510,    -1,    -1,    -1,    -1,
      -1,  1516,  1517,   383,  1519,    -1,    -1,    -1,    -1,  1843,
    1844,    -1,    -1,    -1,  1848,    -1,  1850,  1998,    -1,    -1,
    2001,  2002,    -1,    -1,  2005,    -1,    -1,    -1,    -1,  2010,
      -1,    -1,    -1,    -1,    -1,    -1,  1870,    -1,    -1,    -1,
     389,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1882,    -1,
      -1,    -1,     0,     1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    11,    12,    -1,    14,    15,    16,    17,
      18,    19,  2053,    -1,    -1,    23,    24,    25,    26,    27,
      28,    -1,    -1,    31,    32,    33,    34,    35,    -1,    -1,
      -1,  2072,    -1,    -1,  2075,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    -1,    -1,  2098,  2099,    -1,
      -1,  1955,  1956,    -1,    -1,    10,    11,    12,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    23,    24,
      25,    26,    27,    28,    -1,    -1,    31,    32,    33,    34,
      35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1998,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2072,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  2098,  2099,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1803,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   271,   272,   273,   274,  1842,    -1,    -1,
    1845,  1846,  1847,    -1,    -1,    -1,    -1,  1852,  1853,  1854,
     288,   289,   290,   291,    -1,    -1,    -1,    -1,   296,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   306,    -1,
      -1,   309,   310,    -1,    -1,    -1,    -1,    -1,   316,   317,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       1,    -1,    -1,    -1,    -1,    -1,   271,   272,   273,   274,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,   288,   289,   290,   291,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   316,   317,    -1,    -1,   383,  1951,  1952,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,    -1,
      -1,    -1,    93,    -1,    95,    96,    97,    98,    99,    -1,
      -1,  1996,    -1,    -1,    -1,    -1,    -1,    -1,  2003,  2004,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2054,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  2071,    -1,    -1,    -1,
      -1,    -1,  2077,   184,   185,   186,   187,   188,   189,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,    -1,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,    -1,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,    -1,   265,   266,    -1,   268,   269,   270,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,    -1,
      -1,    -1,    -1,   284,   285,   286,    -1,    -1,    -1,   290,
      -1,   292,   293,   294,   295,   296,   297,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,   313,   314,   315,    -1,    -1,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    86,    87,    88,    89,    -1,     0,     1,    93,
       3,    95,    96,    97,    98,    99,    -1,    10,    11,    12,
      -1,    14,    15,    16,    17,    18,    19,    -1,   389,    -1,
      23,    24,    25,    26,    27,    28,    -1,    -1,    31,    32,
      33,    34,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     0,     1,    -1,     3,    -1,    -1,    -1,    61,    -1,
      -1,    10,    11,    12,    -1,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    23,    24,    25,    26,    27,    28,
      -1,    -1,    31,    32,    33,    34,    35,    -1,    -1,    -1,
     184,   185,   186,   187,   188,   189,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,    61,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,    -1,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
      -1,   265,   266,    -1,   268,   269,   270,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     284,   285,   286,    -1,    -1,    -1,   290,    -1,   292,   293,
     294,   295,   296,   297,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   313,
     314,   315,    -1,    -1,   318,   319,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   271,   272,
     273,   274,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   288,   289,   290,   291,    -1,
      -1,    -1,    -1,   296,    -1,   389,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   306,    -1,    -1,   309,   310,    -1,    -1,
      -1,    -1,    -1,   316,   317,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   271,   272,   273,   274,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,
     289,   290,   291,    -1,    -1,    -1,    -1,   296,    -1,     4,
       5,     6,     7,    -1,    -1,    -1,    -1,   306,    -1,    -1,
     309,   310,    -1,    -1,    -1,    -1,    -1,   316,   317,    -1,
      -1,     0,     1,    -1,     3,    -1,    -1,    -1,    -1,    -1,
     383,    10,    11,    12,    -1,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    23,    24,    25,    26,    27,    28,
      -1,    -1,    31,    32,    33,    34,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,   383,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   271,   272,   273,   274,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,
     289,   290,   291,    -1,    -1,    -1,    -1,   296,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   306,    -1,    -1,
     309,   310,    -1,    -1,    -1,    -1,    -1,   316,   317,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,    -1,   377,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     385,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   383,    -1,    -1,    -1,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   388,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   365,   366,
     367,   368,   369,   370,   371,   372,   373,   374,   375,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   388,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     388,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   375
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   397,     0,     1,     3,    10,    11,    12,    14,    15,
      16,    17,    18,    19,    23,    24,    25,    26,    27,    28,
      31,    32,    33,    34,    35,    61,   271,   272,   273,   274,
     288,   289,   290,   291,   296,   306,   309,   310,   316,   317,
     383,   398,   426,   429,   433,   434,   435,   436,   437,   442,
     443,   445,   446,   449,   457,   463,   467,   470,   473,   477,
     488,   489,   495,   497,   500,   503,   506,   510,   514,   518,
      22,    22,   429,   431,   432,   432,   432,   431,   431,   431,
     431,   431,    22,   427,   429,   488,   432,    62,   431,   431,
     431,   431,   432,   432,   432,   432,    22,   432,   432,   432,
     488,    22,   431,     1,     1,     1,     1,     1,     1,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,   494,   495,   501,   431,   391,   480,
     481,   388,   388,   388,   464,   474,   478,   471,   468,   428,
     481,   488,   504,   384,   288,   289,   430,   430,   430,   430,
     385,   388,   507,   511,   515,   519,   388,   388,   383,   447,
     383,   383,    22,   481,   481,   479,    21,    21,    21,   481,
     481,   481,   481,   481,   481,   388,   388,   502,     4,     5,
     402,   458,   454,   450,   452,    29,    30,    36,   438,   440,
     441,    21,   481,   481,   481,   481,    21,    21,   481,   496,
     388,     1,    22,   392,   388,   388,   388,   388,   388,   388,
      19,    20,   261,   422,   425,   389,   497,   481,   481,   481,
     481,   379,   439,    47,   386,   387,   508,   512,   516,   520,
     388,     1,   376,   384,   393,   480,   482,   483,   484,   485,
     486,   487,   490,   491,   492,   493,   433,   434,   435,   436,
     437,   442,   443,   446,   449,   457,   463,   467,   470,   473,
     477,   497,   498,   499,   503,   376,   462,   472,   476,   221,
     222,   469,   466,   425,    22,    22,   423,   385,     1,   389,
     422,   389,   459,   455,   451,   453,    22,   439,   388,   440,
     388,   388,   388,   388,   444,     1,     6,     7,   377,   385,
     388,   400,   401,   402,   410,    22,   377,   385,   394,   401,
     490,   480,   490,   480,   490,   491,   383,   389,   498,     9,
     400,   401,   402,   405,     1,   169,   170,   171,   172,   173,
     389,     1,   236,   237,   238,   239,   240,   241,   242,   243,
     245,   246,   247,   248,   249,   253,   254,   255,   256,   257,
     258,   259,   260,   389,     1,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   389,   376,   376,
       1,   208,   210,   211,   214,   215,   216,   217,   218,   219,
     220,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   234,   235,   389,     1,   184,   185,   186,   187,   188,
     189,   190,   192,   193,   194,   195,   196,   197,   198,   199,
     201,   202,   203,   204,   205,   207,   389,   389,   287,   383,
     424,   481,     9,    22,   388,   388,   388,   388,    22,    21,
     298,   505,   509,   513,   517,     1,    73,   389,   400,   401,
     400,   401,   377,   378,   379,   380,   382,   377,   378,   379,
     380,   382,   400,   401,   402,   410,   412,   413,   416,   401,
     401,   377,   378,   379,   394,   490,   490,   480,   490,   383,
     383,   383,   383,   383,   376,   376,   376,   376,   376,   383,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   383,   376,   376,   376,   393,   393,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   410,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   419,   383,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     383,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   383,   481,   388,   386,   386,   456,   448,   448,
     448,   376,    34,   299,   300,   301,   302,   303,   304,   305,
     389,    34,   307,   308,   389,   278,   281,   389,   311,   312,
     389,   376,   386,   386,   400,   401,   400,   401,   400,   401,
     400,   401,   400,   401,   400,   401,   400,   401,   400,   401,
     400,   401,   400,   401,   412,     1,   387,   389,   411,     1,
     387,   411,   387,   411,   401,   401,   401,   490,     1,   174,
     175,   176,   177,   178,   179,   460,   460,     1,   180,   181,
     182,   183,   461,   461,   461,   400,   401,   402,   403,   419,
     403,   403,   403,   410,   403,   419,   400,   401,   402,   404,
     404,   419,   419,   419,   404,   419,   404,   404,   419,   419,
     419,   404,   388,   419,   419,   404,   404,   419,   401,   402,
     405,   419,   419,   419,   419,   419,   405,   405,   419,   405,
     419,   404,   419,   419,   419,   419,   419,   383,   403,   419,
     405,   403,   419,   419,   419,   419,   419,   419,   419,   419,
     410,   403,   403,   403,   419,   410,   403,   403,   403,   403,
     405,   403,   403,   403,   419,   405,   403,   404,   404,   403,
     403,   419,   419,   403,   410,   419,   405,   405,   403,   403,
     403,   403,   388,   421,   376,   376,    22,   275,   276,   389,
      22,    73,   277,   278,   279,   280,   281,   389,   389,   389,
     410,   385,   376,   376,   376,   376,   376,   376,   376,   385,
     376,   376,   376,   376,   376,   376,    22,   411,   400,   401,
     410,   402,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   405,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   405,   383,
     383,   394,   394,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     387,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   405,   383,   383,   383,   383,   403,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   419,   383,   383,
     383,   383,   383,   383,   383,   421,     1,    23,    34,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    85,    86,    87,    88,    89,    93,    95,
      96,    97,    98,    99,   184,   185,   186,   187,   188,   189,
     190,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     265,   266,   268,   269,   270,   284,   285,   286,   290,   292,
     293,   294,   295,   296,   297,   313,   314,   315,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   389,   465,   475,   404,   404,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,    22,   409,    22,
     410,    22,    22,    22,    22,   401,    22,    22,   410,   410,
     283,   410,   282,     9,     9,   383,   412,   387,   406,   387,
     376,   376,   419,   406,   406,   406,   389,   383,   385,   385,
     376,   391,   376,   391,   376,   391,   376,   391,   376,   391,
     376,   385,   385,   385,   376,   385,   376,   385,   376,   385,
     385,   385,   376,   391,   376,   391,   376,   391,   376,   391,
     376,   391,   385,   385,   385,   385,   385,   385,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   385,   376,   376,   376,   376,   376,   376,   393,   376,
     385,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   391,   376,   391,   376,   391,   376,   391,   376,
     376,   376,   393,   393,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   383,   383,     9,    22,    22,   410,     9,    22,   283,
     404,   410,   283,   410,   405,    22,   282,   406,   411,   386,
     409,   383,   383,   383,   383,   383,   383,   386,   409,   409,
     383,   401,   414,   383,   383,   383,   411,   405,   405,   388,
     388,   411,   405,   403,   419,    22,    22,     5,    22,   407,
     408,   410,     9,    22,   399,   408,   399,   408,   399,   408,
     399,   408,   399,     9,    22,    22,    22,    22,    22,    22,
      22,    22,    22,    22,    22,   408,   399,   408,   399,   408,
     399,   408,   399,   408,   399,    22,    22,    22,    22,    22,
      22,   405,   403,   403,   403,   419,   405,   403,   404,   404,
     403,   403,   419,   419,   419,   403,   410,   410,   405,   405,
     403,   403,   403,   403,   403,   410,   419,   405,   388,   388,
     403,   419,   419,   419,   419,   419,   419,   410,   419,   419,
     419,   410,   403,   403,   403,   419,   410,   403,   403,   403,
     403,   403,   419,   403,   403,   403,   410,   403,   388,   419,
     388,   404,   404,   419,   419,   419,   388,   388,   404,   388,
     419,   404,   404,   419,   419,   419,   404,   399,   404,   264,
      22,   267,   420,    22,   410,   404,   410,    22,   404,   403,
     388,   408,   401,   410,   415,   415,    22,   401,   410,   408,
     399,   408,   399,   408,   399,   408,   399,   388,   419,   419,
     404,   404,   419,   405,   419,   419,   419,   419,   419,   405,
     405,   419,   405,   419,   404,   419,   419,   419,   419,   383,
     383,   383,   409,   383,   383,   383,   383,   414,   383,   414,
     383,   383,   383,    22,   383,   376,   490,   411,   376,   490,
     411,   411,   387,   411,   411,   389,   419,   419,   383,   406,
     406,   411,   386,   386,   393,   395,   406,   383,   407,   392,
     383,   392,   383,   392,   383,   392,   383,   392,   383,   386,
     386,   386,   383,   386,   383,   386,   383,   386,   386,   386,
     383,   392,   383,   392,   383,   392,   383,   392,   383,   392,
     386,   386,   386,   386,   386,   386,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   419,   405,   383,   383,   383,   383,   383,   383,   383,
     419,   383,   383,   419,   419,   383,   383,   383,   383,   383,
     383,   383,   419,   383,   383,   383,   405,   383,   383,   383,
     383,   403,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   405,   383,   419,   383,   419,   383,   383,   383,   383,
     383,   419,   419,   383,   419,   383,   383,   383,   383,   383,
     383,   383,   386,   383,   383,   383,   383,   390,   383,   412,
     394,   414,   387,   383,   383,    22,   383,   383,   414,     1,
     383,   387,   383,   383,   383,   414,   383,   392,   383,   392,
     383,   392,   383,   392,   405,   383,   383,   394,   394,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   411,   411,   411,    22,
     410,   383,   383,    22,   410,   383,   383,   383,   401,   383,
     383,   383,   387,   387,   405,   403,   383,   376,   376,   490,
     401,    71,    72,    73,    77,    78,    22,   411,   376,   376,
     376,   376,   376,   376,   376,   376,   383,   383,   383,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   406,   387,   387,   387,   387,   387,   406,   406,
     406,   387,   387,   387,   387,   387,   376,   267,   411,   376,
     411,    22,   387,   411,   410,   411,   376,   376,   376,   376,
     387,   376,   376,   383,   383,   383,   383,   409,   383,   409,
     404,   419,   406,   406,    22,    22,   410,   383,   394,   386,
     386,   386,   386,   386,   408,   408,   408,   408,   408,    22,
     388,   401,    22,    22,   388,   401,   408,   408,   408,   408,
     408,    22,   401,    22,    22,   388,   401,    22,   419,   405,
     405,   419,   419,   419,   405,   403,   405,   404,   419,   419,
     419,   404,   404,   383,   413,   383,   386,     1,    22,    29,
     400,   401,   417,   418,   414,   383,   408,   408,   408,   408,
     405,   388,   388,   411,   411,   387,   387,   405,   403,   383,
     383,   409,   386,   376,   376,   376,   376,   376,   383,   383,
     383,   383,   383,   383,    22,   383,   383,   383,    22,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,    22,
     383,   383,   411,   411,   411,   389,   387,   411,   406,   406,
     411,   387,   387,   387,   387,   387,   383,   383,   383,     1,
     387,   389,   411,   383,   383,   383,   383,   389,   419,   419,
     383,   383,   405,   405,   411,   411,   411,   376,    22,   401,
      22,    22,   401,   387,   387,   387,   383,   383,   383,   383,
     419,   383,   405,   403,   383,   404,   404,   419,   419,   404,
     417,   383,   383,   387,   387,   389,   389,   383,   383,   383,
      22,   410,   415,   383,   383,   383,   383,   383,   401,   401,
     401,   387,   406,   406,   389,   387,   389,   387,   389,   404,
     419,   383,   383,   383,   413,   416,   383,   389,   389,   389,
     419,   405,   403,   383,   404,   383,   419,   383,   387,   387,
     411,   411,   383,   383,   383,   389,   411,   411,   389,   389,
     405,   405,   383,   383,   383,   383,   383,   383,   383,   389,
     389,   383,   383
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   396,   397,   397,   397,   397,   397,   397,   397,   397,
     397,   397,   397,   397,   397,   397,   397,   397,   397,   397,
     397,   397,   397,   397,   397,   397,   397,   397,   397,   398,
     399,   399,   400,   400,   400,   400,   400,   400,   400,   400,
     400,   400,   400,   400,   400,   400,   400,   400,   400,   400,
     400,   400,   401,   401,   401,   401,   401,   401,   402,   402,
     403,   403,   403,   404,   404,   404,   405,   405,   405,   406,
     406,   407,   407,   407,   408,   408,   408,   409,   409,   409,
     410,   411,   412,   412,   412,   412,   412,   413,   413,   413,
     414,   414,   415,   415,   415,   416,   416,   417,   417,   417,
     417,   418,   418,   418,   418,   418,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   420,   420,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   423,   422,   424,   422,   422,
     422,   422,   422,   425,   425,   425,   427,   426,   428,   426,
     429,   429,   430,   430,   430,   431,   432,   432,   432,   433,
     434,   435,   436,   437,   438,   438,   439,   439,   440,   440,
     441,   441,   441,   441,   442,   443,   444,   444,   444,   445,
     445,   445,   445,   445,   445,   446,   447,   446,   448,   448,
     448,   448,   448,   448,   448,   448,   448,   448,   448,   450,
     451,   449,   452,   453,   449,   454,   455,   449,   456,   456,
     456,   456,   456,   458,   459,   457,   460,   460,   460,   460,
     460,   460,   460,   461,   461,   461,   461,   461,   462,   462,
     462,   462,   462,   462,   462,   464,   463,   463,   465,   465,
     465,   465,   465,   465,   465,   465,   465,   465,   465,   465,
     465,   465,   465,   465,   465,   465,   465,   465,   465,   465,
     465,   465,   465,   465,   465,   465,   465,   465,   465,   465,
     465,   465,   465,   465,   465,   465,   465,   465,   465,   465,
     465,   465,   465,   465,   465,   465,   465,   465,   465,   465,
     465,   465,   465,   465,   465,   465,   465,   465,   465,   465,
     465,   465,   465,   465,   465,   465,   465,   465,   465,   465,
     465,   465,   465,   465,   466,   466,   466,   466,   466,   466,
     466,   466,   466,   466,   466,   466,   466,   466,   466,   466,
     466,   466,   466,   466,   466,   466,   466,   466,   468,   467,
     467,   469,   469,   469,   469,   469,   469,   469,   469,   469,
     469,   469,   469,   469,   469,   469,   469,   469,   469,   469,
     469,   469,   469,   469,   469,   469,   469,   471,   470,   470,
     472,   472,   472,   472,   472,   472,   472,   472,   472,   472,
     472,   472,   472,   472,   472,   472,   472,   472,   472,   472,
     472,   472,   472,   474,   473,   473,   475,   475,   475,   475,
     475,   475,   475,   475,   475,   475,   475,   475,   475,   475,
     475,   475,   475,   475,   475,   475,   475,   475,   476,   476,
     476,   476,   476,   476,   476,   476,   476,   476,   476,   476,
     476,   476,   476,   476,   476,   476,   476,   476,   476,   476,
     476,   476,   478,   477,   477,   479,   479,   479,   479,   479,
     479,   480,   481,   481,   482,   483,   484,   485,   485,   486,
     487,   488,   488,   488,   489,   489,   489,   489,   489,   489,
     489,   490,   490,   490,   490,   490,   490,   490,   491,   492,
     492,   493,   493,   493,   493,   493,   493,   493,   493,   493,
     493,   493,   493,   493,   493,   493,   493,   494,   494,   494,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   496,   495,   497,   497,   498,   498,   498,
     498,   498,   498,   498,   498,   498,   498,   498,   498,   498,
     498,   498,   498,   498,   499,   499,   501,   500,   502,   502,
     502,   504,   503,   505,   505,   505,   505,   505,   505,   505,
     505,   505,   505,   505,   507,   508,   506,   509,   509,   509,
     509,   509,   509,   511,   512,   510,   513,   513,   513,   513,
     515,   516,   514,   517,   517,   517,   519,   520,   518
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     4,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     3,     3,     3,
       3,     3,     1,     3,     3,     3,     2,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     1,     1,     3,     0,     1,     3,     0,     1,     3,
       1,     1,     1,     1,     3,     3,     2,     3,     5,     2,
       1,     3,     3,     5,     2,     1,     3,     1,     1,     1,
       1,     0,     1,     1,     3,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     0,
       2,     2,     5,     5,     5,     8,     5,     8,     5,     8,
       5,     8,     5,     8,     5,     8,     5,     8,     5,     8,
       5,     8,     5,     8,     5,     8,     5,     8,     5,     8,
       5,     8,    10,     8,     7,    11,    13,    11,    13,     5,
       8,     8,     8,     8,    10,    12,    12,     8,     8,    10,
      12,     8,     8,     8,     8,    10,    10,     8,     8,    10,
       5,     6,     5,     6,     5,     6,     5,     5,     5,     5,
       5,     8,     7,     7,     8,     5,     5,     7,     5,     9,
       5,     5,     5,     8,     3,     0,     6,     0,     7,     7,
       7,     3,     4,     1,     2,     2,     0,     6,     0,     7,
       1,     1,     0,     1,     1,     1,     0,     2,     3,     4,
       4,     4,     4,     4,     0,     1,     0,     1,     3,     4,
       0,     1,     1,     3,     7,     4,     0,     5,     2,     1,
       1,     1,     1,     1,     1,     3,     0,     7,     0,     5,
       5,     5,     7,     5,     5,     7,     5,     5,     5,     0,
       0,     9,     0,     0,     9,     0,     0,     9,     0,     5,
       5,     7,     5,     0,     0,     9,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     5,
       5,     5,     5,     5,     3,     0,     7,     2,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     8,     4,     4,     8,     4,     4,     4,
       4,     4,     4,     4,     8,     4,     8,    12,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     8,    12,     4,
       4,     4,     4,    12,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     8,     4,     4,    10,    12,     4,     4,
       4,     4,     4,     4,    10,    12,     4,    10,     4,     4,
       4,     4,     4,     4,     0,     5,     5,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       9,     5,     5,     5,     5,     5,     5,     3,     0,     7,
       2,     0,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     5,     5,     5,     4,     8,    13,     5,     5,     5,
       5,    13,     5,     5,     5,     5,     3,     0,     7,     2,
       0,     5,     5,     5,     5,     5,     9,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     5,     3,     0,     7,     3,     8,     4,     4,    13,
      13,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     0,     9,
       5,     5,    14,    14,     5,     5,     5,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     3,     0,     7,     3,     0,     5,     5,     5,     5,
       2,     3,     0,     1,     2,     4,     4,     2,     2,     2,
       4,     1,     1,     1,     1,     2,     2,     3,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       3,     0,     1,     1,     2,     1,     2,     2,     3,     1,
       2,     2,     3,     2,     3,     3,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     6,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     0,     7,     0,     2,
       3,     0,     4,     6,     7,     5,     5,     5,     5,     5,
       5,    10,     8,     7,     0,     0,     8,     0,     7,     7,
      10,     8,     7,     0,     0,     8,     0,     5,     5,     7,
       0,     0,     8,     0,     5,     5,     0,     0,     8
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yytype)
    {
          case 407: /* shaderlist  */
#line 456 "Grammar.ypp" /* yacc.c:1257  */
      { if(((*yyvaluep).shaderList)) delete ((*yyvaluep).shaderList); }
#line 3787 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1257  */
        break;

    case 408: /* one_or_more_shaders  */
#line 456 "Grammar.ypp" /* yacc.c:1257  */
      { if(((*yyvaluep).shaderList)) delete ((*yyvaluep).shaderList); }
#line 3793 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1257  */
        break;

    case 409: /* varnamelist  */
#line 455 "Grammar.ypp" /* yacc.c:1257  */
      { if(((*yyvaluep).varNameList)) delete ((*yyvaluep).varNameList); }
#line 3799 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1257  */
        break;

    case 412: /* fexplist  */
#line 457 "Grammar.ypp" /* yacc.c:1257  */
      { if(((*yyvaluep).floatList)) delete ((*yyvaluep).floatList); }
#line 3805 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1257  */
        break;

    case 413: /* fexpveclist  */
#line 458 "Grammar.ypp" /* yacc.c:1257  */
      { if(((*yyvaluep).floatVecList)) delete ((*yyvaluep).floatVecList); }
#line 3811 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1257  */
        break;

    case 414: /* iexplist  */
#line 459 "Grammar.ypp" /* yacc.c:1257  */
      { if(((*yyvaluep).intList)) delete ((*yyvaluep).intList); }
#line 3817 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1257  */
        break;

    case 416: /* bexplist  */
#line 460 "Grammar.ypp" /* yacc.c:1257  */
      { if(((*yyvaluep).boolList)) delete ((*yyvaluep).boolList); }
#line 3823 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1257  */
        break;

    case 441: /* arglist  */
#line 461 "Grammar.ypp" /* yacc.c:1257  */
      { if(((*yyvaluep).argList)) delete ((*yyvaluep).argList); }
#line 3829 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1257  */
        break;

    case 482: /* valueassignment_f  */
#line 458 "Grammar.ypp" /* yacc.c:1257  */
      { if(((*yyvaluep).floatVecList)) delete ((*yyvaluep).floatVecList); }
#line 3835 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1257  */
        break;

    case 483: /* valueassignment_fv  */
#line 458 "Grammar.ypp" /* yacc.c:1257  */
      { if(((*yyvaluep).floatVecList)) delete ((*yyvaluep).floatVecList); }
#line 3841 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1257  */
        break;

    case 484: /* valueassignment_fvl  */
#line 458 "Grammar.ypp" /* yacc.c:1257  */
      { if(((*yyvaluep).floatVecList)) delete ((*yyvaluep).floatVecList); }
#line 3847 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1257  */
        break;


      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

/* User initialization code.  */
#line 388 "Grammar.ypp" /* yacc.c:1429  */
{
    /*$$.container = Container::create(NULL);
    printf("My initial action %p\n", $$.container );*/
    line_num        = 1;
    bracket_level   = 0;
    //curContainer    = NULL; initialized in loadEffectFromFile()
    curTechnique    = NULL;
    curPass         = NULL;
    curShader       = NULL;
    curProgram      = NULL;
    curSamplerState = NULL;
    curRState       = NULL;
    curCSState      = NULL;
    curDSTState     = NULL;
    curPRState    = NULL;
    curResource     = NULL;
    curFBO          = NULL;
    curUniform      = NULL;
    curCstBuffer    = NULL;
    bCurCstBufferAlreadyPopulated = false;
    pShdRep         = nvFX::getShaderModuleRepositorySingleton()->getExInterface();
    pFboRep         = nvFX::getFrameBufferObjectsRepositorySingleton()->getExInterface();
    pResRep         = nvFX::getResourceRepositorySingleton()->getExInterface();
    pCRep           = nvFX::getCstBufferRepositorySingleton()->getExInterface();
    pURep           = nvFX::getUniformRepositorySingleton()->getExInterface();
}

#line 3960 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1429  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 29:
#line 1041 "Grammar.ypp" /* yacc.c:1646  */
    {
    curContainer->separateShadersEnable((yyvsp[0].bval));
}
#line 4144 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 1049 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4150 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 1050 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.s) = (yyvsp[0].s); }
#line 4156 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 1056 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.fval)  =  (yyvsp[0].fval); }
#line 4162 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 1057 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.fval)  =  (yyvsp[-2].fval)  +  (yyvsp[0].fval); }
#line 4168 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 1058 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.fval)  =  (float)(yyvsp[-2].ival)  +  (yyvsp[0].fval); }
#line 4174 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 1059 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.fval)  =  (yyvsp[-2].fval)  +  (float)(yyvsp[0].ival); }
#line 4180 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 1060 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.fval)  =  (yyvsp[-2].fval)  -  (yyvsp[0].fval); }
#line 4186 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 1061 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.fval)  =  (float)(yyvsp[-2].ival)  -  (yyvsp[0].fval); }
#line 4192 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 1062 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.fval)  =  (yyvsp[-2].fval)  -  (float)(yyvsp[0].ival); }
#line 4198 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 1063 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.fval)  =  (yyvsp[-2].fval)  *  (yyvsp[0].fval); }
#line 4204 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 1064 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.fval)  =  (float)(yyvsp[-2].ival)  *  (yyvsp[0].fval); }
#line 4210 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 1065 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.fval)  =  (yyvsp[-2].fval)  *  (float)(yyvsp[0].ival); }
#line 4216 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 1066 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.fval)  =  (yyvsp[-2].fval)  /  (yyvsp[0].fval); }
#line 4222 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 1067 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.fval)  =  (float)(yyvsp[-2].ival)  /  (yyvsp[0].fval); }
#line 4228 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 1068 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.fval)  =  (yyvsp[-2].fval)  /  (float)(yyvsp[0].ival); }
#line 4234 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 1069 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.fval)  =  (float)(yyvsp[-2].ival)  /  (float)(yyvsp[0].ival); }
#line 4240 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 1070 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.fval)  =  -(yyvsp[0].fval); }
#line 4246 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 1071 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.fval)  =  pow  ((yyvsp[-2].fval),  (yyvsp[0].fval)); }
#line 4252 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 1072 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.fval)  =  pow  ((float)(yyvsp[-2].ival),  (yyvsp[0].fval)); }
#line 4258 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 1073 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.fval)  =  pow  ((yyvsp[-2].fval),  (float)(yyvsp[0].ival)); }
#line 4264 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 1074 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.fval)  =  pow  ((float)(yyvsp[-2].ival),  (float)(yyvsp[0].ival)); }
#line 4270 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 1075 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.fval)  =  (yyvsp[-1].fval); }
#line 4276 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 1083 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.ival)  =  (yyvsp[0].ival); }
#line 4282 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 1084 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.ival)  =  (yyvsp[-2].ival)  +  (yyvsp[0].ival); }
#line 4288 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 1085 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.ival)  =  (yyvsp[-2].ival)  -  (yyvsp[0].ival); }
#line 4294 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 1086 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.ival)  =  (yyvsp[-2].ival)  *  (yyvsp[0].ival); }
#line 4300 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 1087 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.ival)  =  -(yyvsp[0].ival); }
#line 4306 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 1088 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.ival)  =  (yyvsp[-1].ival); }
#line 4312 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 1093 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.bval)  =  true; }
#line 4318 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 1094 "Grammar.ypp" /* yacc.c:1646  */
    {  (yyval.bval)  =  false; }
#line 4324 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 1100 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.bval) = (yyvsp[0].bval); }
#line 4330 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 1101 "Grammar.ypp" /* yacc.c:1646  */
    { 
    if(((yyvsp[0].ival) != 0)&&((yyvsp[0].ival) != 1)) yyerror("    Warning : truncation of float to bool");
    (yyval.bval) = (yyvsp[0].ival) ? true: false; }
#line 4338 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 1104 "Grammar.ypp" /* yacc.c:1646  */
    { 
    if(((yyvsp[0].fval) != 0.0f)&&((yyvsp[0].fval) != 1.0f)) yyerror("    Warning : truncation of float to bool");
    (yyval.bval) = (yyvsp[0].fval) ? true: false; }
#line 4346 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 1108 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.ival) = (yyvsp[0].ival); }
#line 4352 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 1109 "Grammar.ypp" /* yacc.c:1646  */
    { 
    if(((yyvsp[0].fval) - (int)(yyvsp[0].fval))!=0.0f)
        yyerror("    Warning : truncation of float to int");
    (yyval.ival) = (int)(yyvsp[0].fval); }
#line 4361 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 1113 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.ival) = (yyvsp[0].bval) ? 1: 0; }
#line 4367 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 1116 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.fval) = (yyvsp[0].fval); }
#line 4373 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 1117 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.fval) = (float)(yyvsp[0].ival); }
#line 4379 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 1118 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.fval) = (yyvsp[0].bval) ? 1.0f: 0.0f; }
#line 4385 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 1143 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.shaderList) = NULL; }
#line 4391 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 1144 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.shaderList) = new ShaderList;
    IShader *pShader = curContainer->findShader((yyvsp[0].s)->c_str());
    if(pShader == NULL) {
        // second chance: look for it in the global repository
        pShader = pShdRep->findShader((yyvsp[0].s)->c_str());
        if(pShader == NULL) {
            yyerror("Error : Shader not found:");
            yyerror((yyvsp[0].s)->c_str());
        }
    }
    if(pShader)
        ((yyval.shaderList))->push_back(pShader);
    delete (yyvsp[0].s);
}
#line 4411 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 1159 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.shaderList) = (yyvsp[-2].shaderList);
    IShader *pShader = curContainer->findShader((yyvsp[0].s)->c_str());
    if(pShader == NULL) {
        // second chance: look for it in the global repository
        pShader = pShdRep->findShader((yyvsp[0].s)->c_str());
        if(pShader == NULL) {
            yyerror("Error : Shader not found:");
            yyerror((yyvsp[0].s)->c_str());
        }
    }
    if(pShader)
        ((yyval.shaderList))->push_back(pShader);
    delete (yyvsp[0].s);
}
#line 4431 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 1176 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.shaderList) = NULL; }
#line 4437 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 1192 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.shaderList) = (yyvsp[0].shaderList); }
#line 4443 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 1193 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.shaderList) = (yyvsp[-1].shaderList); }
#line 4449 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 1200 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.varNameList) = NULL; }
#line 4455 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 1201 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.varNameList) = new VarNameList;
    (yyval.varNameList)->push_back(*(yyvsp[0].s));
    delete (yyvsp[0].s);
}
#line 4465 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 1206 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.varNameList) = (yyvsp[-2].varNameList);
    (yyval.varNameList)->push_back(*(yyvsp[0].s));
    delete (yyvsp[0].s);
}
#line 4475 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 1231 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.floatList) = new FloatList;
    (yyval.floatList)->push_back((yyvsp[0].fval));
}
#line 4484 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 1235 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.floatList) = new FloatList;
    (yyval.floatList)->push_back((float)(yyvsp[0].ival));
}
#line 4493 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 1239 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.floatList) = (yyvsp[-2].floatList);
    (yyval.floatList)->push_back((yyvsp[0].fval));
}
#line 4502 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 1243 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.floatList) = (yyvsp[-2].floatList);
    (yyval.floatList)->push_back((float)(yyvsp[0].ival));
}
#line 4511 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 1247 "Grammar.ypp" /* yacc.c:1646  */
    { 
    (yyval.floatList) = (yyvsp[-1].floatList);
    yyerror ("Error in the list of floats");
}
#line 4520 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 1253 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.floatVecList) = new FloatVecList;
    (yyval.floatVecList)->push_back((yyvsp[-1].floatList));
}
#line 4529 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 1257 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.floatVecList) = (yyvsp[-4].floatVecList);
    (yyval.floatVecList)->push_back((yyvsp[-1].floatList));
}
#line 4538 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 1261 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.floatVecList) = (yyvsp[-1].floatVecList); yyerror ("Error in the list of default values"); }
#line 4544 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 1264 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.intList) = new IntList;
    (yyval.intList)->push_back((yyvsp[0].ival));
}
#line 4553 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 1273 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.intList) = (yyvsp[-2].intList);
    (yyval.intList)->push_back((yyvsp[0].ival));
}
#line 4562 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 1284 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.intVecList) = new IntVecList;
    (yyval.intVecList)->push_back((yyvsp[-1].intList));
}
#line 4571 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 1288 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.intVecList) = (yyvsp[-4].intVecList);
    (yyval.intVecList)->push_back((yyvsp[-1].intList));
}
#line 4580 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 1292 "Grammar.ypp" /* yacc.c:1646  */
    { yyerror ("Error in the list of default values"); }
#line 4586 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 1295 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.boolList) = new BoolList;
    (yyval.boolList)->push_back((yyvsp[0].bval));
}
#line 4595 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 1299 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.boolList) = (yyvsp[-2].boolList);
    (yyval.boolList)->push_back((yyvsp[0].bval));
}
#line 4604 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 1319 "Grammar.ypp" /* yacc.c:1646  */
    { 
    (yyval.argVal) = new ArgVal(IUniform::TInt); (yyval.argVal)->val.i[0] = (yyvsp[0].ival); 
    LOGD("Arg val = %d\n", (yyvsp[0].ival));
}
#line 4613 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 1323 "Grammar.ypp" /* yacc.c:1646  */
    { 
    (yyval.argVal) = new ArgVal(IUniform::TFloat); (yyval.argVal)->val.f[0] = (yyvsp[0].fval);
    LOGD("Arg val = %f\n", (yyvsp[0].fval));
}
#line 4622 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 1327 "Grammar.ypp" /* yacc.c:1646  */
    {
    IResource* pRes = curContainer->findResource((yyvsp[0].s)->c_str());
    if(pRes) {
        nvFX::IUniform::Type utype;
        switch(pRes->getType())
        {
        case RESOURCE_UNKNOWN: utype = IUniform::TUndefined; break;
        case RESTEX_1D: utype = IUniform::TTexture1D; break;
        case RESTEX_2D: utype = IUniform::TTexture2D; break;
        case RESTEX_2DRECT: utype = IUniform::TTexture2DRect; break;
        case RESTEX_3D: utype = IUniform::TTexture3D; break;
        case RESTEX_CUBE_MAP: utype = IUniform::TTextureCube; break;
        default:
            assert(!"missing convertion. TODO or to CHECK");
        }
        (yyval.argVal) = new ArgVal(utype); (yyval.argVal)->val.pResource = pRes;
        LOGD("Arg val = %s as a resource pointer\n", (yyvsp[0].s)->c_str());
    } else {
        IUniform* pU = curContainer->findUniform((yyvsp[0].s)->c_str());
        if(pU) {
            (yyval.argVal) = new ArgVal(IUniform::TUniform); (yyval.argVal)->val.pUniform = pU;
            LOGD("Arg val = %s from a uniform pointer\n", (yyvsp[0].s)->c_str());
        } else {
            LOGD("Arg val = %s - unknown parameter !\n", (yyvsp[0].s)->c_str());
            yyerror("Unknown parameter\n"); yyerror((yyvsp[0].s)->c_str()); yyerror("\n");
        }
    }
    delete (yyvsp[0].s);
}
#line 4656 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 1356 "Grammar.ypp" /* yacc.c:1646  */
    { 
    yyerror("Error in argument\n");
}
#line 4664 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 1361 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.argValList) = new ArgValList; }
#line 4670 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 1362 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.argValList) = new ArgValList; }
#line 4676 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 1363 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.argValList) = new ArgValList;
    (yyval.argValList)->push_back((yyvsp[0].argVal));
    }
#line 4685 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 1367 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.argValList) = (yyvsp[-2].argValList);
    (yyval.argValList)->push_back((yyvsp[0].argVal));
    }
#line 4694 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 1371 "Grammar.ypp" /* yacc.c:1646  */
    { yyerror("Error in argument list\n");}
#line 4700 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 207:
#line 1493 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.clear_mode) = (yyvsp[0].clear_mode); }
#line 4706 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 208:
#line 1494 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.clear_mode) = (ClearMode)((unsigned int)(yyvsp[0].clear_mode) | (unsigned int)(yyvsp[-2].clear_mode)); }
#line 4712 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 212:
#line 1504 "Grammar.ypp" /* yacc.c:1646  */
    {
    //if($4->size() > 0)
    //    curPass->createState(IPassState::TVertexShader, & (*$4)[0], (int)$4->size());
    delete (yyvsp[-1].s);
    }
#line 4722 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 213:
#line 1509 "Grammar.ypp" /* yacc.c:1646  */
    {
    //if($4->size() > 0)
    //    curPass->createState(IPassState::TVertexShader, & (*$4)[0], (int)$4->size());
    }
#line 4731 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 214:
#line 1513 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createState(IPassState::TVertexShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size());
    delete (yyvsp[-1].shaderList);
    }
#line 4741 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 215:
#line 1518 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createState(IPassState::TVertexShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size(), (yyvsp[-4].s)->c_str());
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].shaderList);
    }
#line 4752 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 216:
#line 1524 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size()>0)
        curPass->createState(IPassState::TFragmentShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size());
    delete (yyvsp[-1].shaderList);
    }
#line 4762 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 217:
#line 1529 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size()>0)
        curPass->createState(IPassState::TFragmentShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size(), (yyvsp[-4].s)->c_str());
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].shaderList);
    }
#line 4773 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 218:
#line 1535 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createState(IPassState::TComputeShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size());
    delete (yyvsp[-1].shaderList);
    }
#line 4783 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 219:
#line 1540 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createState(IPassState::TComputeShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size(), (yyvsp[-4].s)->c_str());
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].shaderList);
    }
#line 4794 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 220:
#line 1546 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createState(IPassState::TPathShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size());
    delete (yyvsp[-1].shaderList);
    }
#line 4804 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 221:
#line 1551 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createState(IPassState::TPathShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size(), (yyvsp[-4].s)->c_str());
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].shaderList);
    }
#line 4815 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 222:
#line 1557 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createState(IPassState::TGeometryShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size());
    delete (yyvsp[-1].shaderList);
    }
#line 4825 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 223:
#line 1562 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createState(IPassState::TGeometryShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size(), (yyvsp[-4].s)->c_str());
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].shaderList);
    }
#line 4836 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 224:
#line 1568 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createState(IPassState::TTCSShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size());
    delete (yyvsp[-1].shaderList);
    }
#line 4846 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 225:
#line 1573 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createState(IPassState::TTCSShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size(), (yyvsp[-4].s)->c_str());
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].shaderList);
    }
#line 4857 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 226:
#line 1579 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createState(IPassState::TTESShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size());
    delete (yyvsp[-1].shaderList);
    }
#line 4867 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 227:
#line 1584 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createState(IPassState::TTESShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size(), (yyvsp[-4].s)->c_str());
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].shaderList);
    }
#line 4878 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 228:
#line 1590 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TVertexShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size());
    delete (yyvsp[-1].shaderList);
    }
#line 4888 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 229:
#line 1595 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TVertexShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size(), (yyvsp[-4].s)->c_str());
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].shaderList);
    }
#line 4899 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 230:
#line 1601 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size()>0)
        curPass->createStateOverride(IPassState::TFragmentShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size());
    delete (yyvsp[-1].shaderList);
    }
#line 4909 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 231:
#line 1606 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size()>0)
        curPass->createStateOverride(IPassState::TFragmentShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size(), (yyvsp[-4].s)->c_str());
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].shaderList);
    }
#line 4920 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 232:
#line 1612 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TComputeShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size());
    delete (yyvsp[-1].shaderList);
    }
#line 4930 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 233:
#line 1617 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TComputeShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size(), (yyvsp[-4].s)->c_str());
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].shaderList);
    }
#line 4941 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 234:
#line 1623 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TPathShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size());
    delete (yyvsp[-1].shaderList);
    }
#line 4951 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 235:
#line 1628 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TPathShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size(), (yyvsp[-4].s)->c_str());
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].shaderList);
    }
#line 4962 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 236:
#line 1634 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TGeometryShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size());
    delete (yyvsp[-1].shaderList);
    }
#line 4972 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 237:
#line 1639 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TGeometryShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size(), (yyvsp[-4].s)->c_str());
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].shaderList);
    }
#line 4983 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 238:
#line 1645 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TTCSShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size());
    delete (yyvsp[-1].shaderList);
    }
#line 4993 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 239:
#line 1650 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TTCSShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size(), (yyvsp[-4].s)->c_str());
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].shaderList);
    }
#line 5004 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 240:
#line 1656 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TTESShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size());
    delete (yyvsp[-1].shaderList);
    }
#line 5014 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 241:
#line 1661 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TTESShader, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size(), (yyvsp[-4].s)->c_str());
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].shaderList);
    }
#line 5025 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 242:
#line 1667 "Grammar.ypp" /* yacc.c:1646  */
    {
    #ifndef OGLES2
    // we prefer this loop : we don't have the char ** available here... using vector of std::string...
    int sz = (int)(yyvsp[-2].varNameList)->size();
    const char ** srnames = (const char **)malloc(sizeof(const char *)*sz);
    for(int i=0; i<(int)(yyvsp[-2].varNameList)->size(); i++)
        srnames[i] = (*(yyvsp[-2].varNameList))[i].c_str();
    curPass->createState((yyvsp[-6].s)->c_str(), srnames, sz);
    delete [] srnames;
    #else
        yyerror("Not supported in ES2\n");
    #endif
    delete (yyvsp[-6].s);
    delete (yyvsp[-2].varNameList);
    }
#line 5045 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 243:
#line 1682 "Grammar.ypp" /* yacc.c:1646  */
    {
    #ifndef OGLES2
    curPass->createState((yyvsp[-4].s)->c_str(), 0, (yyvsp[-1].s)->c_str());
    #else
        yyerror("Not supported in ES2\n");
    #endif
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].s);
    }
#line 5059 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 244:
#line 1691 "Grammar.ypp" /* yacc.c:1646  */
    {
    ValueList* pvalueList = (yyvsp[-1].valueList);
    if(pvalueList && pvalueList->floatVecList)
    {
        FloatVecList *pfvl = pvalueList->floatVecList;
        FloatList* pfl = (*pfvl)[0];
        float* pf = &(*pfl)[0];
        curPass->createState((yyvsp[-3].s)->c_str(), 0, pf, (int)pfl->size());
        delete pfl;
        delete pfvl;
        delete pvalueList;
    }
    delete (yyvsp[-3].s);
    }
#line 5078 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 245:
#line 1748 "Grammar.ypp" /* yacc.c:1646  */
    {
    #ifndef OGLES2
    curPass->createState((yyvsp[-7].s)->c_str(), (yyvsp[-5].ival), (yyvsp[-1].s)->c_str());
    #else
        yyerror("Not supported in ES2\n");
    #endif
    delete (yyvsp[-7].s);
    delete (yyvsp[-1].s);
    }
#line 5092 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 246:
#line 1767 "Grammar.ypp" /* yacc.c:1646  */
    {
    // walk through the vector list and check few things then send the packed version
    int szvec = 0;
    float *farray = NULL;
    float *p = NULL;
    for(int i=0; i<(int)(yyvsp[-2].floatVecList)->size(); i++)
    {
        if(szvec == 0) {
            szvec = (int)(*(yyvsp[-2].floatVecList))[i]->size();
            p = farray = new float[szvec * (yyvsp[-2].floatVecList)->size()]; // we assume all vectors are the same size
        }
        else if(szvec != (*(yyvsp[-2].floatVecList))[i]->size()) {
            yyerror("Vector list has inconsistent vectors\n");
            continue;
        }
        FloatList* pfl = (*(yyvsp[-2].floatVecList))[i];
        for(int j=0; j<(int)pfl->size(); j++)
            *p++ = (*pfl)[j];
        delete pfl;
    }
    if(szvec == 0)
        yyerror("Error : vectors are 0 sized\n");
    else
        curPass->createState((yyvsp[-9].s)->c_str(), (yyvsp[-7].ival), farray, szvec, (int)(yyvsp[-2].floatVecList)->size());
    delete farray;
    delete (yyvsp[-9].s);
    delete (yyvsp[-2].floatVecList);
    }
#line 5125 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 247:
#line 1795 "Grammar.ypp" /* yacc.c:1646  */
    {
    IntVecList *pvl = (yyvsp[-1].intVecList);
    IntList* pl = (*pvl)[0];
    int* p = &(*pl)[0];
    curPass->createState((yyvsp[-7].s)->c_str(), (yyvsp[-5].ival), p, (int)pl->size());
    delete (yyvsp[-7].s);
    delete pl;
    delete pvl;
    }
#line 5139 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 248:
#line 1804 "Grammar.ypp" /* yacc.c:1646  */
    {
    curPass->createState((yyvsp[-9].s)->c_str(), (yyvsp[-7].ival), &(*(yyvsp[-2].boolList))[0], (int)(yyvsp[-2].boolList)->size());
    delete (yyvsp[-9].s);
    delete (yyvsp[-2].boolList);
    }
#line 5149 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 249:
#line 1809 "Grammar.ypp" /* yacc.c:1646  */
    {
    yyerror("TODO: SHDPROFILE for HLSL to do...\n");
    delete (yyvsp[-1].s);
    }
#line 5158 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 250:
#line 1813 "Grammar.ypp" /* yacc.c:1646  */
    {
    IResource *pRes = curContainer->findResource((yyvsp[-1].s)->c_str());
    if(pRes) {
        IPassState *pS = curPass->findState((yyvsp[-4].s)->c_str(), 0);
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            pU->setTextureResource(pRes);
        }
        else
            pS = curPass->createStateTextureResource((yyvsp[-4].s)->c_str(), pRes);
    }
    else yyerror("Error in sampler resource assignment : cannot find the resource");
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].s);
}
#line 5180 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 251:
#line 1830 "Grammar.ypp" /* yacc.c:1646  */
    {
    IResource *pRes = curContainer->findResource((yyvsp[-1].s)->c_str());
    if(pRes) {
        IPassState *pS = curPass->findStateOverride((yyvsp[-4].s)->c_str());
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            pU->setTextureResource(pRes);
        }
        else
            pS = curPass->createStateTextureResourceOverride((yyvsp[-4].s)->c_str(), pRes);
    }
    else yyerror("Error in sampler resource assignment : cannot find the resource");
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].s);
}
#line 5202 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 252:
#line 1847 "Grammar.ypp" /* yacc.c:1646  */
    {
    IResource *pRes = curContainer->findResource((yyvsp[-1].s)->c_str());
    if(pRes) {
        IPassState *pS = curPass->findState((yyvsp[-4].s)->c_str(), 0);
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            pU->setSamplerResource(pRes, -1);
        }
        else
            pS = curPass->createStateSamplerResource((yyvsp[-4].s)->c_str(), pRes, -1);
    }
    else yyerror("Error in sampler resource assignment : cannot find the resource");
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].s);
}
#line 5224 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 253:
#line 1864 "Grammar.ypp" /* yacc.c:1646  */
    {
    IResource *pRes = curContainer->findResource((yyvsp[-1].s)->c_str());
    if(pRes) {
        IPassState *pS = curPass->findStateOverride((yyvsp[-4].s)->c_str());
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            pU->setSamplerResource(pRes, -1);
        }
        else
            pS = curPass->createStateSamplerResourceOverride((yyvsp[-4].s)->c_str(), pRes, -1);
    }
    else yyerror("Error in sampler resource assignment : cannot find the resource");
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].s);
}
#line 5246 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 254:
#line 1881 "Grammar.ypp" /* yacc.c:1646  */
    {
    IResource *pRes = curContainer->findResource((yyvsp[-1].s)->c_str());
    if(pRes) {
        IPassState *pS = curPass->findState((yyvsp[-6].s)->c_str(), 0);
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            pU->setSamplerResource(pRes, -1);
        }
        else
            pS = curPass->createStateSamplerResource((yyvsp[-6].s)->c_str(), pRes, -1);
    }
    else yyerror("Error in sampler resource assignment : cannot find the resource");
    delete (yyvsp[-6].s);
    delete (yyvsp[-1].s);
}
#line 5268 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 255:
#line 1898 "Grammar.ypp" /* yacc.c:1646  */
    {
    IResource *pRes = curContainer->findResource((yyvsp[-4].s)->c_str());
    if(pRes) {
        IPassState *pS = curPass->findState((yyvsp[-8].s)->c_str(), 0);
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            pU->setSamplerResource(pRes, (yyvsp[-2].ival));
        }
        else
            pS = curPass->createStateSamplerResource((yyvsp[-8].s)->c_str(), pRes, (yyvsp[-2].ival));
    }
    else yyerror("Error in sampler resource assignment : cannot find the resource");
    delete (yyvsp[-8].s);
    delete (yyvsp[-4].s);
}
#line 5290 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 256:
#line 1915 "Grammar.ypp" /* yacc.c:1646  */
    {
    IResource *pRes = curContainer->findResource((yyvsp[-4].s)->c_str());
    if(pRes) {
        IPassState *pS = curPass->findStateOverride((yyvsp[-8].s)->c_str());
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            pU->setSamplerResource(pRes, (yyvsp[-2].ival));
        }
        else
            pS = curPass->createStateSamplerResourceOverride((yyvsp[-8].s)->c_str(), pRes, (yyvsp[-2].ival));
    }
    else yyerror("Error in sampler resource assignment : cannot find the resource");
    delete (yyvsp[-8].s);
    delete (yyvsp[-4].s);
}
#line 5312 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 257:
#line 1932 "Grammar.ypp" /* yacc.c:1646  */
    {
    IResource *pRes = curContainer->findResource((yyvsp[-1].s)->c_str());
    if(pRes) {
        IPassState *pS = curPass->findState((yyvsp[-4].s)->c_str(), 0);
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            pU->setImageResource(pRes, -1);
        }
        else
            pS = curPass->createStateImageResource((yyvsp[-4].s)->c_str(), pRes, -1);
    }
    else yyerror("Error in sampler resource assignment : cannot find the resource");
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].s);
}
#line 5334 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 258:
#line 1949 "Grammar.ypp" /* yacc.c:1646  */
    {
    IResource *pRes = curContainer->findResource((yyvsp[-1].s)->c_str());
    if(pRes) {
        IPassState *pS = curPass->findStateOverride((yyvsp[-4].s)->c_str());
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            pU->setImageResource(pRes, -1);
        }
        else
            pS = curPass->createStateImageResourceOverride((yyvsp[-4].s)->c_str(), pRes, -1);
    }
    else yyerror("Error in sampler resource assignment : cannot find the resource");
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].s);
}
#line 5356 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 259:
#line 1966 "Grammar.ypp" /* yacc.c:1646  */
    {
    IResource *pRes = curContainer->findResource((yyvsp[-1].s)->c_str());
    if(pRes) {
        IPassState *pS = curPass->findState((yyvsp[-6].s)->c_str(), 0);
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            pU->setImageResource(pRes, -1);
        }
        else
            pS = curPass->createStateImageResource((yyvsp[-6].s)->c_str(), pRes, -1);
    }
    else yyerror("Error in sampler resource assignment : cannot find the resource");
    delete (yyvsp[-6].s);
    delete (yyvsp[-1].s);
}
#line 5378 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 260:
#line 1983 "Grammar.ypp" /* yacc.c:1646  */
    {
    IResource *pRes = curContainer->findResource((yyvsp[-4].s)->c_str());
    if(pRes) {
        IPassState *pS = curPass->findState((yyvsp[-8].s)->c_str(), 0);
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            pU->setImageResource(pRes, (yyvsp[-2].ival));
        }
        else
            pS = curPass->createStateImageResource((yyvsp[-8].s)->c_str(), pRes, (yyvsp[-2].ival));
    }
    else yyerror("Error in sampler resource assignment : cannot find the resource");
    delete (yyvsp[-8].s);
    delete (yyvsp[-4].s);
}
#line 5400 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 261:
#line 2000 "Grammar.ypp" /* yacc.c:1646  */
    {
    IPassState *pS = curPass->findState((yyvsp[-4].s)->c_str(), 0);
    if(pS)
    {
        IUniform *pU;
        pU = pS->getUniform();
        if(pU)
            pU->setSamplerUnit((yyvsp[-1].ival));
    }
    else
        pS = curPass->createStateSamplerResource((yyvsp[-4].s)->c_str(), (IResource*)NULL, (yyvsp[-1].ival));
    delete (yyvsp[-4].s);
}
#line 5418 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 262:
#line 2013 "Grammar.ypp" /* yacc.c:1646  */
    {
    IPassState *pS = curPass->findStateOverride((yyvsp[-4].s)->c_str());
    if(pS)
    {
        IUniform *pU;
        pU = pS->getUniform();
        if(pU)
            pU->setSamplerUnit((yyvsp[-1].ival));
    }
    else
        pS = curPass->createStateSamplerResourceOverride((yyvsp[-4].s)->c_str(), (IResource*)NULL, (yyvsp[-1].ival));
    delete (yyvsp[-4].s);
}
#line 5436 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 263:
#line 2026 "Grammar.ypp" /* yacc.c:1646  */
    {
    IPassState *pS = curPass->findState((yyvsp[-4].s)->c_str(), 0);
    if(pS)
    {
        IUniform *pU;
        pU = pS->getUniform();
        if(pU)
            pU->setImageUnit((yyvsp[-1].ival));
    }
    else
        pS = curPass->createStateSamplerResource((yyvsp[-4].s)->c_str(), (IResource*)NULL, (yyvsp[-1].ival));
    delete (yyvsp[-4].s);
}
#line 5454 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 264:
#line 2039 "Grammar.ypp" /* yacc.c:1646  */
    {
    IPassState *pS = curPass->findStateOverride((yyvsp[-4].s)->c_str());
    if(pS)
    {
        IUniform *pU;
        pU = pS->getUniform();
        if(pU)
            pU->setImageUnit((yyvsp[-1].ival));
    }
    else
        pS = curPass->createStateSamplerResourceOverride((yyvsp[-4].s)->c_str(), (IResource*)NULL, (yyvsp[-1].ival));
    delete (yyvsp[-4].s);
}
#line 5472 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 265:
#line 2052 "Grammar.ypp" /* yacc.c:1646  */
    {
    IPassState *pS = curPass->findState((yyvsp[-6].s)->c_str());
    if(pS)
    {
        IUniform *pU;
        pU = pS->getUniform();
        if(pU)
            pU->setSamplerUnit((yyvsp[-1].ival));
    }
    else
        pS = curPass->createStateSamplerResource((yyvsp[-6].s)->c_str(), (IResource*)NULL, (yyvsp[-1].ival));
    delete (yyvsp[-6].s);
}
#line 5490 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 266:
#line 2065 "Grammar.ypp" /* yacc.c:1646  */
    {
    IPassState *pS = curPass->findState((yyvsp[-6].s)->c_str());
    if(pS)
    {
        IUniform *pU;
        pU = pS->getUniform();
        if(pU)
            pU->setImageUnit((yyvsp[-1].ival));
    }
    else
        pS = curPass->createStateSamplerResource((yyvsp[-6].s)->c_str(), (IResource*)NULL, (yyvsp[-1].ival));
    delete (yyvsp[-6].s);
}
#line 5508 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 267:
#line 2078 "Grammar.ypp" /* yacc.c:1646  */
    {
    IPassState *pS = curPass->findState((yyvsp[-4].s)->c_str(), 0);
    ISamplerState * pState = curContainer->findSamplerState((yyvsp[-1].s)->c_str());
    if(!pState)
        yyerror("Error: sampler-state referenced not found\n");
    else {
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            if(pU && pState)
                pU->setSamplerState(pState);
            if(!pU)
                yyerror("Error: IUniform parameter not found\n");
        } else {
            pS = curPass->createStateSamplerState((yyvsp[-4].s)->c_str(), pState);
        }
    }
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].s);
}
#line 5534 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 268:
#line 2099 "Grammar.ypp" /* yacc.c:1646  */
    {
    IPassState *pS = curPass->findStateOverride((yyvsp[-4].s)->c_str());
    ISamplerState * pState = curContainer->findSamplerState((yyvsp[-1].s)->c_str());
    if(!pState)
        yyerror("Error: sampler-state referenced not found\n");
    else {
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            if(pU && pState)
                pU->setSamplerState(pState);
            if(!pU)
                yyerror("Error: IUniform parameter not found\n");
        } else {
            pS = curPass->createStateSamplerStateOverride((yyvsp[-4].s)->c_str(), pState);
        }
    }
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].s);
}
#line 5560 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 269:
#line 2120 "Grammar.ypp" /* yacc.c:1646  */
    { // same as above but different way to write it
    IPassState *pS = curPass->findState((yyvsp[-6].s)->c_str());
    ISamplerState * pState = curContainer->findSamplerState((yyvsp[-1].s)->c_str());
    if(!pState)
        yyerror("Error: sampler-state referenced not found\n");
    else {
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            if(pU && pState)
                pU->setSamplerState(pState);
            if(!pU)
                yyerror("Error: IUniform parameter not found\n");
        } else {
            pS = curPass->createStateSamplerState((yyvsp[-6].s)->c_str(), pState);
        }
    }
    delete (yyvsp[-6].s);
    delete (yyvsp[-1].s);
}
#line 5586 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 270:
#line 2141 "Grammar.ypp" /* yacc.c:1646  */
    { // same as above but different way to write it
    IStateGroupDST* pSG = curContainer->findStateGroupDST((yyvsp[-1].s)->c_str());
    if(!pSG)
        yyerror("Error: State group not found\n");
    else {
        // TODO: shall we check if a previous state group was assigned ? And rais an error/warning ?
        curPass->createState(pSG);
    }
    delete (yyvsp[-1].s);
}
#line 5601 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 271:
#line 2151 "Grammar.ypp" /* yacc.c:1646  */
    { // same as above but different way to write it
    IStateGroupDST* pSG = curContainer->findStateGroupDST((yyvsp[-2].s)->c_str());
    if(!pSG)
        yyerror("Error: State group not found\n");
    else {
        // TODO: shall we check if a previous state group was assigned ? And rais an error/warning ?
        curPass->createState(pSG);
    }
    delete (yyvsp[-2].s);
}
#line 5616 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 272:
#line 2161 "Grammar.ypp" /* yacc.c:1646  */
    { // same as above but different way to write it
    IStateGroupRaster* pSG = curContainer->findStateGroupRaster((yyvsp[-1].s)->c_str());
    if(!pSG)
        yyerror("Error: State group not found\n");
    else {
        // TODO: shall we check if a previous state group was assigned ? And rais an error/warning ?
        curPass->createState(pSG);
    }
    delete (yyvsp[-1].s);
}
#line 5631 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 273:
#line 2171 "Grammar.ypp" /* yacc.c:1646  */
    { // same as above but different way to write it
    IStateGroupRaster* pSG = curContainer->findStateGroupRaster((yyvsp[-2].s)->c_str());
    if(!pSG)
        yyerror("Error: State group not found\n");
    else {
        // TODO: shall we check if a previous state group was assigned ? And rais an error/warning ?
        curPass->createState(pSG);
    }
    delete (yyvsp[-2].s);
}
#line 5646 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 274:
#line 2181 "Grammar.ypp" /* yacc.c:1646  */
    { // same as above but different way to write it
    IStateGroupCS* pSG = curContainer->findStateGroupCS((yyvsp[-1].s)->c_str());
    if(!pSG)
        yyerror("Error: State group not found\n");
    else {
        // TODO: shall we check if a previous state group was assigned ? And rais an error/warning ?
        curPass->createState(pSG);
    }
    delete (yyvsp[-1].s);
}
#line 5661 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 275:
#line 2191 "Grammar.ypp" /* yacc.c:1646  */
    { // same as above but different way to write it
    IStateGroupCS* pSG = curContainer->findStateGroupCS((yyvsp[-2].s)->c_str());
    if(!pSG)
        yyerror("Error: State group not found\n");
    else {
        // TODO: shall we check if a previous state group was assigned ? And rais an error/warning ?
        curPass->createState(pSG);
    }
    delete (yyvsp[-2].s);
}
#line 5676 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 276:
#line 2204 "Grammar.ypp" /* yacc.c:1646  */
    {
    curPass->createStateRenderMode((yyvsp[-1].render_mode));
}
#line 5684 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 277:
#line 2207 "Grammar.ypp" /* yacc.c:1646  */
    {
    curPass->createStateRenderGroup((yyvsp[-1].ival));
}
#line 5692 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 278:
#line 2210 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((*(yyvsp[-1].s)) == "BackBuffer")
        curPass->createStateBlitFBOToActiveTarget(NULL);
    else {
        IFrameBufferObject* pFBO = curContainer->findFbo((yyvsp[-1].s)->c_str());
        if(!pFBO) {
            yyerror("Error: could not find the Frame Buffer Object");
            yyerror((yyvsp[-1].s)->c_str());
        } else {
            curPass->createStateBlitFBOToActiveTarget(pFBO);
        }
    }
    delete (yyvsp[-1].s)
}
#line 5711 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 279:
#line 2224 "Grammar.ypp" /* yacc.c:1646  */
    {
    curPass->createStateClearMode((yyvsp[-1].clear_mode));
}
#line 5719 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 280:
#line 2227 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((*(yyvsp[-1].s)) == "backbuffer")
        curPass->createStateRenderTarget(NULL);
    else {
        IFrameBufferObject* pFBO = curContainer->findFbo((yyvsp[-1].s)->c_str());
        if(!pFBO) {
            yyerror("Error: could not find the Frame Buffer Object");
            yyerror((yyvsp[-1].s)->c_str());
        } else {
            curPass->createStateRenderTarget(pFBO);
        }
    }
    delete (yyvsp[-1].s);
}
#line 5738 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 281:
#line 2241 "Grammar.ypp" /* yacc.c:1646  */
    {
    FloatVecList *pfvl = (yyvsp[-1].floatVecList);
    FloatList* pfl = (*pfvl)[0];
    float* pf = &(*pfl)[0];
    if((yyvsp[-4].ival) < 4)
        curPass->createStateClearColor((yyvsp[-4].ival), (*pfl)[0], (*pfl)[1], (*pfl)[2], (*pfl)[3]);
    else
        yyerror("ClearColor[n] can only clear up to 4 targets\n");
    delete (yyvsp[-1].floatVecList);
}
#line 5753 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 282:
#line 2251 "Grammar.ypp" /* yacc.c:1646  */
    {
    curPass->createStateClearColor(0, (*(yyvsp[-2].floatList))[0], (*(yyvsp[-2].floatList))[1], (*(yyvsp[-2].floatList))[2], (*(yyvsp[-2].floatList))[3]);
    delete (yyvsp[-2].floatList);
}
#line 5762 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 283:
#line 2255 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[-2].intList)->size() >= 4)
        curPass->createStateViewport((*(yyvsp[-2].intList))[0], (*(yyvsp[-2].intList))[1], (*(yyvsp[-2].intList))[2], (*(yyvsp[-2].intList))[3], ((yyvsp[-2].intList)->size() >= 5)?(*(yyvsp[-2].intList))[4]:0.0f, ((yyvsp[-2].intList)->size() >= 6)?(*(yyvsp[-2].intList))[5]:0.0f);
    else
        yyerror("Viewport needs a 4 or 6 components vector\n");
    delete (yyvsp[-2].intList);
}
#line 5774 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 284:
#line 2262 "Grammar.ypp" /* yacc.c:1646  */
    {
    if(((*(yyvsp[-4].s)) == "backbuffer")||((*(yyvsp[-2].s)) == "backbuffer")) {
            yyerror("Error: cannot swap resources with backbuffer target");
    } else {
        IFrameBufferObject* pFBO1 = curContainer->findFbo((yyvsp[-4].s)->c_str());
        if(!pFBO1) {
            yyerror("Error: could not find the Frame Buffer Object");
            yyerror((yyvsp[-4].s)->c_str());
        } else {
            IFrameBufferObject* pFBO2 = curContainer->findFbo((yyvsp[-2].s)->c_str());
            if(!pFBO2) {
                yyerror("Error: could not find the Frame Buffer Object");
                yyerror((yyvsp[-2].s)->c_str());
            } else {
                curPass->createStateSwapResources(pFBO1, pFBO2);
            }
        }
    }
    delete (yyvsp[-4].s);
    delete (yyvsp[-2].s);
}
#line 5800 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 285:
#line 2283 "Grammar.ypp" /* yacc.c:1646  */
    {
    curPass->setLoopCount((yyvsp[-1].ival));
}
#line 5808 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 286:
#line 2286 "Grammar.ypp" /* yacc.c:1646  */
    {
    curPass->setActive((yyvsp[-1].bval));
}
#line 5816 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 287:
#line 2292 "Grammar.ypp" /* yacc.c:1646  */
    {
    LOGD("computeGroups\n");
#ifdef USEGLSLCOMPUTE
    if((yyvsp[-2].intList)->size() < 2)
        yyerror("Grid size must have 2 or 3 values\n");
    else
        curPass->createStateComputeGroup((*(yyvsp[-2].intList))[0], (*(yyvsp[-2].intList))[1], (yyvsp[-2].intList)->size() > 2 ? (*(yyvsp[-2].intList))[2] : 1);
#else
    yyerror("Compute Not supported in this build\n");
#endif
    delete (yyvsp[-2].intList);
}
#line 5833 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 288:
#line 2307 "Grammar.ypp" /* yacc.c:1646  */
    {
    LOGD("CUDAMODULE\n");
#ifdef USECUDA
    if((yyvsp[-1].shaderList)->size() > 0)
        curPass->createState(IPassState::TCUDAModule, & (*(yyvsp[-1].shaderList))[0], (int)(yyvsp[-1].shaderList)->size());
    delete (yyvsp[-1].shaderList);
#else
    LOGI("CUDA Not supported in this build\n");
#endif
}
#line 5848 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 289:
#line 2317 "Grammar.ypp" /* yacc.c:1646  */
    {
    LOGD("CUDAKERNEL\n");
#ifdef USECUDA
    // CUDAMODULE gathered pieces of CUDA code. We expect some kernels to be in there
    // let's find the named kernel and check if the arguments are matching the assignements, here
    bool bRes = true;
    IShader* pShd = curContainer->findShader((yyvsp[-4].s)->c_str());
    if(pShd)
    {
        IShaderEx* pShdEx = pShd->getExInterface();
        for(int i=0; i<(int)(yyvsp[-2].argValList)->size(); i++)
        {
            // get the argument of this 
            IShaderEx::Argument arg = pShdEx->getArgument(i);
            // compare if the one from the pass-state is compatible
            if(checkArgCompatibility((*(yyvsp[-2].argValList))[i], arg) == false)
            {
                bRes = false;
                yyerror("Argument values don't match the Kernel's argument!\n");
                break;
            }
        }
        if(bRes) {
            ArgVal* vals = NULL;
            if((yyvsp[-2].argValList)->size() > 0) {
                vals = new ArgVal[(yyvsp[-2].argValList)->size()];
                for(int i=0; i<(int)(yyvsp[-2].argValList)->size(); i++)
                    vals[i] = *((*(yyvsp[-2].argValList))[i]);
            }
            curPass->createStateKernelEntry((yyvsp[-4].s)->c_str(), (int)(yyvsp[-2].argValList)->size(), vals);
            // we dont "delete [] vals;" because this pointer got passed to the pass-state and will be passed
            // down to the kernel execution.
        }
    } else {
        yyerror("CUDAKernel name Not found !\n");
    }
    for(int i=0; i<(int)(yyvsp[-2].argValList)->size(); i++)
        delete (*(yyvsp[-2].argValList))[i]; // free after done using it
#else
    LOGI("CUDA Not supported in this build\n");
    for(int i=0; i<(int)(yyvsp[-2].argValList)->size(); i++)
        delete (*(yyvsp[-2].argValList))[i];
#endif
    delete (yyvsp[-2].argValList);
    delete (yyvsp[-4].s);
}
#line 5899 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 290:
#line 2363 "Grammar.ypp" /* yacc.c:1646  */
    {
    LOGD("CUDASHMEM\n");
#ifdef USECUDA
    curPass->createStateSharedMemory((yyvsp[-1].ival))
#else
    LOGI("CUDA Not supported in this build\n");
#endif
}
#line 5912 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 291:
#line 2371 "Grammar.ypp" /* yacc.c:1646  */
    {
    LOGD("CUDAGRID\n");
#ifdef USECUDA
    IntVecList* pIVL = (yyvsp[-1].intVecList);
    IntList* pIL = pIVL->size() >=1 ? (*pIVL)[0]:NULL;
    if(pIL && (pIL->size()>=2))
    {
        if(pIL->size() < 2)
            yyerror("Grid size must have 2 or 3 values\n");
        curPass->createStateGridSz((*pIL)[0], (*pIL)[1], pIL->size() > 2 ? (*pIL)[2] : 1);
    } else
        yyerror("Error in cuda grid setup\n");
#else
    LOGI("CUDA Not supported in this build\n");
#endif
    delete (yyvsp[-1].intVecList);
}
#line 5934 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 292:
#line 2388 "Grammar.ypp" /* yacc.c:1646  */
    {
    LOGD("CUDABLOCK\n");
#ifdef USECUDA
    IntVecList* pIVL = (yyvsp[-1].intVecList);
    IntList* pIL = pIVL->size() >=1 ? (*pIVL)[0]:NULL;
    if(pIL && (pIL->size()>=2))
    {
        if(pIL->size() < 2)
            yyerror("Grid size must have 2 values\n");
        curPass->createStateBlockSz((*pIL)[0], (*pIL)[1], pIL->size()>2 ? (*pIL)[2] : 1);
    } else
        yyerror("Error in cuda grid setup\n");
#else
    LOGI("CUDA Not supported in this build\n");
#endif
    delete (yyvsp[-1].intVecList);
}
#line 5956 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 293:
#line 2408 "Grammar.ypp" /* yacc.c:1646  */
    {
    curPass->bindAttribute((yyvsp[-4].s)->c_str(), (yyvsp[-1].ival));
    delete (yyvsp[-4].s);
}
#line 5965 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 294:
#line 2412 "Grammar.ypp" /* yacc.c:1646  */
    { 
    yyerror("Error in a state of the Pass\n");
}
#line 5973 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 295:
#line 2421 "Grammar.ypp" /* yacc.c:1646  */
    {
    //printf("Creation of PASS\n");
    curPass = curTechnique->addPass()->getExInterface();
    curAnnotations = curPass->annotations()->getExInterface();
    }
#line 5983 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 296:
#line 2426 "Grammar.ypp" /* yacc.c:1646  */
    {
    LOGI("Pass with no name...\n");
    curAnnotations = NULL;
    curRState = NULL;
    curCSState = NULL;
    curDSTState = NULL;
    curPRState = NULL;
    lex_pop_state();
}
#line 5997 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 297:
#line 2435 "Grammar.ypp" /* yacc.c:1646  */
    {
    //printf("Creation of PASS %s\n", $2->c_str());
    curPass = curTechnique->addPass((yyvsp[0].s)->c_str())->getExInterface();
    curAnnotations = curPass->annotations()->getExInterface();
    }
#line 6007 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 298:
#line 2440 "Grammar.ypp" /* yacc.c:1646  */
    {
    LOGD("Pass %s...\n", (yyvsp[-5].s)->c_str() );
    delete (yyvsp[-5].s);
    curAnnotations = NULL;
    curRState = NULL;
    curCSState = NULL;
    curDSTState = NULL;
    curPRState = NULL;
    lex_pop_state();
}
#line 6022 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 299:
#line 2453 "Grammar.ypp" /* yacc.c:1646  */
    {
    curTechnique->bindAttribute((yyvsp[-4].s)->c_str(), (yyvsp[-1].ival));
    delete (yyvsp[-4].s);
}
#line 6031 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 300:
#line 2457 "Grammar.ypp" /* yacc.c:1646  */
    {
    curTechnique->bindAttribute((yyvsp[-4].s)->c_str(), (yyvsp[-1].ival));
    delete (yyvsp[-4].s);
}
#line 6040 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 301:
#line 2461 "Grammar.ypp" /* yacc.c:1646  */
    {
    ITechnique *pTech = curContainer->findTechnique((yyvsp[-1].s)->c_str());
    if(pTech == NULL)
    {
        yyerror("sub-technique not found\n");
    } else if(!curTechnique->addSubTechnique(pTech))
    {
        yyerror("Error in adding a sub-technique\n");
    }
    delete (yyvsp[-1].s);
}
#line 6056 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 302:
#line 2472 "Grammar.ypp" /* yacc.c:1646  */
    {
    ITechnique *pTech = curContainer->findTechnique((yyvsp[-2].s)->c_str());
    if(pTech == NULL)
    {
        yyerror("sub-technique not found\n");
    } else if(!curTechnique->addSubTechnique(pTech, false))
    {
        yyerror("Error in adding a sub-technique\n");
    }
    delete (yyvsp[-2].s);
}
#line 6072 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 305:
#line 2492 "Grammar.ypp" /* yacc.c:1646  */
    { yyerror("Error in Pass list\n");}
#line 6078 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 306:
#line 2498 "Grammar.ypp" /* yacc.c:1646  */
    {
    //printf("Creation of Technique for NO name\n");
    curTechnique = curContainer->createTechnique()->getExInterface();
    curAnnotations = curTechnique->annotations()->getExInterface();
}
#line 6088 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 307:
#line 2502 "Grammar.ypp" /* yacc.c:1646  */
    { 
    curAnnotations = NULL;
    lex_pop_state();
}
#line 6097 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 308:
#line 2506 "Grammar.ypp" /* yacc.c:1646  */
    {
    LOGD("creation of Technique %s...\n", (yyvsp[0].s)->c_str() );
    curTechnique = curContainer->createTechnique((yyvsp[0].s)->c_str())->getExInterface();
    curAnnotations = curTechnique->annotations()->getExInterface();
    delete (yyvsp[0].s);
}
#line 6108 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 309:
#line 2511 "Grammar.ypp" /* yacc.c:1646  */
    { 
    lex_pop_state();
    curAnnotations = NULL;
}
#line 6117 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 310:
#line 2521 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.bval)=false; }
#line 6123 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 311:
#line 2522 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.bval)=true; }
#line 6129 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 312:
#line 2527 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.bval)=true; }
#line 6135 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 313:
#line 2528 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.bval)=false; }
#line 6141 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 314:
#line 2529 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.bval)=true; }
#line 6147 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 315:
#line 2537 "Grammar.ypp" /* yacc.c:1646  */
    {
    if(!namespaceName.empty())
        (*(yyvsp[0].s)) = namespaceName + std::string("_NS_") + (*(yyvsp[0].s));
    (yyval.s) = (yyvsp[0].s);
}
#line 6157 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 316:
#line 2546 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.global_varname_annots).global = false; (yyval.global_varname_annots).varName = NULL; }
#line 6163 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 317:
#line 2547 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.global_varname_annots).global = false; (yyval.global_varname_annots).varName = (yyvsp[-1].s); }
#line 6169 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 318:
#line 2549 "Grammar.ypp" /* yacc.c:1646  */
    { 
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
    (yyval.global_varname_annots).global = (yyvsp[-2].bval); 
    (yyval.global_varname_annots).varName = (yyvsp[-1].s);
}
#line 6181 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 319:
#line 2561 "Grammar.ypp" /* yacc.c:1646  */
    {
    Sglobal_varname &gva = (yyvsp[-2].global_varname_annots);
    LOGD("GLSLShader %s...\n", gva.varName ? gva.varName->c_str() : "Un-named");
    IShader* pShader = curContainer->createGLSLShader(gva.varName ? gva.varName->c_str() : NULL, gva.global);
    if(pShader)
    {
        curShader = pShader->getExInterface();
        // Now we copy the possible annotations that were filled in arraysz_semantic_annotations_uniformvalueassigment
        curShader->annotations()->getExInterface()->clear();
        curShader->annotations()->getExInterface()->copy(curAnnotations);

        if(curShader->getShaderCodeSz() == 0) // ==0 means it needs to have some code loaded now
            curShader->loadShaderCode((yyvsp[0].s)->c_str(), line_num, file_name.c_str() );
        else
            LOGI("Skipping the shader code of %s : some is already loaded\n", gva.varName ? gva.varName->c_str() : "Un-named");
    }
    else yyerror("Failed in creating shader");
    delete gva.varName;
    delete (yyvsp[0].s);
}
#line 6206 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 320:
#line 2585 "Grammar.ypp" /* yacc.c:1646  */
    {
    LOGD("SVG Path %s...\n", (yyvsp[-2].global_varname_annots).varName ? (yyvsp[-2].global_varname_annots).varName->c_str() : "Un-named" );
    if(curShader = curContainer->createPathShader((yyvsp[-2].global_varname_annots).varName ? (yyvsp[-2].global_varname_annots).varName->c_str() : NULL, (yyvsp[-2].global_varname_annots).global)->getExInterface())
    {
        // Now we copy the possible annotations that were filled in arraysz_semantic_annotations_uniformvalueassigment
        curShader->annotations()->getExInterface()->clear();
        curShader->annotations()->getExInterface()->copy(curAnnotations);

        if(curShader->getShaderCodeSz() == 0) // ==0 means it needs to have some code loaded now
            curShader->loadShaderCode((yyvsp[0].s)->c_str(), line_num, file_name.c_str() );
        else
            LOGI("Skipping the shader code of %s : some is already loaded\n", (yyvsp[-2].global_varname_annots).varName ? (yyvsp[-2].global_varname_annots).varName->c_str() : "Un-named");
    }
    else yyerror("Failed in creating shader");
    delete (yyvsp[-2].global_varname_annots).varName;
    delete (yyvsp[0].s);
}
#line 6228 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 321:
#line 2606 "Grammar.ypp" /* yacc.c:1646  */
    {
    LOGD("PS Path %s...\n", (yyvsp[-2].global_varname_annots).varName ? (yyvsp[-2].global_varname_annots).varName->c_str() : "Un-named" );
    if(curShader = curContainer->createPathShader((yyvsp[-2].global_varname_annots).varName ? (yyvsp[-2].global_varname_annots).varName->c_str() : NULL, (yyvsp[-2].global_varname_annots).global, true)->getExInterface())
    {
        // Now we copy the possible annotations that were filled in arraysz_semantic_annotations_uniformvalueassigment
        curShader->annotations()->getExInterface()->clear();
        curShader->annotations()->getExInterface()->copy(curAnnotations);

        if(curShader->getShaderCodeSz() == 0) // ==0 means it needs to have some code loaded now
            curShader->loadShaderCode((yyvsp[0].s)->c_str(), line_num, file_name.c_str() );
        else
            LOGI("Skipping the shader code of %s : some is already loaded\n", (yyvsp[-2].global_varname_annots).varName ? (yyvsp[-2].global_varname_annots).varName->c_str() : "Un-named");
    }
    else yyerror("Failed in creating shader");
    delete (yyvsp[-2].global_varname_annots).varName;
    delete (yyvsp[0].s);
}
#line 6250 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 322:
#line 2627 "Grammar.ypp" /* yacc.c:1646  */
    {
    LOGD("HLSL11Shader %s...\n", (yyvsp[-2].global_varname_annots).varName ? (yyvsp[-2].global_varname_annots).varName->c_str() : "Un-named" );
    if((yyvsp[-2].global_varname_annots).global) { // if we need to make this shader GLOBAL
        if(curShader = pShdRep->findShader((yyvsp[-2].global_varname_annots).varName ? (yyvsp[-2].global_varname_annots).varName->c_str() : NULL)->getExInterface())
        {
            LOGI("    Warning : %s is already global. Skipping the declaration...\n", (yyvsp[-2].global_varname_annots).varName ? (yyvsp[-2].global_varname_annots).varName->c_str() : "Un-named" );
            if(curContainer->addHLSL11Shader(curShader))
                pShdRep->addShader(curShader);
            else
                yyerror("Failed in adding shader to the effect container");
        } else {
            if(curShader = curContainer->createHLSL11Shader((yyvsp[-2].global_varname_annots).varName ? (yyvsp[-2].global_varname_annots).varName->c_str() : NULL)->getExInterface()) {
                // Now we copy the possible annotations that were filled in arraysz_semantic_annotations_uniformvalueassigment
                curShader->annotations()->getExInterface()->clear();
                curShader->annotations()->getExInterface()->copy(curAnnotations);

                curShader->loadShaderCode((yyvsp[0].s)->c_str(), line_num, file_name.c_str() );
                pShdRep->addShader(curShader);
            } else
                yyerror("Failed in creating shader");
        }
    } else {
        if(curShader = curContainer->createHLSL11Shader((yyvsp[-2].global_varname_annots).varName ? (yyvsp[-2].global_varname_annots).varName->c_str() : NULL)->getExInterface())
        {
            // Now we copy the possible annotations that were filled in arraysz_semantic_annotations_uniformvalueassigment
            curShader->annotations()->getExInterface()->clear();
            curShader->annotations()->getExInterface()->copy(curAnnotations);

            curShader->loadShaderCode((yyvsp[0].s)->c_str(), line_num, file_name.c_str() );
        } else yyerror("Failed in creating shader");
    }
    delete (yyvsp[-2].global_varname_annots).varName;
    delete (yyvsp[0].s);
}
#line 6289 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 323:
#line 2665 "Grammar.ypp" /* yacc.c:1646  */
    {
    LOGD("HLSL10Shader %s...\n", (yyvsp[-2].global_varname_annots).varName ? (yyvsp[-2].global_varname_annots).varName->c_str() : "Un-named" );
    if((yyvsp[-2].global_varname_annots).global) { // if we need to make this shader GLOBAL
        if(curShader = pShdRep->findShader((yyvsp[-2].global_varname_annots).varName ? (yyvsp[-2].global_varname_annots).varName->c_str() : NULL)->getExInterface())
        {
            LOGI("    Warning : %s is already global. Skipping the declaration...\n", (yyvsp[-2].global_varname_annots).varName ? (yyvsp[-2].global_varname_annots).varName->c_str() : "Un-named" );
            if(curContainer->addHLSL10Shader(curShader))
                pShdRep->addShader(curShader);
            else
                yyerror("Failed in adding shader to the effect container");
        } else {
            if(curShader = curContainer->createHLSL10Shader((yyvsp[-2].global_varname_annots).varName ? (yyvsp[-2].global_varname_annots).varName->c_str() : NULL)->getExInterface()) {
                // Now we copy the possible annotations that were filled in arraysz_semantic_annotations_uniformvalueassigment
                curShader->annotations()->getExInterface()->clear();
                curShader->annotations()->getExInterface()->copy(curAnnotations);

                curShader->loadShaderCode((yyvsp[0].s)->c_str(), line_num, file_name.c_str() );
                pShdRep->addShader(curShader);
            } else
                yyerror("Failed in creating shader");
        }
    } else {
        if(curShader = curContainer->createHLSL10Shader((yyvsp[-2].global_varname_annots).varName ? (yyvsp[-2].global_varname_annots).varName->c_str() : NULL)->getExInterface())
        {
            // Now we copy the possible annotations that were filled in arraysz_semantic_annotations_uniformvalueassigment
            curShader->annotations()->getExInterface()->clear();
            curShader->annotations()->getExInterface()->copy(curAnnotations);

            curShader->loadShaderCode((yyvsp[0].s)->c_str(), line_num, file_name.c_str() );
        } else yyerror("Failed in creating shader");
    }
    delete (yyvsp[-2].global_varname_annots).varName;
    delete (yyvsp[0].s);
}
#line 6328 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 324:
#line 2703 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.bval) = false; }
#line 6334 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 325:
#line 2704 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.bval) = true; }
#line 6340 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 326:
#line 2706 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.bval) = false; }
#line 6346 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 327:
#line 2707 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.bval) = true; }
#line 6352 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 328:
#line 2712 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.argument) = new Argument((yyvsp[-1].bval) ? nvFX::TFloatPtr : nvFX::TFloat, (yyvsp[0].s));
    LOGD("Argument %s %s\n", (yyvsp[-1].bval) ? "TFloatPtr" : "TFloat", (yyvsp[0].s)->c_str());
    delete (yyvsp[0].s);
}
#line 6362 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 329:
#line 2717 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.argument) = new Argument((yyvsp[-3].bval) ? ((yyvsp[-1].bval) ? nvFX::TUIntPtr : nvFX::TUInt) : ((yyvsp[-1].bval) ? nvFX::TIntPtr : nvFX::TInt), (yyvsp[0].s));
    LOGD("Argument %s %s\n", (yyvsp[-3].bval) ? ((yyvsp[-1].bval) ? "TUIntPtr" : "TUInt") : ((yyvsp[-1].bval) ? "TIntPtr" : "TInt"), (yyvsp[0].s)->c_str() );
    delete (yyvsp[0].s);
}
#line 6372 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 330:
#line 2726 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.argList) = new ArgList; }
#line 6378 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 331:
#line 2727 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.argList) = new ArgList; }
#line 6384 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 332:
#line 2728 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.argList) = new ArgList;
    (yyval.argList)->push_back((yyvsp[0].argument));
    }
#line 6393 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 333:
#line 2732 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.argList) = (yyvsp[-2].argList);
    (yyval.argList)->push_back((yyvsp[0].argument));
    }
#line 6402 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 334:
#line 2740 "Grammar.ypp" /* yacc.c:1646  */
    {
    LOGD("CUDAShader %s...\n", (yyvsp[-5].global_varname_annots).varName ? (yyvsp[-5].global_varname_annots).varName->c_str() : "Un-named" );
#ifdef USECUDA
    bool bNew = true;
    if((yyvsp[-5].global_varname_annots).global) { // if we need to make this shader GLOBAL
        if(curShader = pShdRep->findShader((yyvsp[-5].global_varname_annots).varName ? (yyvsp[-5].global_varname_annots).varName->c_str() : NULL)->getExInterface())
        {
            LOGI("    Warning : %s is already global. Skipping the declaration...\n", (yyvsp[-5].global_varname_annots).varName ? (yyvsp[-5].global_varname_annots).varName->c_str() : NULL );
            bNew = false;
            if(curContainer->addCUDAShader(curShader))
                pShdRep->addShader(curShader);
            else
                yyerror("Failed in adding shader to the effect container");
        } else {
            if(curShader = curContainer->createCUDAShader((yyvsp[-5].global_varname_annots).varName ? (yyvsp[-5].global_varname_annots).varName->c_str() : NULL)->getExInterface()) {
                // Now we copy the possible annotations that were filled in arraysz_semantic_annotations_uniformvalueassigment
                curShader->annotations()->getExInterface()->clear();
                curShader->annotations()->getExInterface()->copy(curAnnotations);

                curShader->loadShaderCode((yyvsp[0].s)->c_str(), line_num, file_name.c_str() );
                pShdRep->addShader(curShader);
            } else
                yyerror("Failed in creating shader");
        }
    } else {
        if(curShader = curContainer->createCUDAShader((yyvsp[-5].global_varname_annots).varName ? (yyvsp[-5].global_varname_annots).varName->c_str() : NULL)->getExInterface())
        {
            // Now we copy the possible annotations that were filled in arraysz_semantic_annotations_uniformvalueassigment
            curShader->annotations()->getExInterface()->clear();
            curShader->annotations()->getExInterface()->copy(curAnnotations);

            curShader->loadShaderCode((yyvsp[0].s)->c_str(), line_num, file_name.c_str() );
        } else yyerror("Failed in creating shader");
    }
    // Now take care of the arguments
    if(bNew)
    {
        IShaderEx::Argument *args = new IShaderEx::Argument[(yyvsp[-3].argList)->size()];
        for(int i=0; i< (int)(yyvsp[-3].argList)->size(); i++)
        {
            args[i].t = (*(yyvsp[-3].argList))[i]->type;
            args[i].argName = (*(yyvsp[-3].argList))[i]->name.c_str();
        }
        // send the list of arguments
        curShader->setAsKernel((int)(yyvsp[-3].argList)->size(), args);
        delete args;
        for(int i=0; i< (int)(yyvsp[-3].argList)->size(); i++)
            delete (*(yyvsp[-3].argList))[i]; // done... delete
    }
#else
    LOGI("CUDA Not supported in this build\n");
#endif
    delete (yyvsp[-5].global_varname_annots).varName;
    delete (yyvsp[-3].argList);
    delete (yyvsp[0].s);
}
#line 6463 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 335:
#line 2800 "Grammar.ypp" /* yacc.c:1646  */
    {
    LOGD("CUDAShader %s...\n", (yyvsp[-2].global_varname_annots).varName ? (yyvsp[-2].global_varname_annots).varName->c_str() : "Un-named" );
#ifdef USECUDA
    if((yyvsp[-2].global_varname_annots).global) { // if we need to make this shader GLOBAL
        if(curShader = pShdRep->findShader((yyvsp[-2].global_varname_annots).varName ? (yyvsp[-2].global_varname_annots).varName->c_str() : NULL)->getExInterface())
        {
            LOGI("    Warning : %s is already global. Skipping the declaration...\n", (yyvsp[-2].global_varname_annots).varName ? (yyvsp[-2].global_varname_annots).varName->c_str() : "Un-named" );
            if(curContainer->addCUDAShader(curShader))
                pShdRep->addShader(curShader);
            else
                yyerror("Failed in adding shader to the effect container");
        } else {
            if(curShader = curContainer->createCUDAShader((yyvsp[-2].global_varname_annots).varName ? (yyvsp[-2].global_varname_annots).varName->c_str() : NULL)->getExInterface()) {
                // Now we copy the possible annotations that were filled in arraysz_semantic_annotations_uniformvalueassigment
                curShader->annotations()->getExInterface()->clear();
                curShader->annotations()->getExInterface()->copy(curAnnotations);

                curShader->loadShaderCode((yyvsp[0].s)->c_str(), line_num, file_name.c_str() );
                pShdRep->addShader(curShader);
            } else
                yyerror("Failed in creating shader");
        }
    } else {
        if(curShader = curContainer->createCUDAShader((yyvsp[-2].global_varname_annots).varName ? (yyvsp[-2].global_varname_annots).varName->c_str() : NULL)->getExInterface())
        {
            // Now we copy the possible annotations that were filled in arraysz_semantic_annotations_uniformvalueassigment
            curShader->annotations()->getExInterface()->clear();
            curShader->annotations()->getExInterface()->copy(curAnnotations);

            curShader->loadShaderCode((yyvsp[0].s)->c_str(), line_num, file_name.c_str() );
        } else yyerror("Failed in creating shader");
    }
#else
    LOGI("CUDA Not supported in this build\n");
#endif
    delete (yyvsp[-2].global_varname_annots).varName;
    delete (yyvsp[0].s);
}
#line 6506 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 337:
#line 2842 "Grammar.ypp" /* yacc.c:1646  */
    {
    assert(curResource);
    // for now, sampler-state MUST be decalred before the texture work. Or it won't be found
    ISamplerState * pState = curContainer->findSamplerState((yyvsp[-1].s)->c_str());
    if(!pState)
        yyerror("Error: sampler-state referenced not found. Did you declare it before the texture using it ?\n");
    curResource->setSamplerState(pState);
    delete (yyvsp[-1].s);
}
#line 6520 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 338:
#line 2870 "Grammar.ypp" /* yacc.c:1646  */
    { 
    yyerror("Error in texture params declaration\n"); 
}
#line 6528 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 339:
#line 2877 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.resourceType)=nvFX::RESOURCE_UNKNOWN; }
#line 6534 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 340:
#line 2878 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.resourceType)=nvFX::RESTEX_1D; }
#line 6540 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 341:
#line 2879 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.resourceType)=nvFX::RESTEX_2D; }
#line 6546 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 342:
#line 2880 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.resourceType)=nvFX::RESTEX_3D; }
#line 6552 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 343:
#line 2881 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.resourceType)=nvFX::RESTEX_2DRECT; }
#line 6558 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 344:
#line 2882 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.resourceType)=nvFX::RESTEX_CUBE_MAP; }
#line 6564 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 345:
#line 2884 "Grammar.ypp" /* yacc.c:1646  */
    {
    curResource = curContainer->createResource((yyvsp[-1].s)->c_str(), (yyvsp[-2].resourceType), true/*ADD AS nvFX token*/)->getExInterface();
    curResource->setGLTexture(0); // this will tell the resource it is made of some external resource (reference to an existing texture)
    lex_pop_state();
    delete (yyvsp[-1].s);
    }
#line 6575 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 346:
#line 2890 "Grammar.ypp" /* yacc.c:1646  */
    {
    curResource = curContainer->createResource((yyvsp[0].s)->c_str(), (yyvsp[-1].resourceType), true/*ADD AS nvFX token*/)->getExInterface();
    curResource->setGLTexture(0); // this will tell the resource it is made of some external resource (reference to an existing texture)
    curAnnotations = curResource->annotations()->getExInterface();
    delete (yyvsp[0].s);
}
#line 6586 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 347:
#line 2895 "Grammar.ypp" /* yacc.c:1646  */
    { 
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
    curAnnotations = NULL;
    lex_pop_state(); lex_pop_state(); 
}
#line 6598 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 349:
#line 2907 "Grammar.ypp" /* yacc.c:1646  */
    {
    if(curResource) {
        // for now, sampler-state MUST be decalred before the texture work. Or it won't be found
        ISamplerState * pState = curContainer->findSamplerState((yyvsp[-1].s)->c_str());
        if(!pState)
            yyerror("Error: sampler-state referenced not found. Did you declare it before the texture using it ?\n");
        curResource->setSamplerState(pState);
    }
    delete (yyvsp[-1].s);
}
#line 6613 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 350:
#line 2917 "Grammar.ypp" /* yacc.c:1646  */
    {
    if(curResource)
        if(!curResource->annotations()->getExInterface()->addAnnotation((yyvsp[-3].s)->c_str(), (yyvsp[-1].s)->c_str()))
            yyerror("err\n");
    delete (yyvsp[-1].s);
    }
#line 6624 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 351:
#line 2923 "Grammar.ypp" /* yacc.c:1646  */
    {
    if(curResource)
        curResource->setMSAAApplicationDependent();
}
#line 6633 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 352:
#line 2927 "Grammar.ypp" /* yacc.c:1646  */
    {
    if(curResource)
        curResource->setMSAA((*(yyvsp[-2].intList))[0], (yyvsp[-2].intList)->size() > 1 ? (*(yyvsp[-2].intList))[1] : 0);
    delete (yyvsp[-2].intList);
}
#line 6643 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 353:
#line 2932 "Grammar.ypp" /* yacc.c:1646  */
    {
    if(curResource)
        curResource->setMSAA((yyvsp[-1].ival));
}
#line 6652 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 354:
#line 2936 "Grammar.ypp" /* yacc.c:1646  */
    {
    if(curResource)
        curResource->setDimensionsApplicationDependent();
}
#line 6661 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 355:
#line 2940 "Grammar.ypp" /* yacc.c:1646  */
    {
    int d = (int)(yyvsp[-2].intList)->size();
    if(curResource)
        curResource->setDimensions((*(yyvsp[-2].intList))[0], d > 1 ? (*(yyvsp[-2].intList))[1] : 1, d > 2 ? (*(yyvsp[-2].intList))[2] : 1);
    delete (yyvsp[-2].intList);
}
#line 6672 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 356:
#line 2946 "Grammar.ypp" /* yacc.c:1646  */
    {
    if(curResource)
        curResource->setDimensionFactor((yyvsp[-1].fval), (yyvsp[-1].fval));
}
#line 6681 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 357:
#line 2950 "Grammar.ypp" /* yacc.c:1646  */
    {
    if(curResource) {
        IResource* pRes = curContainer->findResource((yyvsp[-1].s)->c_str());
        if(!pRes)
        {
            yyerror("Error: Could not find the Resource :");
            yyerror((yyvsp[-1].s)->c_str());
        } else {
            curResource->setResolveTarget(pRes);
        }
    }
    delete (yyvsp[-1].s);
}
#line 6699 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 358:
#line 2963 "Grammar.ypp" /* yacc.c:1646  */
    {
    if(curResource) {
        if(!curResource->setFormat((yyvsp[-1].resource_format)))
        {
            yyerror("Error: Bad format\n");
        }
    }
}
#line 6712 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 359:
#line 2975 "Grammar.ypp" /* yacc.c:1646  */
    {
    curResource = curContainer->createResource((yyvsp[-1].s)->c_str(), RESTEX_2D, (yyvsp[0].bval))->getExInterface();
    if(curResource->getFormat() != FMT_UNKNOWN)
        curResource = NULL;
    else
        curAnnotations = curResource->annotations()->getExInterface();
    delete (yyvsp[-1].s);
}
#line 6725 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 360:
#line 2982 "Grammar.ypp" /* yacc.c:1646  */
    {
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
}
#line 6735 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 361:
#line 2986 "Grammar.ypp" /* yacc.c:1646  */
    { 
    curAnnotations = NULL;
    lex_pop_state(); lex_pop_state(); 
}
#line 6744 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 362:
#line 2990 "Grammar.ypp" /* yacc.c:1646  */
    {
    curResource = curContainer->createResource((yyvsp[-1].s)->c_str(), RESTEX_2DRECT, (yyvsp[0].bval))->getExInterface();
    if(curResource->getFormat() != FMT_UNKNOWN)
        curResource = NULL;
    else
        curAnnotations = curResource->annotations()->getExInterface();
    delete (yyvsp[-1].s);
}
#line 6757 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 363:
#line 2997 "Grammar.ypp" /* yacc.c:1646  */
    {
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
}
#line 6767 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 364:
#line 3001 "Grammar.ypp" /* yacc.c:1646  */
    { 
    curAnnotations = NULL;
    lex_pop_state(); lex_pop_state(); 
}
#line 6776 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 365:
#line 3005 "Grammar.ypp" /* yacc.c:1646  */
    {
    curResource = curContainer->createResource((yyvsp[-1].s)->c_str(), RESRBUF_2D, (yyvsp[0].bval))->getExInterface();
    if(curResource->getFormat() != FMT_UNKNOWN)
        curResource = NULL;
    else
        curAnnotations = curResource->annotations()->getExInterface();
    delete (yyvsp[-1].s);
}
#line 6789 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 366:
#line 3012 "Grammar.ypp" /* yacc.c:1646  */
    {
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
}
#line 6799 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 367:
#line 3016 "Grammar.ypp" /* yacc.c:1646  */
    { 
    curAnnotations = NULL;
    lex_pop_state(); lex_pop_state(); 
}
#line 6808 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 369:
#line 3025 "Grammar.ypp" /* yacc.c:1646  */
    {
    if(curFBO)
      if(!curFBO->annotations()->getExInterface()->addAnnotation((yyvsp[-3].s)->c_str(), (yyvsp[-1].s)->c_str()))
        yyerror("err\n");
    delete (yyvsp[-1].s);
    }
#line 6819 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 370:
#line 3034 "Grammar.ypp" /* yacc.c:1646  */
    {
    if(curFBO)
    {
        IResource* pRes = curContainer->findResource((yyvsp[-1].s)->c_str());
        if(!pRes)
        {
            yyerror("Error: Could not find the Texture resource :");
            yyerror((yyvsp[-1].s)->c_str());
        } else {
            curFBO->setDSTResource(pRes);
        }
    }
    delete (yyvsp[-1].s);
}
#line 6838 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 371:
#line 3048 "Grammar.ypp" /* yacc.c:1646  */
    { /*liststart VARNAME ',' VARNAME ',' VARNAME ',' VARNAME listend ';' { */
    if(curFBO) {
        IResource* pRes[4/*MAX_RENDERTARGETS*/];
        if((yyvsp[-2].varNameList)->size() > 4)
            yyerror("Error: Can't assign more than 4 color targets in a Framebuffer");
        else {
            bool ok = true;
            for(int i=0; i<(int)(yyvsp[-2].varNameList)->size(); i++)
            {
                pRes[i] = curContainer->findResource((*(yyvsp[-2].varNameList))[i].c_str());
                if(pRes[i]==NULL)
                {
                    yyerror("Error: Could not find some of the Textures");
                    ok = false;
                }
            }
            if(ok) curFBO->setColorResource(pRes, (int)(yyvsp[-2].varNameList)->size());
        }
    }
    delete (yyvsp[-2].varNameList);
}
#line 6864 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 372:
#line 3069 "Grammar.ypp" /* yacc.c:1646  */
    {
    if(curFBO) {
        IResource* pRes = curContainer->findResource((yyvsp[-1].s)->c_str());
        if(!pRes)
        {
            yyerror("Error: Could not find the Texture :");
            yyerror((yyvsp[-1].s)->c_str());
        } else {
            curFBO->setColorResource(&pRes, 1);
        }
    }
    delete (yyvsp[-1].s);
}
#line 6882 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 373:
#line 3086 "Grammar.ypp" /* yacc.c:1646  */
    {
    curFBO = curContainer->createFBO((yyvsp[-1].s)->c_str(), (yyvsp[0].bval))->getExInterface();
    if(curFBO->getDSTResource() || curFBO->getColorResource(0))
        curFBO = NULL; // we already have it defined. Don't want to re-define it again
    else
        curAnnotations = curFBO->annotations()->getExInterface();
    delete (yyvsp[-1].s);
}
#line 6895 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 374:
#line 3093 "Grammar.ypp" /* yacc.c:1646  */
    {
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
}
#line 6905 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 375:
#line 3097 "Grammar.ypp" /* yacc.c:1646  */
    { 
    curAnnotations = NULL;
    lex_pop_state(); lex_pop_state(); // fbo and then fbo1 state
}
#line 6914 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 382:
#line 3112 "Grammar.ypp" /* yacc.c:1646  */
    { yyerror("Error in Sampler State filtering\n");}
#line 6920 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 387:
#line 3119 "Grammar.ypp" /* yacc.c:1646  */
    { yyerror("Error in Sampler State UV Addressing\n");}
#line 6926 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 389:
#line 3127 "Grammar.ypp" /* yacc.c:1646  */
    {
    curSamplerState->getExInterface()->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)));
}
#line 6934 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 390:
#line 3130 "Grammar.ypp" /* yacc.c:1646  */
    {
    curSamplerState->getExInterface()->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)));
}
#line 6942 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 391:
#line 3133 "Grammar.ypp" /* yacc.c:1646  */
    {
    curSamplerState->getExInterface()->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)));
}
#line 6950 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 392:
#line 3136 "Grammar.ypp" /* yacc.c:1646  */
    {
    curSamplerState->getExInterface()->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)));
}
#line 6958 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 393:
#line 3139 "Grammar.ypp" /* yacc.c:1646  */
    {
    curSamplerState->getExInterface()->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)));
}
#line 6966 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 394:
#line 3142 "Grammar.ypp" /* yacc.c:1646  */
    { yyerror("Error in one of sampler-state states\n"); }
#line 6972 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 395:
#line 3147 "Grammar.ypp" /* yacc.c:1646  */
    {
    LOGD("Sampler-state %s\n", (yyvsp[0].s)->c_str() );
    curSamplerState = curContainer->createSamplerState((yyvsp[0].s)->c_str())->getExInterface();
    curAnnotations = curSamplerState->annotations()->getExInterface();
    delete (yyvsp[0].s);
}
#line 6983 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 396:
#line 3152 "Grammar.ypp" /* yacc.c:1646  */
    { 
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
    lex_pop_state(); curAnnotations = NULL;}
#line 6993 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 397:
#line 3157 "Grammar.ypp" /* yacc.c:1646  */
    { 
    yyerror("Error sampler-state declaration\n"); 
}
#line 7001 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 398:
#line 3172 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), ((yyvsp[-1].fval)))) yyerror("Failed to create state!\n"); }
#line 7007 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 399:
#line 3173 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)(((yyvsp[-1].bval) )? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7013 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 400:
#line 3174 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)(((yyvsp[-1].bval) )? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7019 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 401:
#line 3175 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)(((yyvsp[-1].bval) )? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7025 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 402:
#line 3176 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7031 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 403:
#line 3177 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), ((yyvsp[-1].fval)))) yyerror("Failed to create state!\n"); }
#line 7037 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 404:
#line 3178 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)(((yyvsp[-1].bval) )? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7043 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 405:
#line 3180 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (unsigned int)(yyvsp[-1].ival))) yyerror("Failed to create state!\n"); }
#line 7049 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 406:
#line 3181 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (unsigned int)(yyvsp[-1].ival))) yyerror("Failed to create state!\n"); }
#line 7055 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 407:
#line 3182 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7061 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 408:
#line 3183 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7067 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 409:
#line 3184 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7073 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 410:
#line 3185 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7079 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 411:
#line 3186 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7085 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 412:
#line 3187 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7091 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 413:
#line 3188 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-7].glenum)), getGLEnumValue((yyvsp[-4].glenum)), getGLEnumValue((yyvsp[-2].glenum)))) yyerror("Failed to create state!\n"); }
#line 7097 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 414:
#line 3189 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (yyvsp[-1].fval))) yyerror("Failed to create state!\n"); }
#line 7103 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 415:
#line 3190 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (yyvsp[-1].fval))) yyerror("Failed to create state!\n"); }
#line 7109 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 416:
#line 3191 "Grammar.ypp" /* yacc.c:1646  */
    {
    check_curRState();
    if(!curRState->createState(getGLEnumValue(GLPOLYGONOFFSETFACTOR), (yyvsp[-4].fval))) yyerror("Failed to create state!\n");
    if(!curRState->createState(getGLEnumValue(GLPOLYGONOFFSETUNITS), (yyvsp[-2].fval))) yyerror("Failed to create state!\n");
}
#line 7119 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 417:
#line 3196 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7125 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 418:
#line 3197 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7131 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 419:
#line 3198 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7137 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 420:
#line 3200 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7143 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 421:
#line 3201 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7149 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 422:
#line 3202 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7155 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 423:
#line 3203 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), (yyvsp[-1].fval))) yyerror("Failed to create state!\n"); }
#line 7161 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 424:
#line 3204 "Grammar.ypp" /* yacc.c:1646  */
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLALPHATESTFUNC), getGLEnumValue((yyvsp[-4].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLALPHATESTREF), (yyvsp[-2].fval))) yyerror("Failed to create state!\n");
}
#line 7171 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 425:
#line 3209 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7177 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 426:
#line 3210 "Grammar.ypp" /* yacc.c:1646  */
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLBLENDSRC), getGLEnumValue((yyvsp[-4].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDDST), getGLEnumValue((yyvsp[-2].glenum)))) yyerror("Failed to create state!\n");
}
#line 7187 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 427:
#line 3215 "Grammar.ypp" /* yacc.c:1646  */
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLBLENDSRCRGB), getGLEnumValue((yyvsp[-8].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDDSTRGB), getGLEnumValue((yyvsp[-6].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDSRCALPHA), getGLEnumValue((yyvsp[-4].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDDSTALPHA), getGLEnumValue((yyvsp[-2].glenum)))) yyerror("Failed to create state!\n");
}
#line 7199 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 428:
#line 3222 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7205 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 429:
#line 3223 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7211 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 430:
#line 3224 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7217 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 431:
#line 3225 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7223 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 432:
#line 3226 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7229 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 433:
#line 3227 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7235 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 434:
#line 3229 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7241 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 435:
#line 3230 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7247 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 436:
#line 3231 "Grammar.ypp" /* yacc.c:1646  */
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONRGB), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONALPHA), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n");
}
#line 7257 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 437:
#line 3236 "Grammar.ypp" /* yacc.c:1646  */
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONRGB), getGLEnumValue((yyvsp[-4].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONALPHA), getGLEnumValue((yyvsp[-2].glenum)))) yyerror("Failed to create state!\n");
}
#line 7267 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 438:
#line 3246 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-11].glenum)), (yyvsp[-8].fval), (yyvsp[-6].fval), (yyvsp[-4].fval), (yyvsp[-2].fval))) yyerror("Failed to create state!\n"); }
#line 7273 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 439:
#line 3247 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7279 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 440:
#line 3248 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7285 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 441:
#line 3249 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7291 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 442:
#line 3250 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7297 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 443:
#line 3252 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-11].glenum)), 
    (GLboolean)((yyvsp[-8].bval) ? GL_TRUE: GL_FALSE),
    (GLboolean)((yyvsp[-6].bval) ? GL_TRUE: GL_FALSE),
    (GLboolean)((yyvsp[-4].bval) ? GL_TRUE: GL_FALSE),
    (GLboolean)((yyvsp[-2].bval) ? GL_TRUE: GL_FALSE)
    )) yyerror("Failed to create state!\n"); }
#line 7308 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 444:
#line 3258 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7314 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 445:
#line 3259 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7320 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 446:
#line 3261 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7326 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 447:
#line 3262 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7332 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 448:
#line 3263 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState();if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7338 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 449:
#line 3264 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7344 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 450:
#line 3265 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7350 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 451:
#line 3266 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7356 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 452:
#line 3267 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7362 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 453:
#line 3269 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-7].glenum)), (yyvsp[-4].fval), (yyvsp[-2].fval))) yyerror("Failed to create state!\n"); }
#line 7368 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 454:
#line 3270 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7374 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 455:
#line 3271 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7380 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 456:
#line 3272 "Grammar.ypp" /* yacc.c:1646  */
    { 
    check_curDSTState();
    if(!curDSTState->createState(getGLEnumValue((yyvsp[-9].glenum)), getGLEnumValue((yyvsp[-6].glenum)))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILREF), (unsigned int)(yyvsp[-4].ival))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILVALUEMASK), (unsigned int)(yyvsp[-2].ival))) yyerror("Failed to create state!\n");
}
#line 7391 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 457:
#line 3278 "Grammar.ypp" /* yacc.c:1646  */
    { 
    check_curDSTState();
    if(!curDSTState->createState(getGLEnumValue((yyvsp[-8].glenum) == GLFRONT ? GLSTENCILFUNC : GLSTENCILBACKFUNC), getGLEnumValue((yyvsp[-6].glenum)))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue((yyvsp[-8].glenum) == GLFRONT ? GLSTENCILREF : GLSTENCILBACKREF), (unsigned int)(yyvsp[-4].ival))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue((yyvsp[-8].glenum) == GLFRONT ? GLSTENCILVALUEMASK : GLSTENCILBACKVALUEMASK), (unsigned int)(yyvsp[-2].ival))) yyerror("Failed to create state!\n");
}
#line 7402 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 458:
#line 3284 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), (unsigned int)(yyvsp[-1].ival))) yyerror("Failed to create state!\n"); }
#line 7408 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 459:
#line 3285 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), (unsigned int)(yyvsp[-1].ival))) yyerror("Failed to create state!\n"); }
#line 7414 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 460:
#line 3286 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), (unsigned int)(yyvsp[-1].ival))) yyerror("Failed to create state!\n"); }
#line 7420 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 461:
#line 3287 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7426 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 462:
#line 3288 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7432 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 463:
#line 3289 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7438 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 464:
#line 3290 "Grammar.ypp" /* yacc.c:1646  */
    {
    // a concatenated version of GLSTENCILPASSDEPTHFAIL+GLSTENCILPASSDEPTHPASS+GLSTENCILWRITEMASK == GL function arguments
    check_curDSTState();
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILFAIL), getGLEnumValue((yyvsp[-6].glenum)))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILPASSDEPTHFAIL), getGLEnumValue((yyvsp[-4].glenum)))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILPASSDEPTHPASS), getGLEnumValue((yyvsp[-2].glenum)))) yyerror("Failed to create state!\n");
}
#line 7450 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 465:
#line 3297 "Grammar.ypp" /* yacc.c:1646  */
    { 
    check_curDSTState();
    if(!curDSTState->createState(getGLEnumValue((yyvsp[-8].glenum) == GLFRONT ? GLSTENCILFAIL : GLSTENCILBACKFAIL), getGLEnumValue((yyvsp[-6].glenum)))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue((yyvsp[-8].glenum) == GLFRONT ? GLSTENCILPASSDEPTHFAIL : GLSTENCILBACKPASSDEPTHFAIL), (unsigned int)(yyvsp[-4].glenum))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue((yyvsp[-8].glenum) == GLFRONT ? GLSTENCILPASSDEPTHPASS : GLSTENCILBACKPASSDEPTHPASS), (unsigned int)(yyvsp[-2].glenum))) yyerror("Failed to create state!\n");
}
#line 7461 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 466:
#line 3310 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7467 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 467:
#line 3311 "Grammar.ypp" /* yacc.c:1646  */
    { 
    check_curDSTState();
    if(!curDSTState->createState(getGLEnumValue((yyvsp[-9].glenum)), getGLEnumValue((yyvsp[-6].glenum)))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILBACKREF), (unsigned int)(yyvsp[-4].ival))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILBACKVALUEMASK), (unsigned int)(yyvsp[-2].ival))) yyerror("Failed to create state!\n");
}
#line 7478 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 468:
#line 3317 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), (unsigned int)(yyvsp[-1].ival))) yyerror("Failed to create state!\n"); }
#line 7484 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 469:
#line 3318 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), (unsigned int)(yyvsp[-1].ival))) yyerror("Failed to create state!\n"); }
#line 7490 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 470:
#line 3319 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7496 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 471:
#line 3320 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7502 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 472:
#line 3321 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7508 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 473:
#line 3322 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), (unsigned int)(yyvsp[-1].ival))) yyerror("Failed to create state!\n"); }
#line 7514 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 475:
#line 3329 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (yyvsp[-1].fval))) yyerror("Failed to create state!\n"); }
#line 7520 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 476:
#line 3330 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7526 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 477:
#line 3331 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7532 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 478:
#line 3332 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7538 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 479:
#line 3333 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7544 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 480:
#line 3334 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (yyvsp[-1].fval))) yyerror("Failed to create state!\n"); }
#line 7550 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 481:
#line 3335 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7556 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 482:
#line 3337 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (unsigned int)(yyvsp[-1].ival))) yyerror("Failed to create state!\n"); }
#line 7562 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 483:
#line 3338 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (unsigned int)(yyvsp[-1].ival))) yyerror("Failed to create state!\n"); }
#line 7568 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 484:
#line 3339 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7574 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 485:
#line 3340 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7580 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 486:
#line 3341 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7586 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 487:
#line 3342 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7592 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 488:
#line 3343 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7598 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 489:
#line 3344 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7604 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 490:
#line 3345 "Grammar.ypp" /* yacc.c:1646  */
    { if(!curRState->createState(getGLEnumValue((yyvsp[-7].glenum)), getGLEnumValue((yyvsp[-4].glenum)), getGLEnumValue((yyvsp[-2].glenum)))) yyerror("Failed to create state!\n"); }
#line 7610 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 491:
#line 3346 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (yyvsp[-1].fval))) yyerror("Failed to create state!\n"); }
#line 7616 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 492:
#line 3347 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (yyvsp[-1].fval))) yyerror("Failed to create state!\n"); }
#line 7622 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 493:
#line 3348 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7628 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 494:
#line 3349 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7634 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 495:
#line 3350 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7640 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 496:
#line 3352 "Grammar.ypp" /* yacc.c:1646  */
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7646 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 497:
#line 3353 "Grammar.ypp" /* yacc.c:1646  */
    { yyerror("Error in Rasterizer-state list\n"); }
#line 7652 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 498:
#line 3358 "Grammar.ypp" /* yacc.c:1646  */
    {
    LOGD("Rasterization state %s\n", (yyvsp[0].s)->c_str() );
    curRState = curContainer->createStateGroup_Raster((yyvsp[0].s)->c_str())->getExInterface();
    curAnnotations = curRState->annotations()->getExInterface();
    delete (yyvsp[0].s);
}
#line 7663 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 499:
#line 3363 "Grammar.ypp" /* yacc.c:1646  */
    { 
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
    curAnnotations = NULL; curRState = NULL; lex_pop_state(); }
#line 7673 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 500:
#line 3368 "Grammar.ypp" /* yacc.c:1646  */
    { yyerror("Error in Rasterizer-state declaration\n"); }
#line 7679 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 502:
#line 3374 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7685 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 503:
#line 3375 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7691 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 504:
#line 3376 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), (yyvsp[-1].fval))) yyerror("Failed to create state!\n"); }
#line 7697 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 505:
#line 3377 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7703 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 506:
#line 3378 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7709 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 507:
#line 3379 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7715 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 508:
#line 3380 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7721 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 509:
#line 3381 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7727 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 510:
#line 3382 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7733 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 511:
#line 3383 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7739 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 512:
#line 3385 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7745 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 513:
#line 3386 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7751 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 514:
#line 3387 "Grammar.ypp" /* yacc.c:1646  */
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONRGB), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONALPHA), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n");
}
#line 7761 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 515:
#line 3392 "Grammar.ypp" /* yacc.c:1646  */
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONRGB), getGLEnumValue((yyvsp[-4].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONALPHA), getGLEnumValue((yyvsp[-2].glenum)))) yyerror("Failed to create state!\n");
}
#line 7771 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 516:
#line 3402 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-11].glenum)), (yyvsp[-8].fval), (yyvsp[-6].fval), (yyvsp[-4].fval), (yyvsp[-2].fval))) yyerror("Failed to create state!\n"); }
#line 7777 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 517:
#line 3403 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7783 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 518:
#line 3404 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7789 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 519:
#line 3405 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7795 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 520:
#line 3406 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7801 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 521:
#line 3408 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-11].glenum)), 
    (GLboolean)((yyvsp[-8].bval) ? GL_TRUE: GL_FALSE),
    (GLboolean)((yyvsp[-6].bval) ? GL_TRUE: GL_FALSE),
    (GLboolean)((yyvsp[-4].bval) ? GL_TRUE: GL_FALSE),
    (GLboolean)((yyvsp[-2].bval) ? GL_TRUE: GL_FALSE)
    )) yyerror("Failed to create state!\n"); }
#line 7812 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 522:
#line 3414 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7818 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 523:
#line 3415 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7824 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 524:
#line 3417 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7830 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 525:
#line 3418 "Grammar.ypp" /* yacc.c:1646  */
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7836 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 526:
#line 3419 "Grammar.ypp" /* yacc.c:1646  */
    { yyerror("Error in Color sample state list\n"); }
#line 7842 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 527:
#line 3424 "Grammar.ypp" /* yacc.c:1646  */
    {
    LOGD("Color sample state %s\n", (yyvsp[0].s)->c_str() );
    curCSState = curContainer->createStateGroup_CS((yyvsp[0].s)->c_str())->getExInterface();
    curAnnotations = curCSState->annotations()->getExInterface();
    delete (yyvsp[0].s);
}
#line 7853 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 528:
#line 3429 "Grammar.ypp" /* yacc.c:1646  */
    { 
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
    curAnnotations = NULL; curCSState = NULL; lex_pop_state(); }
#line 7863 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 529:
#line 3434 "Grammar.ypp" /* yacc.c:1646  */
    { yyerror("Error in color sampler state declaration\n"); }
#line 7869 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 531:
#line 3440 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7875 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 532:
#line 3441 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7881 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 533:
#line 3442 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7887 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 534:
#line 3443 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7893 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 535:
#line 3444 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7899 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 536:
#line 3446 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-7].glenum)), (yyvsp[-4].fval), (yyvsp[-2].fval))) yyerror("Failed to create state!\n"); }
#line 7905 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 537:
#line 3447 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), (GLenum)((yyvsp[-1].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); }
#line 7911 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 538:
#line 3448 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7917 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 539:
#line 3449 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), (unsigned int)(yyvsp[-1].ival))) yyerror("Failed to create state!\n"); }
#line 7923 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 540:
#line 3450 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), (unsigned int)(yyvsp[-1].ival))) yyerror("Failed to create state!\n"); }
#line 7929 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 541:
#line 3451 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7935 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 542:
#line 3452 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7941 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 543:
#line 3453 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7947 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 544:
#line 3454 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), (unsigned int)(yyvsp[-1].ival))) yyerror("Failed to create state!\n"); }
#line 7953 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 545:
#line 3462 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7959 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 546:
#line 3463 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), (unsigned int)(yyvsp[-1].ival))) yyerror("Failed to create state!\n"); }
#line 7965 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 547:
#line 3464 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), (unsigned int)(yyvsp[-1].ival))) yyerror("Failed to create state!\n"); }
#line 7971 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 548:
#line 3465 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7977 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 549:
#line 3466 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7983 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 550:
#line 3467 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n"); }
#line 7989 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 551:
#line 3468 "Grammar.ypp" /* yacc.c:1646  */
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[-3].glenum)), (unsigned int)(yyvsp[-1].ival))) yyerror("Failed to create state!\n"); }
#line 7995 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 552:
#line 3469 "Grammar.ypp" /* yacc.c:1646  */
    { yyerror("Error in Depth Stencil state list\n"); }
#line 8001 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 553:
#line 3474 "Grammar.ypp" /* yacc.c:1646  */
    {
    LOGD("Depth-Stencil state %s\n", (yyvsp[0].s)->c_str() );
    curDSTState = curContainer->createStateGroup_DST((yyvsp[0].s)->c_str())->getExInterface();
    curAnnotations = curDSTState->annotations()->getExInterface();
    delete (yyvsp[0].s);
}
#line 8012 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 554:
#line 3479 "Grammar.ypp" /* yacc.c:1646  */
    { 
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
    curAnnotations = NULL; curDSTState = NULL; lex_pop_state(); }
#line 8022 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 555:
#line 3484 "Grammar.ypp" /* yacc.c:1646  */
    { yyerror("Error in DST-state declaration\n"); }
#line 8028 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 556:
#line 3487 "Grammar.ypp" /* yacc.c:1646  */
    { LOGI("TODO"); }
#line 8034 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 557:
#line 3488 "Grammar.ypp" /* yacc.c:1646  */
    { LOGI("TODO"); }
#line 8040 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 558:
#line 3489 "Grammar.ypp" /* yacc.c:1646  */
    { LOGI("TODO"); }
#line 8046 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 559:
#line 3490 "Grammar.ypp" /* yacc.c:1646  */
    { LOGI("TODO"); }
#line 8052 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 560:
#line 3491 "Grammar.ypp" /* yacc.c:1646  */
    { LOGI("TODO"); }
#line 8058 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 561:
#line 3492 "Grammar.ypp" /* yacc.c:1646  */
    { LOGI("TODO"); }
#line 8064 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 562:
#line 3493 "Grammar.ypp" /* yacc.c:1646  */
    { LOGI("TODO"); }
#line 8070 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 563:
#line 3494 "Grammar.ypp" /* yacc.c:1646  */
    { LOGI("TODO"); }
#line 8076 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 564:
#line 3495 "Grammar.ypp" /* yacc.c:1646  */
    { LOGI("TODO"); }
#line 8082 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 565:
#line 3496 "Grammar.ypp" /* yacc.c:1646  */
    { LOGI("TODO"); }
#line 8088 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 566:
#line 3497 "Grammar.ypp" /* yacc.c:1646  */
    { LOGI("TODO"); }
#line 8094 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 567:
#line 3498 "Grammar.ypp" /* yacc.c:1646  */
    { LOGI("TODO"); }
#line 8100 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 568:
#line 3499 "Grammar.ypp" /* yacc.c:1646  */
    { LOGI("TODO"); }
#line 8106 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 569:
#line 3500 "Grammar.ypp" /* yacc.c:1646  */
    { LOGI("TODO"); }
#line 8112 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 570:
#line 3501 "Grammar.ypp" /* yacc.c:1646  */
    { LOGI("TODO"); }
#line 8118 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 571:
#line 3502 "Grammar.ypp" /* yacc.c:1646  */
    { LOGI("TODO"); }
#line 8124 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 572:
#line 3503 "Grammar.ypp" /* yacc.c:1646  */
    { LOGI("TODO"); }
#line 8130 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 573:
#line 3504 "Grammar.ypp" /* yacc.c:1646  */
    { LOGI("TODO"); }
#line 8136 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 574:
#line 3505 "Grammar.ypp" /* yacc.c:1646  */
    { LOGI("TODO"); }
#line 8142 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 575:
#line 3506 "Grammar.ypp" /* yacc.c:1646  */
    { LOGI("TODO"); }
#line 8148 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 576:
#line 3507 "Grammar.ypp" /* yacc.c:1646  */
    { LOGI("TODO"); }
#line 8154 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 577:
#line 3508 "Grammar.ypp" /* yacc.c:1646  */
    { LOGI("TODO"); }
#line 8160 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 579:
#line 3517 "Grammar.ypp" /* yacc.c:1646  */
    { LOGI("TODO"); }
#line 8166 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 580:
#line 3518 "Grammar.ypp" /* yacc.c:1646  */
    { 
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n");
#endif
}
#line 8176 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 581:
#line 3523 "Grammar.ypp" /* yacc.c:1646  */
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n");
#endif
}
#line 8186 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 582:
#line 3528 "Grammar.ypp" /* yacc.c:1646  */
    {
#ifndef OGLES2
    LOGI("TODO");
#endif
}
#line 8196 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 583:
#line 3533 "Grammar.ypp" /* yacc.c:1646  */
    {
#ifndef OGLES2
    LOGI("TODO");
#endif
}
#line 8206 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 584:
#line 3538 "Grammar.ypp" /* yacc.c:1646  */
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n");
#endif
}
#line 8216 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 585:
#line 3543 "Grammar.ypp" /* yacc.c:1646  */
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (yyvsp[-1].fval))) yyerror("Failed to create state!\n");
#endif
}
#line 8226 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 586:
#line 3548 "Grammar.ypp" /* yacc.c:1646  */
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n");
#endif
}
#line 8236 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 587:
#line 3553 "Grammar.ypp" /* yacc.c:1646  */
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n");
#endif
}
#line 8246 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 588:
#line 3558 "Grammar.ypp" /* yacc.c:1646  */
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n");
#endif
}
#line 8256 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 589:
#line 3563 "Grammar.ypp" /* yacc.c:1646  */
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n");
#endif
}
#line 8266 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 590:
#line 3568 "Grammar.ypp" /* yacc.c:1646  */
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n");
#endif
}
#line 8276 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 591:
#line 3573 "Grammar.ypp" /* yacc.c:1646  */
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (yyvsp[-1].fval))) yyerror("Failed to create state!\n");
#endif
}
#line 8286 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 592:
#line 3578 "Grammar.ypp" /* yacc.c:1646  */
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (yyvsp[-1].fval))) yyerror("Failed to create state!\n");
#endif
}
#line 8296 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 593:
#line 3583 "Grammar.ypp" /* yacc.c:1646  */
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n");
#endif
}
#line 8306 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 594:
#line 3588 "Grammar.ypp" /* yacc.c:1646  */
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (yyvsp[-1].fval))) yyerror("Failed to create state!\n");
#endif
}
#line 8316 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 595:
#line 3593 "Grammar.ypp" /* yacc.c:1646  */
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n");
#endif
}
#line 8326 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 596:
#line 3598 "Grammar.ypp" /* yacc.c:1646  */
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[-3].glenum)), (yyvsp[-1].ival))) yyerror("Failed to create state!\n");
#endif
}
#line 8336 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 597:
#line 3603 "Grammar.ypp" /* yacc.c:1646  */
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n");
#endif
}
#line 8346 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 598:
#line 3608 "Grammar.ypp" /* yacc.c:1646  */
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n");
#endif
}
#line 8356 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 599:
#line 3613 "Grammar.ypp" /* yacc.c:1646  */
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n");
#endif
}
#line 8366 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 600:
#line 3618 "Grammar.ypp" /* yacc.c:1646  */
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[-3].glenum)), getGLEnumValue((yyvsp[-1].glenum)))) yyerror("Failed to create state!\n");
#endif
}
#line 8376 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 601:
#line 3629 "Grammar.ypp" /* yacc.c:1646  */
    { yyerror("Error in Path-Rendering state list\n"); }
#line 8382 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 602:
#line 3635 "Grammar.ypp" /* yacc.c:1646  */
    {
#ifndef OGLES2
    LOGD("Path Rendering state %s\n", (yyvsp[0].s)->c_str() ); 
    curPRState = curContainer->createStateGroup_PR((yyvsp[0].s)->c_str())->getExInterface();
    curAnnotations = curPRState->annotations()->getExInterface();
#endif
    delete (yyvsp[0].s);
}
#line 8395 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 603:
#line 3642 "Grammar.ypp" /* yacc.c:1646  */
    {
#ifndef OGLES2
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
    curAnnotations = NULL;
    curPRState = NULL;
#endif
    lex_pop_state(); }
#line 8409 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 604:
#line 3651 "Grammar.ypp" /* yacc.c:1646  */
    { yyerror("Error in DST-state declaration\n"); }
#line 8415 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 606:
#line 3662 "Grammar.ypp" /* yacc.c:1646  */
    {
    if(!curAnnotations)
        curAnnotations = IAnnotationEx::getAnnotationSingleton(2); // need a temporary place since nothing was initialized
    if(!curAnnotations->addAnnotation((yyvsp[-3].s)->c_str(), (yyvsp[-1].s)->c_str()))
        yyerror("err\n");
    delete (yyvsp[-1].s);
    }
#line 8427 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 607:
#line 3669 "Grammar.ypp" /* yacc.c:1646  */
    {
    if(!curAnnotations)
        curAnnotations = IAnnotationEx::getAnnotationSingleton(2); // need a temporary place since nothing was initialized
    else if(!curAnnotations->addAnnotation((yyvsp[-3].s)->c_str(), (yyvsp[-1].fval)))
        yyerror("err\n");
    }
#line 8438 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 608:
#line 3675 "Grammar.ypp" /* yacc.c:1646  */
    {
    if(!curAnnotations)
        curAnnotations = IAnnotationEx::getAnnotationSingleton(2); // need a temporary place since nothing was initialized
    else if(!curAnnotations->addAnnotation((yyvsp[-3].s)->c_str(), (yyvsp[-1].ival)))
        yyerror("err\n");
    }
#line 8449 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 609:
#line 3681 "Grammar.ypp" /* yacc.c:1646  */
    {
    if(!curAnnotations)
        curAnnotations = IAnnotationEx::getAnnotationSingleton(2); // need a temporary place since nothing was initialized
    else if(!curAnnotations->addAnnotation((yyvsp[-3].s)->c_str(), (yyvsp[-1].bval)))
        yyerror("err\n");
    }
#line 8460 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 610:
#line 3693 "Grammar.ypp" /* yacc.c:1646  */
    { yyerror("Error in annotation\n"); }
#line 8466 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 614:
#line 3704 "Grammar.ypp" /* yacc.c:1646  */
    {
        LOGD("value %f\n", (yyvsp[0].fval) );
        FloatList* pfl = new FloatList;
        pfl->push_back((yyvsp[0].fval));
        (yyval.floatVecList) = new FloatVecList;
        (yyval.floatVecList)->push_back(pfl);
}
#line 8478 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 615:
#line 3714 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.floatVecList) = new FloatVecList;
    (yyval.floatVecList)->push_back((yyvsp[-1].floatList));
}
#line 8487 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 616:
#line 3721 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.floatVecList) = (yyvsp[-1].floatVecList);
}
#line 8495 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 617:
#line 3727 "Grammar.ypp" /* yacc.c:1646  */
    {
        LOGD("value %d\n", (yyvsp[0].ival) );
        IntList* pfl = new IntList;
        pfl->push_back((yyvsp[0].ival));
        (yyval.intVecList) = new IntVecList;
        (yyval.intVecList)->push_back(pfl);
}
#line 8507 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 618:
#line 3734 "Grammar.ypp" /* yacc.c:1646  */
    {
      LOGI("Error in valueassignment_i\n");
  }
#line 8515 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 619:
#line 3746 "Grammar.ypp" /* yacc.c:1646  */
    {
        LOGD("value %d\n", (yyvsp[0].bval) );
        BoolList* pfl = new BoolList;
        pfl->push_back((yyvsp[0].bval));
        (yyval.boolVecList) = new BoolVecList;
        (yyval.boolVecList)->push_back(pfl);
}
#line 8527 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 620:
#line 3756 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.boolVecList) = new BoolVecList;
    (yyval.boolVecList)->push_back((yyvsp[-1].boolList));
}
#line 8536 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 621:
#line 3762 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.precisionType)=IUniform::PHighP; }
#line 8542 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 622:
#line 3763 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.precisionType)=IUniform::PMediumP; }
#line 8548 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 623:
#line 3764 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.precisionType)=IUniform::PLowP; }
#line 8554 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 624:
#line 3771 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.uniformQualifiers).hasUniformKeyword = true;
    (yyval.uniformQualifiers).global = false;
    (yyval.uniformQualifiers).precisionType = IUniform::PHighP;
}
#line 8564 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 625:
#line 3776 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.uniformQualifiers).hasUniformKeyword = true;
    (yyval.uniformQualifiers).global = (yyvsp[0].bval);
    (yyval.uniformQualifiers).precisionType = IUniform::PHighP;
}
#line 8574 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 626:
#line 3781 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.uniformQualifiers).hasUniformKeyword = true;
    (yyval.uniformQualifiers).global = false;
    (yyval.uniformQualifiers).precisionType = (yyvsp[0].precisionType);
}
#line 8584 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 627:
#line 3786 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.uniformQualifiers).hasUniformKeyword = true;
    (yyval.uniformQualifiers).global = (yyvsp[-1].bval);
    (yyval.uniformQualifiers).precisionType = (yyvsp[0].precisionType);
}
#line 8594 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 628:
#line 3791 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.uniformQualifiers).hasUniformKeyword = false;
    (yyval.uniformQualifiers).global = (yyvsp[0].bval);
#ifdef OGLES2
    (yyval.uniformQualifiers).precisionType = IUniform::PMediumP; 
#else
    (yyval.uniformQualifiers).precisionType = IUniform::PHighP;
#endif
}
#line 8608 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 629:
#line 3800 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.uniformQualifiers).hasUniformKeyword = false;
    (yyval.uniformQualifiers).global = false;
    (yyval.uniformQualifiers).precisionType = (yyvsp[0].precisionType);
}
#line 8618 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 630:
#line 3805 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.uniformQualifiers).hasUniformKeyword = false;
    (yyval.uniformQualifiers).global = (yyvsp[-1].bval);
    (yyval.uniformQualifiers).precisionType = (yyvsp[0].precisionType);
}
#line 8628 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 631:
#line 3816 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.valueList) = new ValueList; 
    memset((yyval.valueList), 0, sizeof(ValueList));
    (yyval.valueList)->floatVecList = (yyvsp[0].floatVecList); }
#line 8636 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 632:
#line 3819 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.valueList) = new ValueList; 
    memset((yyval.valueList), 0, sizeof(ValueList));
    (yyval.valueList)->floatVecList = (yyvsp[0].floatVecList); }
#line 8644 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 633:
#line 3822 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.valueList) = new ValueList; 
    memset((yyval.valueList), 0, sizeof(ValueList));
    (yyval.valueList)->floatVecList = (yyvsp[0].floatVecList); }
#line 8652 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 634:
#line 3825 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.valueList) = new ValueList; 
    memset((yyval.valueList), 0, sizeof(ValueList));
    (yyval.valueList)->intVecList = (yyvsp[0].intVecList); }
#line 8660 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 635:
#line 3831 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.valueList) = new ValueList; 
    memset((yyval.valueList), 0, sizeof(ValueList));
    (yyval.valueList)->boolVecList = (yyvsp[0].boolVecList); }
#line 8668 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 636:
#line 3834 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.valueList) = new ValueList; 
    memset((yyval.valueList), 0, sizeof(ValueList));
    (yyval.valueList)->boolVecList = (yyvsp[0].boolVecList); }
#line 8676 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 637:
#line 3837 "Grammar.ypp" /* yacc.c:1646  */
    { 
    (yyval.valueList) = NULL; yyerror("Error in value assignment\n"); 
}
#line 8684 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 638:
#line 3843 "Grammar.ypp" /* yacc.c:1646  */
    {
    LOGD("semantic %s\n", (yyvsp[0].s)->c_str() );
    (yyval.s) = (yyvsp[0].s);
}
#line 8693 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 639:
#line 3850 "Grammar.ypp" /* yacc.c:1646  */
    { 
    (yyval.ival) = -1;
}
#line 8701 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 640:
#line 3853 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.ival) = (yyvsp[-1].ival);
}
#line 8709 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 641:
#line 3864 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
}
#line 8718 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 642:
#line 3868 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
}
#line 8727 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 643:
#line 3872 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->valueList = (yyvsp[0].valueList);
}
#line 8737 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 644:
#line 3877 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->valueList = (yyvsp[0].valueList);
}
#line 8747 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 645:
#line 3882 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->semantic = (yyvsp[0].s);
}
#line 8757 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 646:
#line 3887 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->semantic = (yyvsp[-1].s);
}
#line 8767 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 647:
#line 3892 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->semantic = (yyvsp[-1].s);
    (yyval.saszsauva)->valueList = (yyvsp[0].valueList);
}
#line 8778 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 648:
#line 3898 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->semantic = (yyvsp[-2].s);
    (yyval.saszsauva)->valueList = (yyvsp[0].valueList);
}
#line 8789 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 649:
#line 3904 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[0].ival);
}
#line 8799 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 650:
#line 3909 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[-1].ival);
}
#line 8809 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 651:
#line 3914 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[-1].ival);
    (yyval.saszsauva)->valueList = (yyvsp[0].valueList);
}
#line 8820 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 652:
#line 3920 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[-2].ival);
    (yyval.saszsauva)->valueList = (yyvsp[0].valueList);
}
#line 8831 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 653:
#line 3926 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[-1].ival);
    (yyval.saszsauva)->semantic = (yyvsp[0].s);
}
#line 8842 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 654:
#line 3932 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[-2].ival);
    (yyval.saszsauva)->semantic = (yyvsp[-1].s);
}
#line 8853 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 655:
#line 3938 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[-2].ival);
    (yyval.saszsauva)->semantic = (yyvsp[-1].s);
    (yyval.saszsauva)->valueList = (yyvsp[0].valueList);
}
#line 8865 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 656:
#line 3945 "Grammar.ypp" /* yacc.c:1646  */
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[-3].ival);
    (yyval.saszsauva)->semantic = (yyvsp[-2].s);
    (yyval.saszsauva)->valueList = (yyvsp[0].valueList);
}
#line 8877 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 657:
#line 3957 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TFloat; }
#line 8883 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 658:
#line 3958 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TVec2; }
#line 8889 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 659:
#line 3959 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TVec3; }
#line 8895 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 660:
#line 3960 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TVec4; }
#line 8901 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 661:
#line 3961 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TInt; }
#line 8907 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 662:
#line 3962 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TInt2; }
#line 8913 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 663:
#line 3963 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TInt3; }
#line 8919 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 664:
#line 3964 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TInt4; }
#line 8925 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 665:
#line 3965 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TBool; }
#line 8931 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 666:
#line 3966 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TBool2; }
#line 8937 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 667:
#line 3967 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TBool3; }
#line 8943 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 668:
#line 3968 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TBool4; }
#line 8949 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 669:
#line 3969 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TMat2; }
#line 8955 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 670:
#line 3970 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TMat3; }
#line 8961 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 671:
#line 3971 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TMat4; }
#line 8967 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 672:
#line 3972 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TTexture1D; }
#line 8973 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 673:
#line 3973 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TTexture2D; }
#line 8979 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 674:
#line 3974 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TTexture2DRect; }
#line 8985 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 675:
#line 3975 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TTexture3D; }
#line 8991 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 676:
#line 3976 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TTextureCube; }
#line 8997 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 677:
#line 3977 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TTexture1D; }
#line 9003 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 678:
#line 3978 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TTexture2D; }
#line 9009 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 679:
#line 3979 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TTexture2DRect; }
#line 9015 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 680:
#line 3980 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TTexture3D; }
#line 9021 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 681:
#line 3981 "Grammar.ypp" /* yacc.c:1646  */
    { (yyval.uniformType) = nvFX::IUniform::TTextureCube; }
#line 9027 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 682:
#line 3982 "Grammar.ypp" /* yacc.c:1646  */
    { yyerror("Error : unknown type\n"); }
#line 9033 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 683:
#line 3988 "Grammar.ypp" /* yacc.c:1646  */
    {
        curAnnotations = IAnnotationEx::getAnnotationSingleton(); // need a temporary place
        curAnnotations->clear();
    }
#line 9042 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 684:
#line 3991 "Grammar.ypp" /* yacc.c:1646  */
    {
    SUniformQualifiers &q = (yyvsp[-5].uniformQualifiers);
    IUniform::Type     &t = (yyvsp[-4].uniformType);
    std::string*            &varName = (yyvsp[-3].s);
    SASzSAUva*              &a = (yyvsp[-1].saszsauva);
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);

    createUniform(t, varName->c_str(), a->semantic?a->semantic->c_str():NULL, q.global, q.precisionType);
    if(curUniform == NULL)
    {
        yyerror("(DEBUG error) curUniform == NULL");
        yyerror(varName->c_str());
    } else {
        if(a->arraySz > 1)
            curUniform->getExInterface()->setArraySz(a->arraySz);
        // Now we copy the possible annotations that were filled in arraysz_semantic_annotations_uniformvalueassigment
        curUniform->annotations()->getExInterface()->clear();
        curUniform->annotations()->getExInterface()->copy(curAnnotations);
        curAnnotations->clear();
        curAnnotations = curUniform->annotations()->getExInterface();
    }
    if(a->valueList)
    {
        if(a->valueList->floatVecList)
        {
            float *farray;
            int dim = getTypeDim(t);
            int sz = (int)a->valueList->floatVecList->size();
            if(a->arraySz <= 0)
                a->arraySz = (int)sz;
            farray = flattenFVecList(a->arraySz, dim, a->valueList->floatVecList);
            if(farray)
            {
                curUniform->setValuefv(farray, dim, a->arraySz, 0);
                delete farray;
            }
            delete a->valueList->floatVecList;
        }
        else if(a->valueList->intVecList)
        {
            int *farray;
            int dim = getTypeDim(t);
            int sz = (int)a->valueList->intVecList->size();
            if(a->arraySz <= 0)
                a->arraySz = (int)sz;
            farray = flattenIVecList(a->arraySz, dim, a->valueList->intVecList);
            if(farray)
            {
                curUniform->setValueiv(farray, dim, a->arraySz, 0);
                delete farray;
            }
            delete a->valueList->intVecList;
        }
        else if(a->valueList->boolVecList)
        {
            bool *farray;
            int dim = getTypeDim(t);
            int sz = (int)a->valueList->boolVecList->size();
            if(a->arraySz <= 0)
                a->arraySz = (int)sz;
            farray = flattenBVecList(a->arraySz, dim, a->valueList->boolVecList);
            if(farray)
            {
                curUniform->setValuebv(farray, dim, a->arraySz, 0);
                delete farray;
            }
            delete a->valueList->boolVecList;
        }
        delete a->valueList;
    }
    if(a->semantic) delete a->semantic;
    delete a;
    delete varName;
    curUniform = NULL;
    curAnnotations = NULL;
  }
#line 9125 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 706:
#line 4111 "Grammar.ypp" /* yacc.c:1646  */
    {
    LOGD("Namespace %s\n", (yyvsp[0].s)->c_str() );
    namespaceName = *(yyvsp[0].s);
    curAnnotations = IAnnotationEx::getAnnotationSingleton(1); // need a temporary place
    curAnnotations->clear();
    // keep the pointer here because the idea of namespace annotations is to copy them later
    // on every single item in the namespace
    namespaceAnnotations = curAnnotations;
}
#line 9139 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 707:
#line 4119 "Grammar.ypp" /* yacc.c:1646  */
    { 
    namespaceName.clear(); 
    curAnnotations = NULL;
    namespaceAnnotations = NULL;
    delete (yyvsp[-5].s);
}
#line 9150 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 711:
#line 4137 "Grammar.ypp" /* yacc.c:1646  */
    {
    if((yyvsp[0].global_varname_annots).varName == NULL)
        yyerror("a Constant buffer must have a name\n");
    LOGD("ConstantBuffer %s\n", (yyvsp[0].global_varname_annots).varName ? (yyvsp[0].global_varname_annots).varName->c_str() : "Un-named" );
    ICstBuffer* p = curContainer->createCstBuffer((yyvsp[0].global_varname_annots).varName ? (yyvsp[0].global_varname_annots).varName->c_str() : NULL, (yyvsp[0].global_varname_annots).global);
    // it is possible that it failed because already a Global one with same name is here
    if(p == NULL)
    {
        if((yyvsp[0].global_varname_annots).varName)
            p = pCRep->find((yyvsp[0].global_varname_annots).varName ? (yyvsp[0].global_varname_annots).varName->c_str() : NULL);
        if(p == NULL)
            yyerror("Error in Constant Buffer creation\n");
        else {
            curContainer->addCstBuffer(p);
            pCRep->addCstBuffer(p);
            bCurCstBufferAlreadyPopulated = true; // we prevent sub-uniforms to be created during this parsing
        }
    } else {
        bCurCstBufferAlreadyPopulated = false;
    }
    curCstBuffer = p ? p->getExInterface() : NULL;
    if(curCstBuffer)
    {
        IAnnotationEx* pA = curCstBuffer->annotations()->getExInterface();
        pA->copy(curAnnotations);
        // for the case of items within a namespace :
        // copy the name-space annotations to this one
        pA->copy(namespaceAnnotations);
    }

    delete (yyvsp[0].global_varname_annots).varName;
}
#line 9187 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 712:
#line 4168 "Grammar.ypp" /* yacc.c:1646  */
    {
    curCstBuffer = NULL;
    curAnnotations = NULL;
    lex_pop_state();
}
#line 9197 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 713:
#line 4179 "Grammar.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].varNameList);
    }
#line 9205 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 714:
#line 4182 "Grammar.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].varNameList);
    }
#line 9213 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 715:
#line 4185 "Grammar.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].s)
    }
#line 9221 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 716:
#line 4188 "Grammar.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].s)
    }
#line 9229 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 717:
#line 4191 "Grammar.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].s)
    }
#line 9237 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 718:
#line 4194 "Grammar.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].s)
    }
#line 9245 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 719:
#line 4197 "Grammar.ypp" /* yacc.c:1646  */
    {
}
#line 9252 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 720:
#line 4199 "Grammar.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].s)
    }
#line 9260 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 721:
#line 4202 "Grammar.ypp" /* yacc.c:1646  */
    {
    // we prefer this loop : we don't have the char ** available here... using vector of std::string...
    //for(int i=0; i<(int)$8->size(); i++)
    //    curXX->createState($4->c_str(), i, (*$8)[i].c_str());
    delete (yyvsp[-6].s);
    delete (yyvsp[-2].varNameList);
    }
#line 9272 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 722:
#line 4209 "Grammar.ypp" /* yacc.c:1646  */
    {
    //curPass->createState($4->c_str(), 0, $7->c_str());
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].s);
    }
#line 9282 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 723:
#line 4214 "Grammar.ypp" /* yacc.c:1646  */
    {
    ValueList* pvalueList = (yyvsp[-1].valueList);
    if(pvalueList && pvalueList->floatVecList)
    {
        FloatVecList *pfvl = pvalueList->floatVecList;
        FloatList* pfl = (*pfvl)[0];
        float* pf = &(*pfl)[0];
        //curPass->createState($4->c_str(), 0, pf, (int)pfl->size());
        delete pfl;
        delete pfvl;
        delete pvalueList;
    }
    delete (yyvsp[-3].s);
    }
#line 9301 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 724:
#line 4230 "Grammar.ypp" /* yacc.c:1646  */
    {
   }
#line 9308 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 725:
#line 4231 "Grammar.ypp" /* yacc.c:1646  */
    {
   }
#line 9315 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 726:
#line 4232 "Grammar.ypp" /* yacc.c:1646  */
    {
   }
#line 9322 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 728:
#line 4241 "Grammar.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].varNameList);
    }
#line 9330 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 729:
#line 4244 "Grammar.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-2].varNameList);
    }
#line 9338 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 730:
#line 4247 "Grammar.ypp" /* yacc.c:1646  */
    {
    // we prefer this loop : we don't have the char ** available here... using vector of std::string...
    //for(int i=0; i<(int)$8->size(); i++)
    //    curXX->createState($4->c_str(), i, (*$8)[i].c_str());
    delete (yyvsp[-6].s);
    delete (yyvsp[-2].varNameList);
    }
#line 9350 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 731:
#line 4254 "Grammar.ypp" /* yacc.c:1646  */
    {
    //curPass->createState($4->c_str(), 0, $7->c_str());
    delete (yyvsp[-4].s);
    delete (yyvsp[-1].s);
    }
#line 9360 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 732:
#line 4259 "Grammar.ypp" /* yacc.c:1646  */
    {
    ValueList* pvalueList = (yyvsp[-1].valueList);
    if(pvalueList && pvalueList->floatVecList)
    {
        FloatVecList *pfvl = pvalueList->floatVecList;
        FloatList* pfl = (*pfvl)[0];
        float* pf = &(*pfl)[0];
        //curPass->createState($4->c_str(), 0, pf, (int)pfl->size());
        delete pfl;
        delete pfvl;
        delete pvalueList;
    }
    delete (yyvsp[-3].s);
    }
#line 9379 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 733:
#line 4275 "Grammar.ypp" /* yacc.c:1646  */
    {
   }
#line 9386 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 734:
#line 4276 "Grammar.ypp" /* yacc.c:1646  */
    {
   }
#line 9393 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 735:
#line 4277 "Grammar.ypp" /* yacc.c:1646  */
    {
   }
#line 9400 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 737:
#line 4286 "Grammar.ypp" /* yacc.c:1646  */
    {
    //if(curResource) {
    //    if(!curResource->setFormat($4))
    //    {
    //        yyerror("Error: Bad format\n");
    //    }
    //}
}
#line 9413 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 738:
#line 4294 "Grammar.ypp" /* yacc.c:1646  */
    {
    //if(curResource)
    //    curResource->setDimensionsApplicationDependent();
}
#line 9422 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 739:
#line 4298 "Grammar.ypp" /* yacc.c:1646  */
    {
    int d = (int)(yyvsp[-2].intList)->size();
    //if(curResource)
    //    curResource->setDimensions((*$5)[0], d > 1 ? (*$5)[1] : 1, d > 2 ? (*$5)[2] : 1);
    delete (yyvsp[-2].intList);
}
#line 9433 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 740:
#line 4306 "Grammar.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[0].s);
   }
#line 9441 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 741:
#line 4308 "Grammar.ypp" /* yacc.c:1646  */
    {
   }
#line 9448 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 742:
#line 4309 "Grammar.ypp" /* yacc.c:1646  */
    {
   }
#line 9455 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 744:
#line 4318 "Grammar.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].s);
}
#line 9463 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 745:
#line 4321 "Grammar.ypp" /* yacc.c:1646  */
    {
    delete (yyvsp[-1].s);
}
#line 9471 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 746:
#line 4326 "Grammar.ypp" /* yacc.c:1646  */
    {
   }
#line 9478 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 747:
#line 4327 "Grammar.ypp" /* yacc.c:1646  */
    {
   }
#line 9485 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;

  case 748:
#line 4328 "Grammar.ypp" /* yacc.c:1646  */
    {
   }
#line 9492 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
    break;


#line 9496 "/servers/krag/hdd-home3/kansteeg/GP_Project/raceflection/sdks-source/nvFX/FxParser/Grammar.cpp" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 4333 "Grammar.ypp" /* yacc.c:1906  */

