#include <ImportGeometry.h>
#include <ImportMaterial.h>

#include <functional>
#include <numeric>

#include <Package.h>
#include <Utilities/Assert.h>

#include <Utilities/Number.h>

#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/color4.h>
#include <assimp/Importer.hpp>

BEGINNAMESPACE

#define MAX_JOINTS 4

typedef std::map<UUID, GeometrySpec*> GeometryList_t;
GeometryList_t g_GeometryList;

namespace Importer {
	
#if defined BUILD_DEBUG
#	define CHECK( result, cond, message, ... ) \
	ASSERT(cond, message, __VA_ARGS__); \
	result = result && cond
#	else
#	define CHECK( result, cond, message, ... ) result = result && cond
#endif

	namespace geometry_util {


		uint32 geometryFromAiPrimitive(uint32 type)
		{
			uint32 flags = 0;
			flags |= (type & aiPrimitiveType_POINT ? PrimitiveType::Points : 0);
			flags |= (type & aiPrimitiveType_LINE ? PrimitiveType::Line : 0);
			flags |= (type & aiPrimitiveType_TRIANGLE ? PrimitiveType::Triangle : 0);
			flags |= (type & AI_PRIMITIVE_TYPE_FOR_N_INDICES(4) ? PrimitiveType::Quads : 0);
			return flags;
		}

		uint32 numberOfIndicesPerPrimitive(uint32 primitiveType) {
			switch (primitiveType) {
			case PrimitiveType::Triangle: return 3;
			case PrimitiveType::Quads: return 4;
			case PrimitiveType::Points: return 1;
			case PrimitiveType::Line: return 2;
			default: return 0;
			}
		}

		uint32 aiMeshToFlag(const aiMesh* mesh, VertexLayoutSpec* vLayout = nullptr) {
			uint32 m = 0;
			uint32 i = 0;
			if (mesh->HasPositions()) { 
				m |= VertexAttrib::Position; 
				if (vLayout) {
					vLayout->elementCount[i] = 3;
					vLayout->elementType[i] = uint32(VertexAttribType::Float32);
					++vLayout->numberOfElements;
					++i;
				}
			}
			if (mesh->HasNormals()) {
				m |= VertexAttrib::Normal;
				if (vLayout) {
					vLayout->elementCount[i] = 3;
					vLayout->elementType[i] = uint32(VertexAttribType::Float32);
					++vLayout->numberOfElements;
					++i;
				}
			}
			if (mesh->GetNumColorChannels() > 0) {
				m |= VertexAttrib::Color;
				if (vLayout) {
					for (int32 c = 0; c < mesh->GetNumColorChannels(); ++c) {
						vLayout->elementCount[i] = 4;
						vLayout->elementType[i] = uint32(VertexAttribType::Float32);
						++vLayout->numberOfElements;
						++i;
					}
				}
			}
			if (mesh->GetNumUVChannels() > 0) {
				m |= VertexAttrib::Texture;
				if (vLayout) {
					for (int32 t = 0; t < mesh->GetNumUVChannels(); ++t) {
						vLayout->elementCount[i] = mesh->mNumUVComponents[t];
						vLayout->elementType[i] = uint32(VertexAttribType::Float32);
						++vLayout->numberOfElements;
						++i;
					}
				}
			}
			if (mesh->HasTangentsAndBitangents()) {
				m |= VertexAttrib::TangentBinormal;
				if (vLayout) {
					vLayout->elementCount[i] = 3;
					vLayout->elementType[i] = uint32(VertexAttribType::Float32);
					++vLayout->numberOfElements;
					++i;
					vLayout->elementCount[i] = 3;
					vLayout->elementType[i] = uint32(VertexAttribType::Float32);
					++vLayout->numberOfElements;
					++i;
				}
			}
			if (mesh->mNumAnimMeshes > 0) {
				m |= VertexAttrib::JointIndex;
				m |= VertexAttrib::JointWeight;
			}
			return m;
		}

