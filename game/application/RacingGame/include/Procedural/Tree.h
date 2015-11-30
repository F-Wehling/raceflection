#pragma once
/*
#include "OgreVector3.h"
#include <Math/Math.h>
#include <vector>

BEGINNAMESPACE

class ProcTree {

	typedef float32(*pComputeTrunkRadius)(float32 minRadius, float32 maxRadius, int32 segment, int32 maxSegments);

	typedef struct {
		Ogre::Vector3 head;
		Ogre::Vector3 tail;
	} HeadTail;

	//
	/// one branch
	class Branch {
		Ogre::Vector3 m_Head;
		Ogre::Vector3 m_Tail;

		float32 length;
		float32 radius;

		Branch* parent;
		std::vector<Branch*> childs;
	};

public:
	struct RootProperties {
		float32 rootDensity = 1.0;
		float32 averageRootThickness = 0.5;
	};

	struct TrunkProperties {
		float32 length = 20.0f;
		float32 bottomRadius = 1.5f;
		float32 topRadius = 0.3f;
		float32 bendAngle = Constants::c_PiDiv4;
		int32 numSegments = 20;
		int32 numSegmentsPerRing = 10;
		float32 randomSegmentPositionVariation = 0.2f; //per segment variation in meter
		float32 randomSegmentRadiusVeriation = 0.1f; //per segment radius variation
		pComputeTrunkRadius radiusComputer;
		float32 startBranches = 0.5; //[0-1.0] where to start branching (at first)
	};

	struct BranchProperties {
		int32 branchingLevel = 4;
		int32 minBranchesPerLevel = 3;
		int32 maxBranchesPerLevel = 10;
		int32 numSegmentsPerBranch = 3;
		int32 numSegmentsPerBranchRing = 3;
		float32 minRadius = 0.1f;
		float32 maxRadius = 0.6f;
		float32 maxBranchLength = 4.0f;
		float32 minBranchLength = 0.3f;
	};

	struct Properties {
		RootProperties roots;
		TrunkProperties trunk;
		BranchProperties branch;
		int32 seed = 'R' + 'A' + 'C' + 'N' + 'G' + '-' + 'G' + 'A' + 'M' + 'E';
	};

public:
	ProcTree();

	 void setProperties(const Properties& properties);
	 bool generate();

private:
	typedef std::vector<Ogre::Vector3> Vertices;
	typedef std::vector<int32> Indices;
	typedef std::function<float32()> RandomGenerator;

	size_type VertexBufferSize() const;
	size_type IndexBufferSize() const;

	static void generateSegment(Vertices& out_vertices, Indices& out_indices, float32 radius, int32 numFaces = 5);
	static void generateTrunk(Vertices& vertices, Indices& indices, const TrunkProperties& properties, RandomGenerator& randomGenerator, HeadTail& out_trunkHeadTail);
	static void generateBranch(Vertices& vertices, Indices& indices, const BranchProperties& properties, int32 level, RandomGenerator& randomGenerator, const HeadTail& parent);

private:
	typedef std::vector<Branch> Branches;
	Branches m_Branches; //generated branches based on the parameter and randomness
	Properties m_Properties; //The properties for this tree
};
ENDNAMESPACE

*/