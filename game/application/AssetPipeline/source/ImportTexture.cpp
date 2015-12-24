#include "ImportTexture.h"
#include "Package.h"

#include <assimp/texture.h>

BEGINNAMESPACE

namespace Importer {

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

		Textures textures;
		textures.push_back(texture);
		return textures;
	}

}

ENDNAMESPACE
