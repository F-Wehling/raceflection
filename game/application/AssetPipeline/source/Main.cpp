#include <Main.h>
#include <PackageManager.h>

#include <Logging/Logging.h>

#include <thread>

#include <Configuration/ConfigSettings.h>

BEGINNAMESPACE

ConfigSettingAnsichar cfgConfigFile("ConfigFile", "Sets the configuration filename", "config.cfg");
ConfigSettingAnsichar cfgPackageRoot("PackageRoot", "Set the location of the configuration files", "resource/packages/");
ConfigSettingAnsichar cfgPathPrefix("PathPrefix", "Set the prefix for all paths", "./");

bool RunApplication(class GenericApplication*) {
	return false;
}

bool RunApplication(int32 argc, const ansichar* argv[]) {

	path package_dir;

	if (argc < 2) {
		//Use the current directory as resource directory
	
		parseConfigFile(cfgConfigFile, true);

		package_dir = (const ansichar*)cfgPathPrefix;
		package_dir /= "resource";
		String dir = package_dir.string();
		LOG_WARNING(General, "The resource directory wasn't specified. Use the current directory: %s", dir.c_str());
	}
	else {
		package_dir = argv[1];
		if (!is_directory(package_dir)) {
			String dir = package_dir.string();
			LOG_ERROR(General, "The specified directory (%s) is not a valid directory.", dir.c_str());
			return false;
		}
	}

	PackageManager manager(package_dir);

	manager.loadPackages(); //load all existing packages

	while (!manager.shutdown()) {
		manager.tick(); //check all packages
		std::this_thread::sleep_for(std::chrono::milliseconds(500)); //sleep a bit
	}

	return true;
}

ENDNAMESPACE

int main(int argc, const char* argv[]) {
	return NS_NAME::RunApplication(argc, argv);
}