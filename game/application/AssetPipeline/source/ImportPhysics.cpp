#include "ImportPhysics.h"

#include "Logging/Logging.h"
BEGINNAMESPACE

namespace Importer {

	String slurp(IStream& in) {
		return static_cast<StringStream const&>(StringStream() << in.rdbuf()).str();
	}

    Physics physicsAllFromFile(const filesys::path& file) {
        Physics phy;
		//Read in source
		IFileStream stream(file.c_str(), std::ios::in);
		if (!stream.is_open()) {
			LOG_ERROR(General, "Cant open file for reading (%s).", file.c_str());
            return phy;
        }

        float32 mass = 0, restitution = 0;
        CollisionType collisionType = 0;
        int collDataSize = 0;
        std::vector<float32> collData;

        String line;
        while(std::getline(stream, line)){
            size_t eq_pos = line.find("=");
            if(eq_pos == std::string::npos || eq_pos >= line.length - 1)
                continue;
            std::string identifier = line.substr(0, eq_pos);
            std::string data = line.substr(eq_pos + 1);
            std::transform(identifier.begin(), identifier.end(), identifier.begin(), ::tolower);
            std::transform(data.begin(), data.end(), data.begin(), ::tolower);
            if(identifier.compare("mass") == 0){
                mass = std::stof(data);
            } else if(identifier.compare("restitution") == 0){
                restitution = std::stof(data);
            } else if(identifier.compare("collisiontype") == 0){
                if(data.compare("box") == 0){
                    collisionType = CollisionType::Box;
                } else if(data.compare("sphere") == 0){
                    collisionType = CollisionType::Sphere;
                } else if(data.compare("plane") == 0){
                    collisionType = CollisionType::Plane;
                } else if(data.compare("cylinderx") == 0){
                    collisionType = CollisionType::CylinderX;
                } else if(data.compare("cylindery") == 0){
                    collisionType = CollisionType::CylinderY;
                } else if(data.compare("cylinderz") == 0){
                    collisionType = CollisionType::CylinderZ;
                }
            } else if(identifier.compare("collisiondata") == 0){
                switch(collisionType){
                case CollisionType::Sphere:
                    collDataSize = 1;
                    break;
                case CollisionType::Plane:
                    collDataSize = 4;
                    break;
                case CollisionType::Box:
                case CollisionType::CylinderX:
                case CollisionType::CylinderY:
                case CollisionType::CylinderZ:
                    collDataSize = 3;
                    break;
                }
                if(collDataSize == 0)
                    continue;
                collData = std::vector<float32>(collDataSize);
                std::stringstream dataS(data);
                for(int i = 0; i < collDataSize; ++i){
                    std::string f;
                    std::getline(dataS, f, ",");
                    collData[i] = std::stof(f);
                }
            }
        }
        stream.close();

        uint32 bufferSize = sizeof(PhysicsSpec) + (collDataSize) * sizeof(float32);

        union {
            Byte* phy_buffer;
            PhysicsSpec* physics;
            UIntOfPtrSize location;
        };
        phy_buffer = new Byte[bufferSize];
        std::memset(phy_buffer, 0, bufferSize);

        physics->uuid = generateUUID();
        physics->mass = mass;
        physics->restitution = restitution;
        physics->collisionType = collisionType;
        physics->__CollisionShapeDataLocation = location + sizeof(PhysicsSpec);
        std::memcpy(physics->collisionShapeData, collData.data(), collDataSize * sizeof(float32));

        phy.push_back(physics);
        return phy;
	}

}

ENDNAMESPACE
