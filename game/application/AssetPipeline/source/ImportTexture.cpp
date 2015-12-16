#include "ImportTexture.h"
#include "Package.h"

#include <assimp/texture.h>

#include "FreeImage.h"

BEGINNAMESPACE

namespace Importer {

    Textures textureAllFromFile(const filesys::path & path)
	{
        /*
        FREE_IMAGE_FORMAT fmt = FreeImage_GetFileType(path.c_str(),0); //detect file format
        FIBITMAP* image = FreeImage_load(fmt, path.c_str()); //load from file

        if(!image) {
            return Textures();
        }

        FIBITMAP* orgImg = image;
        image = FreeImage_ConvertTo32Bits(orgImg);
        FreeImage_Unload(orgImg);

        int32 w = FreeImage_GetWidth(image);
        imt32 h = FreeImage_GetHeight(image);

        LOG_INFO(General, "Image-Extensions: %dx%d", w, h);

        union {
            uint8* buffer;
            TextureSpec* texture;
        };
        buffer = new Byte[sizeof(TextureSpec) + 4*w*h];
        Byte* pixel = (Byte*)FreeImage_GetBits(image);
        texture->m_Width = w;
        texture->m_Height = h;
        texture->m_Depth = 0;
        texture->m_DataSize = 4 * w* h;

        for(int32 i = 0; i < w*h; ++i){
            texture->m_TextureData[4*i + 0] = pixel[4*i + 2];
            texture->m_TextureData[4*i + 1] = pixel[4*i + 1];
            texture->m_TextureData[4*i + 2] = pixel[4*i + 0];
            texture->m_TextureData[4*i + 3] = pixel[4*i + 3];
        }

        Textures textures;
        textures.push_back(texture);
*/
        return Textures();
	}

}

ENDNAMESPACE
