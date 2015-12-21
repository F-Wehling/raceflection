#include <Configuration/Configuration.h>

BEGINNAMESPACE

extern ConfigSettingAnsichar cfgPathPrefix;

void parseConfigFile(const ansichar* configFile, bool setPrefix /* = false */) {

	IFileStream ifile;
	
	ansichar temp[3][256];

	strcpy(temp[0], "../");
	strcpy(temp[1], configFile);
	strcpy(temp[2], "");

	int32 i = 0, j = 0;
	while(true){		
		ifile.open((const char*)temp[(i + 1) % 2], std::ios::in);
		if (ifile.is_open()) break; //okay found
		
		strcat(temp[i], temp[(i + 1) % 2]);
		strcat(temp[2], "../");
		i = (i + 1) % 2;
		strcpy(temp[i], "../");
		if (++j >= 20) return;
	} 

	if (setPrefix) 
		cfgPathPrefix = temp[2];

	ansichar line[256];
	ansichar name[256];
	ansichar type;
	std::memset(line, 0, sizeof(ansichar) * 256);
	std::memset(name, 0, sizeof(ansichar) * 256);

	while (ifile.getline((char*)&line[0], 256)) {
		//parse line 
		switch (line[0]) {
		case 'u':
		{
			//line will contain an unsigned int32
			uint32 value = 0;
			std::sscanf(line, "%c%s%u", &type, name, &value);

			ConfigSettingUint32* setting = ConfigSettingUint32::FindSetting(name);
			if (setting != nullptr) {
				*setting = value;
			}
		}
		break;
		case 'i':
		{
			//line will contain an signed int32
			int32 value = 0;
			std::sscanf(line, "%c%s%d", &type, name, &value);

			ConfigSettingInt32* setting = ConfigSettingInt32::FindSetting(name);
			if (setting != nullptr) {
				*setting = value;
			}
		}
		break;
		case 'f':
		{
			//line will contain an float32
			float32 value = 0;
			std::sscanf(line, "%c%s%f", &type, name, &value);

			ConfigSettingFloat32* setting = ConfigSettingFloat32::FindSetting(name);
			if (setting != nullptr) {
				*setting = value;
			}
		}
		break;
		case 'b':
		{
			//line will contain a bool
			bool b = true;
			ansichar value[128];
			std::memset(value, 0, sizeof(ansichar) * 128);
			std::sscanf(line, "%c%s%s", &type, name, &value);
			b = strcmp(value, "false") == 0 || strcmp(value, "no") == 0 || strcmp(value, "off") == 0 || strcmp(value, "0");

			ConfigSettingBool* setting = ConfigSettingBool::FindSetting(name);
			if (setting != nullptr) {
				*setting = b;
			}
		}
		case 's':
		{
			ansichar value[512];
			std::memset(value, 0, sizeof(ansichar) * 512);
			std::sscanf(line, "%c%s%s", &type, name, &value);

			ConfigSettingAnsichar* setting = ConfigSettingAnsichar::FindSetting(name);
			if (setting != nullptr) {
				*setting = value;
			}
        }
		break;
		}
	}
}

ENDNAMESPACE
