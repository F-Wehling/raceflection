#include <ImportLight.h>

#include <queue>
BEGINNAMESPACE

namespace Importer {

	const aiNode* _recFind(const aiScene* scene, const aiLight* light, const aiNode* node) {
		if (node->mName == light->mName) { //we found the correct node
			return node;
		}
		for (int32 i = 0; i < node->mNumChildren; ++i) {
			const aiNode* rnode = _recFind(scene, light, node->mChildren[i]);
			if (rnode != nullptr) return rnode;
		}
		return nullptr;
	}

	bool findLightParameter(const aiScene* scene, const aiLight* light, aiVector3D& out_position, aiVector3D& out_direction) {
		std::deque<const aiNode*> queue;
		const aiNode* node = _recFind(scene, light, scene->mRootNode);
		if (node != nullptr) {
			while (node != nullptr) {
				queue.push_front(node);
				node = node->mParent;
			}
			aiMatrix4x4 mat;
			if (queue.size() > 2 && String(queue[1]->mName.C_Str()).find("$AssimpFbx$") != String::npos) { //UGLY: but this is fbx -> other node layout (... don't know why...)
				{
					StringStream s;
					s << "RootNode";
					for (const aiNode* h : queue) {
						if (s.str() == h->mName.C_Str()) {
							mat = h->mTransformation * mat;
							break;
						}
					}
				}
				{
					StringStream s;
					s << light->mName.C_Str() << "_$AssimpFbx$_" << "Rotation";
					for (const aiNode* h : queue) {
						if (s.str() == h->mName.C_Str()) {
							mat = h->mTransformation * mat;
							break;
						}
					}
				}
				{
					StringStream s;
					s << light->mName.C_Str() << "_$AssimpFbx$_" << "Translation";
					for (const aiNode* h : queue) {
						if (s.str() == h->mName.C_Str()) {
							mat = h->mTransformation * mat;
							break;
						}
					}
				}
				{
					StringStream s;
					s << light->mName.C_Str() << "_$AssimpFbx$_" << "PostRotation";
					for (const aiNode* h : queue) {
						if (s.str() == h->mName.C_Str()) {
							mat = h->mTransformation * mat;
							break;
						}
					}
				}
				{
					StringStream s;
					s << light->mName.C_Str();
					for (const aiNode* h : queue) {
						if (s.str() == h->mName.C_Str()) {
							mat = h->mTransformation * mat;
							break;
						}
					}
				}
				aiMatrix4x4 scl;	aiMatrix4x4::Scaling(aiVector3D(1.0, 1.0, -1.0), scl);
				mat = scl * mat;
			}
			else { //other 
				for (const aiNode* h : queue) {
					mat = h->mTransformation * mat;
				}
			}
			out_position = aiVector3D();
			out_position = mat * out_position;
			aiMatrix3x3 matIT = aiMatrix3x3(mat);
			//matIT.Inverse().Transpose();
			out_direction = matIT * out_direction;
			if(out_direction.Length() > 0)
				out_direction.Normalize();
		
			if (std::abs(out_direction.x) < 0.0001) out_direction.x = 0.0f;
			if (std::abs(out_direction.y) < 0.0001) out_direction.y = 0.0f;
			if (std::abs(out_direction.z) < 0.0001) out_direction.z = 0.0f;
			return true;
		}
		return false;
	}

    Lights lightsAllFromScene(const aiScene * scene)
	{
		const aiNode* node = scene->mRootNode;
		
		Lights lights;

		for (int32 i = 0; i < scene->mNumLights; ++i) {
			aiLight* light = scene->mLights[i];
			
			light->mName;

			LightSpec* spec = new LightSpec; //a light has no dynamic data -> no need to use bytes
			std::memset(spec, 0, sizeof(LightSpec));
			spec->uuid = generateUUID();

			aiVector3D l_direction;
			switch (light->mType) {
				case aiLightSourceType::aiLightSource_POINT:
					spec->type = (uint32)LightType::Point;
					break;
				case aiLightSourceType::aiLightSource_DIRECTIONAL:
					spec->type = (uint32)LightType::Directional;
					l_direction = aiVector3D(0, 0, -1);
					break;
				case aiLightSourceType::aiLightSource_SPOT:
					spec->type = (uint32)LightType::Spot;
					l_direction = aiVector3D(0, 0, -1);
					break;
				case aiLightSourceType::aiLightSource_AMBIENT:
					spec->type = (uint32)LightType::Ambient;
					break;
				case aiLightSourceType::aiLightSource_UNDEFINED:
					delete spec;
					continue;
					break;
				default:
					break;
			}

			aiVector3D l_position;
			findLightParameter(scene, light, l_position, l_direction);
			l_position += light->mPosition; //offset from node (normaly 0 0 0)
			l_direction += light->mDirection;
			std::memcpy(spec->position, &l_position[0], sizeof(aiVector3D));
			std::memcpy(spec->direction, &l_direction[0], sizeof(aiVector3D));
			std::memcpy(spec->diffuse, &light->mColorDiffuse[0], sizeof(aiColor3D));
			std::memcpy(spec->specular, &light->mColorSpecular[0], sizeof(aiColor3D));
			std::memcpy(spec->ambient, &light->mColorAmbient[0], sizeof(aiColor3D));
			spec->attenuationConstant = light->mAttenuationConstant != 0.0 ? light->mAttenuationConstant : 1.0; //some default values to make some attenuation available
			spec->attenuationLinear = light->mAttenuationLinear != 1.0 ? light->mAttenuationLinear : 0.0;
			spec->attenuationQuadratic = light->mAttenuationQuadratic != 1.0 ? light->mAttenuationQuadratic : 0.0;
			spec->angleInnerCone = light->mAngleInnerCone;
			spec->angleOuterCone = light->mAngleOuterCone;

			for (uint32 i = 0; i < 3; ++i) {
				spec->diffuse[i] /= 255;
				spec->specular[i] /= 255;
				spec->ambient[i] /= 255;
			}

			lights.push_back(spec);
		}
		return lights;
	}
	Lights lightsAllFromFile(const filesys::path & file)
	{
		return Lights();
	}
}

ENDNAMESPACE
