#include "ImportTexture.h"
#include "Package.h"

#include <assimp/texture.h>

BEGINNAMESPACE

namespace Importer {

	typedef std::map<String, UUID> TextureList_t;
	TextureList_t g_TextureList;

	Textures textureAllFromFile(const filesys::path & path)
	{
		if (filesys::extension(path) != ".dds") {
			LOG_ERROR(General, "Convert the Texture to dds-format before importing it");
			return Textures();
		}

		union {
			Byte* buffer;
			TextureSpec* texture;
			UIntOfPtrSize location;
		};
		uint32 dataSize = filesys::file_size(path);
		buffer = new Byte[sizeof(TextureSpec) + dataSize];
		
		if (filesys::stem(path) == "DefaultTexture") {
			texture->uuid = { 1,1 }; //set an "known" id for the default texture to find it in the engine
		}
		else {
			texture->uuid = generateUUID();
		}
		texture->m_DataSize = dataSize;
		texture->__TextureDataLocation = location + sizeof(TextureSpec);

		IFileStream stream;
		stream.open(path.c_str(), std::ios::in | std::ios::binary);

		if (!stream.is_open()) {
			LOG_ERROR(General, "Fileopen error.");
			return Textures();
		}

		stream.read((ansichar*)texture->m_TextureData, dataSize);
		stream.close();

		String name = filesys::stem(path);
		strncpy(texture->m_TextureName, name.c_str(), TextureSpec::MaxTextureNameLength);
		g_TextureList[name] = texture->uuid;

		Textures textures;
		textures.push_back(texture);
		return textures;
	}

	UUID getTexture(const String & id)
	{
		UUID defaultId = { 1,1 };
		TextureList_t::iterator it = g_TextureList.find(id);
		if (it != g_TextureList.end()) return it->second;
		return defaultId;
	}

}

ENDNAMESPACE