		// all meshes in the meshes array will be combined
		GeometrySpec* geometryInitialize(const std::vector<aiMesh*>& meshes) {
			if (meshes.size() < 1) return nullptr;
			ASSERT(meshes.size() < GeometrySpec::MaxSubMeshes, "Currently only %d submeshes per geometry are supported.", GeometrySpec::MaxSubMeshes);

			for (uint32 sub = 0; sub < meshes.size(); ++sub) {
				ASSERT(meshes[sub]->HasNormals(), "The mesh has no Normal informations, which are required");
				ASSERT(meshes[sub]->GetNumUVChannels() == 0 || meshes[sub]->HasTangentsAndBitangents(), "A mesh which contains texture coordinates should also contain (Bi)tangents");
				ASSERT(meshes[sub]->mNumVertices < std::numeric_limits<uint32>::max(), "Only support %d vertices.", std::numeric_limits<uint32>::max());
				ASSERT(Number::IsPowerOf2(meshes[sub]->mPrimitiveTypes) && meshes[sub]->mPrimitiveTypes <= AI_PRIMITIVE_TYPE_FOR_N_INDICES(4), "Primitives should have <=4 indices and only consist of one type per mesh");
			}
			//----------------------------------
			//Check wheter the submeshes are compatible
			bool compatible = true;
			for (uint32 sub = 1; sub < meshes.size(); ++sub) {
				CHECK(compatible, meshes[sub - 1]->HasPositions() == meshes[sub]->HasPositions(), "mesh %d and %d incompatible: Position", sub - 1, sub);
				CHECK(compatible, meshes[sub - 1]->HasNormals() == meshes[sub]->HasNormals(), "mesh %d and %d incompatible: Normal", sub - 1, sub);
				CHECK(compatible, meshes[sub - 1]->HasTangentsAndBitangents() == meshes[sub]->HasTangentsAndBitangents(), "mesh %d and %d incompatible: Tangent/Bitangents", sub - 1, sub);
				CHECK(compatible, meshes[sub - 1]->GetNumColorChannels() == meshes[sub]->GetNumColorChannels(), "mesh %d and %d incompatible: NumColorChannels", sub - 1, sub);
				CHECK(compatible, meshes[sub - 1]->GetNumUVChannels() == meshes[sub]->GetNumUVChannels(), "mesh %d and %d incompatible: UV-Channel", sub - 1, sub);
			}
			if (!compatible) return nullptr; //not compatible -> return nullptr
			//-----------------------------------

			//
			/// compute buffer size
			size_type memSize = sizeof(GeometrySpec);
			//Count all vertices in this Mesh (combination of all submeshes)
			uint32 vertexCount = 0;
			uint32 primitiveCount = 0;
			for (uint32 i = 0; i < meshes.size(); ++i) {
				vertexCount += meshes[i]->mNumVertices;
				primitiveCount += meshes[i]->mNumFaces;
			}
			//--
			
			uint32 vertexBufferSize = 0;
			uint32 indexBufferSize = 0;
			uint32 primitiveType = geometryFromAiPrimitive(meshes[0]->mPrimitiveTypes);
			for (uint32 i = 0; i < meshes.size(); ++i) {
				ASSERT(primitiveType == geometryFromAiPrimitive(meshes[i]->mPrimitiveTypes), "Each Mesh should only contain one type of Primitive!");
				
				vertexBufferSize += meshes[i]->mNumVertices * VertexStride(aiMeshToFlag(meshes[i]), meshes[i]->GetNumColorChannels(), meshes[i]->GetNumUVChannels(), meshes[i]->mNumUVComponents); //Storage for vertices
				indexBufferSize += meshes[i]->mNumFaces * (vertexCount <= MAX_PER_16BIT ? sizeof(uint16) : sizeof(uint32)) * numberOfIndicesPerPrimitive(primitiveType);
			}
			memSize += (vertexBufferSize + indexBufferSize);
			
			union {
				Byte* geometryAlloc;
				GeometrySpec* geometry;
			};

			geometryAlloc = new Byte[memSize]; //allocate memSize-bytes
			std::memset(geometryAlloc, 0, memSize);

			//
			/// 
			geometry->indexBufferSize = indexBufferSize;
			geometry->vertexBufferSize = vertexBufferSize;
			geometry->__VertexDataLocation = UIntOfPtrSize(geometry) + sizeof(GeometrySpec);
			geometry->__IndexDataLocation = geometry->__VertexDataLocation + geometry->vertexBufferSize;

			//fill header


			geometry->uuid = generateUUID();
			geometry->numberOfColorChannel = meshes[0]->GetNumColorChannels();
			geometry->numberOfTextureCoords = meshes[0]->GetNumUVChannels();
			geometry->numberOfSubmeshes = meshes.size();
			geometry->primitiveType = primitiveType;
			geometry->bufferUsage = BufferUsage::StaticDraw;
			geometry->numberOfVerticesPerBuffer = vertexCount;
			geometry->numberOfVertexBuffer = 1; //one per default
			geometry->vertexAttribs = aiMeshToFlag(meshes[0],&geometry->vertexLayout[0]);

			for (uint32 uvChannel = 0; uvChannel < meshes[0]->GetNumUVChannels(); ++uvChannel) {
				geometry->textureCoordComponentCount[uvChannel] = meshes[0]->mNumUVComponents[uvChannel];
			}

			for (uint32 vbIdx = 0; vbIdx < geometry->numberOfVertexBuffer; ++vbIdx) {
				geometry->vertexStride[vbIdx] = VertexStride(geometry->vertexAttribs, geometry->numberOfColorChannel, geometry->numberOfTextureCoords, geometry->textureCoordComponentCount);
			}

            uint32 startIndex = 0;
			for (uint32 subMeshIdx = 0; subMeshIdx < meshes.size(); ++subMeshIdx) {
                geometry->indices[subMeshIdx].startIndex = startIndex;
                uint32 numIndices =  meshes[subMeshIdx]->mNumFaces * numberOfIndicesPerPrimitive(primitiveType);
                geometry->indices[subMeshIdx].indexCount = numIndices;
                startIndex += numIndices;
            }
			return geometry; //return initialized mesh -> the data slots need to be filled
		}

