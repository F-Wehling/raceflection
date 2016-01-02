#include "ImportEffect.h"

#include "Logging/Logging.h"
BEGINNAMESPACE

namespace Importer {

	String slurp(IStream& in) {
		return static_cast<StringStream const&>(StringStream() << in.rdbuf()).str();
	}

	Effects effectAllFromFile(const filesys::path& file) {

		filesys::path ending[3] = { "material_fx", "scene_fx", "fxh" };

		uint32 effecttype = 0;
		if (std::equal(ending[0].rbegin(), ending[0].rend(), file.rbegin())) {
			effecttype = EffectType::MATERIAL_FX;
		}
		else if (std::equal(ending[1].rbegin(), ending[1].rend(), file.rbegin())) {
			effecttype = EffectType::SCENE_FX;
		}
		else if (std::equal(ending[2].rbegin(), ending[2].rend(), file.rbegin())) {
			effecttype = EffectType::FXHeader;
		}
		else {
			//< this file will not contain an effect -> but we do not print an error, because the underlying filesystem-iterator can't decide so 
			//< this function will be called for each file in the directory
			return Effects();
		}

		Effects eff;
		//Read in source
		IFileStream stream(file.c_str(), std::ios::in);
		if (!stream.is_open()) {
			LOG_ERROR(General, "Cant open file for reading (%s).", file.c_str());
			return eff;
		}
		String source = slurp(stream);

		uint32 bufferSize = sizeof(EffectSpec) + (source.size() + 1) * sizeof(String::value_type);

		union {
			Byte* eff_buffer;
			EffectSpec* effect;
			UIntOfPtrSize location;
		};
		eff_buffer = new Byte[bufferSize];
		std::memset(eff_buffer, 0, bufferSize);
		effect->__effectSourceLocation = location + sizeof(EffectSpec);
		effect->effectType = effecttype;

		effect->uuid = generateUUID();
		filesys::path name = filesys::stem(file) + filesys::extension(file);
		std::strncpy(effect->name, name.c_str(), EffectSpec::MaxEffectName);
		std::strncpy(effect->effectSource, source.c_str(), source.length());

		//LOG_INFO(General, "Source-length: %d Bytes", std::strlen(effect->effectSource));

		eff.push_back(effect);
		return eff;
	}

}

ENDNAMESPACE