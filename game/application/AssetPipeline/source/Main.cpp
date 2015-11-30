#include <Main.h>
#include <Package/PackageManager.h>

#include <Logging/Logging.h>

#include <thread>

BEGINNAMESPACE

bool RunApplication(class GenericApplication*) {
	return false;
}

bool RunApplication(int32 argc, const ansichar* argv[]) {

	path package_dir;

	if (argc < 2) {
		//Use the current directory as resource directory
		package_dir = current_path();
		LOG_WARNING(General, "The resource directory wasn't specified. Use the current directory: %s", package_dir.c_str());
	}
	else {
		package_dir = argv[1];
		if (!is_directory(package_dir)) {
			LOG_ERROR(General, "The specified directory (%s) is not a valid directory.", package_dir.c_str());
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