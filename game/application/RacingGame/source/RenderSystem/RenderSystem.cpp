#include "RenderSystem/RenderSystem.h"
#include "RenderSystem/HardwareResourcePool.h"

#include <ACGL/ACGL.hh>
#include <ACGL/OpenGL/Objects.hh>

#include <Logging/Logging.h>

BEGINNAMESPACE

Byte* gRenderSystemMemory = nullptr;

RenderSystem::RenderSystem() : m_Allocator("RenderSystemAllocator")
{
	if (gRenderSystemMemory) {
		LOG_WARNING(Renderer, "All Renderer instances share their workspace-memory.");
		return;
	}
	gRenderSystemMemory = eng_new_array(Byte[sSystemMemorySize]);
	m_Allocator.initialize(gRenderSystemMemory, sSystemMemorySize);
}

RenderSystem::~RenderSystem()
{
	shutdown();
	if (gRenderSystemMemory) {
		eng_delete_array(gRenderSystemMemory);
	}
}

bool RenderSystem::initialize()
{
	if (!ACGL::init()) {
		LOG_ERROR(Renderer, "ACGL Initialization failed.");
		return false;
	}
	
	m_ResourcePool = eng_new(HardwareResourcePool, m_Allocator);

	/*
	m_OgreSystem = eng_new(Root, m_Allocator)("", ""); //The Renderer is based on OGRE. 

	try {

		const RenderSystemList& renderSystems = m_OgreSystem->getAvailableRenderers();

#	if DEBUG_BUILD
		m_OgreSystem->loadPlugin("RenderSystem_GL_d");
#	else
		m_OgreSystem->loadPlugin("RenderSystem_GL");
#	endif

		/* //To choose by name
		RenderSystemList::const_iterator rensys_it = renderSystems.begin();
		RenderSystemList::const_iterator rensys_end = renderSystems.end();

		for (; rensys_it != rensys_end; ++rensys_it) {
			Ogre::RenderSystem* rensys = (*rensys_it);
			rensys->getName();
			if(rensys->getName() == "NAME"){
				m_OgreSystem->setRenderSystem(rensys);
				break;
			}
		}
		//* /

		//use first render system found
		if (renderSystems.empty()) {
			LOG_ERROR(Renderer, "No RenderSystem available.");
			return false;
		}

		m_OgreSystem->setRenderSystem(renderSystems[0]);

		m_OgreSystem->initialise(false);
		m_OgreRenderWindow = m_OgreSystem->createRenderWindow("Racing Game", 1024, 768, false);

		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	
		m_OgreSceneManager = m_OgreSystem->createSceneManager(ST_GENERIC);
		m_OgreCamera = m_OgreSceneManager->createCamera("MainCamera"); 
		m_OgreCamera->setPosition(Ogre::Vector3(0, 300, 500));
		m_OgreCamera->lookAt(Ogre::Vector3(0, 0, 0));
		m_OgreViewport = m_OgreRenderWindow->addViewport(m_OgreCamera);
		m_OgreViewport->setBackgroundColour(Ogre::ColourValue(0.5, 0.5, .5, 1.0));
		m_OgreCamera->setAspectRatio(m_OgreViewport->getActualWidth() / m_OgreViewport->getActualHeight());

		//
		/// simple examples
		Plane plane(Vector3::UNIT_Y, 0.0);
		MeshManager::getSingleton().createPlane("Ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, 1500, 1500, 20, 20, true, 1, 5, 5, Vector3::UNIT_Z);
		Entity* groundEntity = m_OgreSceneManager->createEntity("Ground");
		m_OgreSceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);

		//cube
		Ogre::MeshPtr mesh = MeshManager::getSingleton().createManual("ColorCube", "General");

		SubMesh* sub = mesh->createSubMesh();

		const float32 nc = 0.577350269f;
		const size_type nVertices = 8;
		const size_type vbufCount = 3 * 2 * nVertices;
		float32 vertices[vbufCount] = {
			-100.0, 100.0, -100.0, -nc, nc, -nc, //vertex 0
			100.0, 100.0, -100.0, nc, nc, -nc, //vertex 1
			100.0, -100.0, -100.0, nc, -nc, -nc, //vertex 2
			-100.0, -100.0, -100.0, -nc, -nc, -nc, //vertex 3
			-100.0, 100.0, 100.0, -nc, nc, nc, //vertex 4
			100.0, 100.0, 100.0, nc, nc, nc, //vertex 5
			100.0, -100.0, 100.0, nc, -nc, nc, //vertex 6
			-100.0, -100.0, 100.0, -nc, -nc, nc //vertex 7
		};

		Ogre::RenderSystem* rs = m_OgreSystem->getRenderSystem();

		RGBA colors[nVertices], *pColor = colors;

		rs->convertColourValue(ColourValue(1.0, 0.0, 0.0), pColor++); //color 0
		rs->convertColourValue(ColourValue(0.0, 1.0, 0.0), pColor++); //color 1
		rs->convertColourValue(ColourValue(1.0, 0.0, 1.0), pColor++); //color 2
		rs->convertColourValue(ColourValue(0.0, 1.0, 1.0), pColor++); //color 3
		rs->convertColourValue(ColourValue(0.0, 0.0, 1.0), pColor++); //color 4
		rs->convertColourValue(ColourValue(1.0, 1.0, 0.0), pColor++); //color 5
		rs->convertColourValue(ColourValue(1.0, 1.0, 0.0), pColor++); //color 6
		rs->convertColourValue(ColourValue(1.0, 0.0, 1.0), pColor++); //color 7

		const size_type ibufCount = 36;
		uint16 faces[ibufCount] = {
			0,2,3,
			0,1,2,
			1,6,2,
			1,5,6,
			4,6,5,
			4,7,6,
			0,7,4,
			0,3,7,
			0,5,1,
			0,4,5,
			2,7,3,
			2,6,7
		};
	
		mesh->sharedVertexData = new VertexData();
		mesh->sharedVertexData->vertexCount = nVertices;

		VertexDeclaration* decl = mesh->sharedVertexData->vertexDeclaration;
		size_type offset = 0;
		decl->addElement(0, offset, VET_FLOAT3, VES_POSITION, 0);
		offset += VertexElement::getTypeSize(VET_FLOAT3);
		decl->addElement(0, offset, VET_FLOAT3, VES_NORMAL, 0);
		offset += VertexElement::getTypeSize(VET_FLOAT3);

		HardwareVertexBufferSharedPtr vbuf = HardwareBufferManager::getSingleton().createVertexBuffer(offset, nVertices, HardwareBuffer::HBU_STATIC_WRITE_ONLY);
		vbuf->writeData(0, vbuf->getSizeInBytes(), vertices, true);

		VertexBufferBinding* bind = mesh->sharedVertexData->vertexBufferBinding;
		bind->setBinding(0, vbuf);

		offset = 0;
		decl->addElement(1, offset, VET_COLOUR, VES_DIFFUSE);
		offset += VertexElement::getTypeSize(VET_COLOUR);

		vbuf = HardwareBufferManager::getSingleton().createVertexBuffer(offset, mesh->sharedVertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY);
		vbuf->writeData(0, vbuf->getSizeInBytes(), colors, true);

		bind->setBinding(1, vbuf);

		HardwareIndexBufferSharedPtr ibuf = HardwareBufferManager::getSingleton().createIndexBuffer(HardwareIndexBuffer::IT_16BIT, ibufCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY);
		ibuf->writeData(0, ibuf->getSizeInBytes(), faces, true);

		sub->useSharedVertices = true;
		sub->indexData->indexBuffer = ibuf;
		sub->indexData->indexCount = ibufCount;
		sub->indexData->indexStart = 0;

		mesh->_setBounds(AxisAlignedBox(-100, -100, -100, 100, 100, 100));
		mesh->_setBoundingSphereRadius(Math::Sqrt(3 * 100 * 100));

		mesh->load();

		//
		/// define a material
		const ansichar* mat_def =
			"material test \n"
			"{ \n"
			"    technique { \n"
			"        pass { \n"
			"            lighting off \n"
			"            ambient 0.0 1.0 1.0 1.0 \n"
			"            diffuse 0.0 1.0 1.0 1.0 \n"
			"        }\n"
			"    }\n"
			"}";
		
		ScriptCompiler* scriptcompiler = new ScriptCompiler();
		if (!scriptcompiler->compile(mat_def, "source", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME)) {
			LOG_ERROR(Renderer, "error");
			return false;
		}
		MaterialPtr material = MaterialManager::getSingleton().getByName("test");
		
		///
		//

		Entity* thisEntity = m_OgreSceneManager->createEntity("cc", "ColorCube");
		thisEntity->setMaterial(material);
		SceneNode* thisSceneNode = m_OgreSceneManager->getRootSceneNode()->createChildSceneNode();
		thisSceneNode->setPosition(-35, 0, 0);
		thisSceneNode->attachObject(thisEntity);

		groundEntity->setMaterial(material);
	}

	catch (Exception& e) {
		LOG_ERROR(Renderer, "Ogre initialization failed: %s", e.getFullDescription().c_str());
		return false;
	}
	*/
	return true;
}

void RenderSystem::shutdown()
{
	eng_delete(m_ResourcePool, m_Allocator);
}

bool RenderSystem::tick(float32 dt)
{
	return true;
}

ENDNAMESPACE