		typedef struct {
			uint32 index;
			uint32 lastVertex;
			uint32 lastIndex;
		} GeometryOffset;

        void geometryCopy(GeometrySpec* geometry, const aiMesh* _aiMesh, uint32 numMeshes, const aiMatrix4x4& transformation = aiMatrix4x4(), bool globalCoords = true) {
			
			aiMatrix4x4 invertTranspTransform = transformation;
			invertTranspTransform.Inverse().Transpose();

			uint32 vertexIdx = 0;
			union {
				float32* vertices;
				Byte* vertexAsByte;
			};
			vertexAsByte = geometry->vertexData;

			aiVector3D globalMinAABB(std::numeric_limits<float32>::max(), std::numeric_limits<float32>::max(), std::numeric_limits<float32>::max());
			aiVector3D globalMaxAABB(std::numeric_limits<float32>::min(), std::numeric_limits<float32>::min(), std::numeric_limits<float32>::min());

			for (uint32 subMesh = 0; subMesh < numMeshes; ++subMesh) {
				const aiMesh* aiMesh = (_aiMesh + subMesh);

				aiVector3D minAABB(std::numeric_limits<float32>::max(), std::numeric_limits<float32>::max(), std::numeric_limits<float32>::max());
				aiVector3D maxAABB(std::numeric_limits<float32>::min(), std::numeric_limits<float32>::min(), std::numeric_limits<float32>::min());
				//
				/// copy vertex data (position, normals, Color, TexCoords,...)
				for (uint32 vertIdx = 0; vertIdx < _aiMesh[subMesh].mNumVertices; ++vertIdx) {
					//?! Position data
					if ((geometry->vertexAttribs & VertexAttrib::Position) != 0) {
						aiVector3D position = _aiMesh[subMesh].mVertices[vertIdx];
                        if (globalCoords)
                            position = transformation * position;
						vertices[vertexIdx++] = position.x;
						vertices[vertexIdx++] = position.y;
						vertices[vertexIdx++] = position.z;

						//update AABB
						if (position.x < minAABB.x) minAABB.x = position.x;
						if (position.y < minAABB.y) minAABB.y = position.y;
						if (position.z < minAABB.z) minAABB.z = position.z;
						if (position.x > maxAABB.x) maxAABB.x = position.x;
						if (position.y > maxAABB.y) maxAABB.y = position.y;
						if (position.z > maxAABB.z) maxAABB.z = position.z;
					}

					//?! Normal data
					if ((geometry->vertexAttribs & VertexAttrib::Normal) != 0) {
						aiVector3D normal = _aiMesh[subMesh].mNormals[vertIdx];
                        if (globalCoords)
                            normal = invertTranspTransform * normal;
						vertices[vertexIdx++] = normal.x;
						vertices[vertexIdx++] = normal.y;
						vertices[vertexIdx++] = normal.z;
					}


					//?! Color Data
					if ((geometry->vertexAttribs & VertexAttrib::Color) != 0) {
						for (uint32 colorChannel = 0; colorChannel < _aiMesh[subMesh].GetNumColorChannels(); ++colorChannel) {
							vertices[vertexIdx++] = _aiMesh[subMesh].mColors[colorChannel][vertIdx].r;
							vertices[vertexIdx++] = _aiMesh[subMesh].mColors[colorChannel][vertIdx].g;
							vertices[vertexIdx++] = _aiMesh[subMesh].mColors[colorChannel][vertIdx].b;
							vertices[vertexIdx++] = _aiMesh[subMesh].mColors[colorChannel][vertIdx].a;
						}
					}

					//?! Texture Data
					if ((geometry->vertexAttribs & VertexAttrib::Texture) != 0) {
						for (uint32 textureChannel = 0; textureChannel < _aiMesh[subMesh].GetNumUVChannels(); ++textureChannel) {
							vertices[vertexIdx++] = _aiMesh[subMesh].mTextureCoords[textureChannel][vertIdx].x;
							if (geometry->textureCoordComponentCount[textureChannel] > 1)
								vertices[vertexIdx++] = _aiMesh[subMesh].mTextureCoords[textureChannel][vertIdx].y;
							if (geometry->textureCoordComponentCount[textureChannel] > 2)
								vertices[vertexIdx++] = _aiMesh[subMesh].mTextureCoords[textureChannel][vertIdx].z;
						}
					}

					//?! Tangent-Bitangent Data
					if ((geometry->vertexAttribs & VertexAttrib::TangentBinormal) != 0) {
						aiVector3D tangent = _aiMesh[subMesh].mTangents[vertIdx];
                        if (globalCoords) tangent = invertTranspTransform * tangent;
						aiVector3D bitangent = _aiMesh[subMesh].mBitangents[vertIdx];
                        if (globalCoords) bitangent = invertTranspTransform * bitangent;
						vertices[vertexIdx++] = tangent.x;
						vertices[vertexIdx++] = tangent.y;
						vertices[vertexIdx++] = tangent.z;
						vertices[vertexIdx++] = bitangent.x;
						vertices[vertexIdx++] = bitangent.y;
						vertices[vertexIdx++] = bitangent.z;
					}

					//?! Animation Data					
					// Copy Animation Data here...!
					//
				}

				//> insert AABB data
				geometry->boundingVolume[subMesh + 1].minAABB[0] = minAABB.x;
				geometry->boundingVolume[subMesh + 1].minAABB[1] = minAABB.y;
				geometry->boundingVolume[subMesh + 1].minAABB[2] = minAABB.z;
				geometry->boundingVolume[subMesh + 1].maxAABB[0] = maxAABB.x;
				geometry->boundingVolume[subMesh + 1].maxAABB[1] = maxAABB.y;
				geometry->boundingVolume[subMesh + 1].maxAABB[2] = maxAABB.z;


				if(globalMinAABB.x > minAABB.x)
					globalMinAABB.x = minAABB.x;
				if (globalMinAABB.y > minAABB.y)
					globalMinAABB.y = minAABB.y;
				if (globalMinAABB.z > minAABB.z)
					globalMinAABB.z = minAABB.z;

				if (globalMaxAABB.x < minAABB.x)
					globalMaxAABB.x = maxAABB.x;
				if (globalMaxAABB.y < maxAABB.y)
					globalMaxAABB.y = maxAABB.y;
				if (globalMaxAABB.z < maxAABB.z)
					globalMaxAABB.z = maxAABB.z;

				union {
					uint16* indices16;
					uint32* indices32;
					void* indicesAsVoid;
				};
				indicesAsVoid = geometry->indexData;

				//
				/// copy face data (indices)
				uint32 idxIdx = 0;
				for (uint32 faceIdx = 0; faceIdx < _aiMesh[subMesh].mNumFaces; ++faceIdx) {
					const aiFace* face = _aiMesh[subMesh].mFaces + faceIdx;
					for (uint32 indexIdx = 0; indexIdx < face->mNumIndices; ++indexIdx) {
						if (geometry->numberOfVerticesPerBuffer * geometry->numberOfVertexBuffer <= MAX_PER_16BIT)
							indices16[idxIdx++] = uint16(face->mIndices[indexIdx]);
						else
							indices32[idxIdx++] = uint32(face->mIndices[indexIdx]);
					}
				}

				//
				/// copy material ref
				//mesh->data.materials[subMesh] = Importer::materialByIndex(_aiMesh[subMesh].mMaterialIndex);
			}
			geometry->boundingVolume[0].minAABB[0] = globalMinAABB.x;
			geometry->boundingVolume[0].minAABB[1] = globalMinAABB.y;
			geometry->boundingVolume[0].minAABB[2] = globalMinAABB.z;
			geometry->boundingVolume[0].maxAABB[0] = globalMaxAABB.x;
			geometry->boundingVolume[0].maxAABB[1] = globalMaxAABB.y;
			geometry->boundingVolume[0].maxAABB[2] = globalMaxAABB.z;

			//
			/// store
			g_GeometryList[geometry->uuid] = geometry;
		}
	}

    GeometrySpec* geometryFromMeshVec(const std::vector<aiMesh*>& subMeshes, const aiMatrix4x4 &mat, bool globalCoords) {
		GeometrySpec* geometry = geometry_util::geometryInitialize(subMeshes);
        geometry_util::geometryCopy(geometry, subMeshes[0], subMeshes.size(), mat, globalCoords);
		return geometry;
	}

	GeometrySpec * geometryByUUID(UUID uuid)
	{
		GeometryList_t::iterator it = g_GeometryList.find(uuid);
		if (it == g_GeometryList.end()) return nullptr;
		return it->second;
	}

}

ENDNAMESPACE

