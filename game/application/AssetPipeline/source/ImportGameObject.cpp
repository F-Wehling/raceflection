#include "ImportGameObject.h"

#include "Logging/Logging.h"
#include "ImportMesh.h"
#include "ImportPhysics.h"

#include <algorithm>

BEGINNAMESPACE

namespace Importer {

    GameObjects gameObjectsAllFromFile(const filesys::path& file) {
        GameObjects obj;
		//Read in source
		IFileStream stream(file.c_str(), std::ios::in);
		if (!stream.is_open()) {
			LOG_ERROR(General, "Cant open file for reading (%s).", file.c_str());
            return obj;
        }

        String line;
        while(std::getline(stream, line)){
            std::transform(line.begin(), line.end(), line.begin(), ::tolower);
            if(line.compare("start obj") == 0){
                obj.push_back(gameObjectFromStream(stream));
            }
        }
        stream.close();
        return obj;
    }

    GameObjectSpec* gameObjectFromStream(IFileStream& stream){
        std::string mesh, physics;
        GameObjectFlagsFlags flags = GameObjectFlags::Enum(0);
        GameObjectSpec* gameObject = new GameObjectSpec;
        gameObject->position[0] = gameObject->position[1] = gameObject->position[2] = 0.0;
        gameObject->rotation[0] = gameObject->rotation[1] = gameObject->rotation[2] = gameObject->rotation[3] = 0.0;
        gameObject->scaling[0] = gameObject->scaling[1] = gameObject->scaling[2] = 0.0;

        String line;
        while(std::getline(stream, line)){
            if(line.compare("end obj") == 0)
                break;
            size_t eq_pos = line.find("=");
            if(eq_pos == std::string::npos || eq_pos >= line.length() - 1)
                continue;
            std::string identifier = line.substr(0, eq_pos);
            std::string data = line.substr(eq_pos + 1);
            std::transform(identifier.begin(), identifier.end(), identifier.begin(), ::tolower);
            std::transform(data.begin(), data.end(), data.begin(), ::tolower);
            if(identifier.compare("position") == 0){
                std::stringstream dataS(data);
                for(int i = 0; i < 3; ++i){
                    std::string f;
                    std::getline(dataS, f, ',');
                    gameObject->position[i] = std::stof(f);
                }
            } else if(identifier.compare("rotation") == 0){
                std::stringstream dataS(data);
                for(int i = 0; i < 4; ++i){
                    std::string f;
                    std::getline(dataS, f, ',');
                    gameObject->rotation[i] = std::stof(f);
                }
            } else if(identifier.compare("scaling") == 0){
                std::stringstream dataS(data);
                for(int i = 0; i < 3; ++i){
                    std::string f;
                    std::getline(dataS, f, ',');
                    gameObject->scaling[i] = std::stof(f);
                }
            } else if(identifier.compare("triggerarea") == 0){
                if(std::stof(data) == 1)
                    flags.set(GameObjectFlags::TriggerArea);
                else if(std::stof(data) == 0)
                    flags.unset(GameObjectFlags::TriggerArea);
            } else if(identifier.compare("mesh") == 0){
                mesh = data;
            } else if(identifier.compare("physics") == 0){
                physics = data;
            }
        }

        gameObject->uuid = generateUUID();
        PhysicsSpec* phySpec = physicsByFileName(physics);
        if(phySpec != nullptr){
            gameObject->physicsId = phySpec->uuid;
            flags.set(GameObjectFlags::Physics);
        } else {
            flags.unset(GameObjectFlags::Physics);
        }
        MeshSpec* meshSpec = meshByName(mesh);
        if(meshSpec != nullptr){
            gameObject->meshId = meshSpec->uuid;
            flags.set(GameObjectFlags::Mesh);
        } else {
            flags.unset(GameObjectFlags::Mesh);
        }
        gameObject->flags = flags;

        return gameObject;
    }

}

ENDNAMESPACE
