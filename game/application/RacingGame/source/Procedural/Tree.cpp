#include "Procedural/Tree.h"
/*
#include <OgreMath.h>
#include <OgreMatrix3.h>
#include <OgreMatrix4.h>
#include <OgrePlane.h>
#include <random>
#include <Logging/Logging.h>

BEGINNAMESPACE

float32 expTrunkRadius(float32 minRadius, float32 maxRadius, int32 iteration, int32 maxIterations) {
	float32 u = maxRadius - minRadius;
	float32 x = float32(iteration) / float32(maxIterations);
	return u * Ogre::Math::Exp(-4.0 * x) + minRadius;
}

ProcTree::ProcTree()
{
	m_Properties.trunk.radiusComputer = &expTrunkRadius;
}

void ProcTree::setProperties(const Properties & properties)
{
	m_Properties = properties;
}

bool ProcTree::generate()
{
	std::default_random_engine generator;
	std::uniform_real_distribution<float32> distribution(0.0,1.0);
	generator.seed(m_Properties.seed); //Seed the generator

	RandomGenerator genFn = std::bind(distribution, generator);
	HeadTail trunk;
	Vertices v_trunk;
	Indices i_trunk;
	generateTrunk(v_trunk, i_trunk, m_Properties.trunk, genFn, trunk);	
	trunk.head = (trunk.tail - trunk.head) * m_Properties.trunk.startBranches + trunk.head;
	generateBranch(v_trunk, i_trunk, m_Properties.branch, 1, genFn, trunk);


	//Simple test debug output
	FileStream stream;
	stream.open("simple.obj", std::ios::out);

	for (auto vert : v_trunk) {
		stream << "v " << vert.x << " " << vert.y << " " << vert.z << std::endl;
	}
	stream << std::endl;
	int32 i = 0;
	for (auto faceIdx : i_trunk) {
		if (i++ % 3 == 0) stream << std::endl << "f";
		stream << " " << (faceIdx + 1);
	}
	stream.close();
	//simple test debug output

	return true;
}

void ProcTree::generateTrunk(Vertices & vertices, Indices & indices, const TrunkProperties& properties, RandomGenerator& randomGenerator, HeadTail& out_trunkHeadTail)
{
	float32 angleOff = Constants::c_2Pi / float32(properties.numSegmentsPerRing + 1);
	float32 heightOff = properties.length / properties.numSegments;

	for (int32 i = 0; i < properties.numSegments; ++i) {
		// generate offset
		Ogre::Vector3 randomOffset(randomGenerator(), 0.0f, randomGenerator());
		randomOffset *= properties.randomSegmentPositionVariation; //max X meter random offset

		float32 radiusVariation = 1.0 - randomGenerator() * properties.randomSegmentRadiusVeriation;
		float32 currentRadius = (*properties.radiusComputer)(properties.topRadius, properties.bottomRadius, i, properties.numSegments) * radiusVariation; //generate the radius for this segment
		
		Ogre::Vector3 start(-currentRadius, 0.0, 0.0f);
		Ogre::Vector3 height(0.0f, i * heightOff, 0.0f);
		for (int32 j = 0; j < properties.numSegmentsPerRing; ++j) {
			Ogre::Matrix3 m;
			m.FromAngleAxis(Ogre::Vector3::UNIT_Y, Ogre::Radian(j * angleOff));
			Ogre::Vector3 pos = m * start + height + randomOffset;
			vertices.push_back(pos);
		}
	}

	out_trunkHeadTail.head = Ogre::Vector3::ZERO;
	for (int32 j = 0; j < properties.numSegmentsPerRing; ++j) {
		out_trunkHeadTail.head += vertices[j];
	}
	out_trunkHeadTail.head /= properties.numSegmentsPerRing;

	out_trunkHeadTail.tail = Ogre::Vector3::ZERO;
	for (int32 j = 0; j < properties.numSegmentsPerRing; ++j) {
		out_trunkHeadTail.tail += vertices[(properties.numSegments - 1) * properties.numSegmentsPerRing + j];
	}
	out_trunkHeadTail.tail /= properties.numSegmentsPerRing;

	/*
	//bend the whole tree 
	float32 bendAngle = properties.bendAngle / properties.numSegments;
	Ogre::Vector3 bendAxis = Ogre::Vector3::UNIT_X;
	for (int32 i = 1; i < properties.numSegments - 1; ++i) {
		//compute pivot point
		Ogre::Vector3 pivot = Ogre::Vector3::ZERO;
		for (int32 j = 0; j < properties.numSegmentsPerRing; ++j) {
			pivot += vertices[i * properties.numSegmentsPerRing + j];
		}
		pivot /= properties.numSegmentsPerRing;

		Ogre::Matrix3 m;
		m.FromAngleAxis(bendAxis, Ogre::Radian(bendAngle * randomGenerator()));
		for (int32 j = i * properties.numSegmentsPerRing; j < vertices.size(); ++j) {
			vertices[j] = (m * (vertices[j] - pivot)) + pivot;
		}
	}
	//* /

	//generate indices for this trunk
	//Do a triangulation 
	//
	//    x--x--x
	// ...| /| /|...
	//    |/ |/ |
	//    x--x--x
	//
	for (int32 i = 0; i < properties.numSegments - 1; ++i) {
		int32 ringStartIdx = i * properties.numSegmentsPerRing;
		int32 nextRingStartIdx = (i + 1) * properties.numSegmentsPerRing;
		for (int32 j = 0; j < properties.numSegmentsPerRing - 1; ++j) {
			int32 c0_idx_0 = ringStartIdx + j;
			int32 c0_idx_1 = ringStartIdx + j + 1;
			int32 c1_idx_0 = nextRingStartIdx + j;
			int32 c1_idx_1 = nextRingStartIdx + j + 1;

			//triangle 1
			indices.push_back(c0_idx_0);
			indices.push_back(c1_idx_1);
			indices.push_back(c1_idx_0);
			//triangle 2
			indices.push_back(c0_idx_0);
			indices.push_back(c0_idx_1);
			indices.push_back(c1_idx_1);
		}
		//connect both ring ends
		int32 c0_idx_0 = ringStartIdx + properties.numSegmentsPerRing - 1;
		int32 c0_idx_1 = ringStartIdx;
		int32 c1_idx_0 = nextRingStartIdx + properties.numSegmentsPerRing - 1;
		int32 c1_idx_1 = nextRingStartIdx;

		//triangle 1
		indices.push_back(c0_idx_0);
		indices.push_back(c1_idx_1);
		indices.push_back(c1_idx_0);
		//triangle 2
		indices.push_back(c0_idx_0);
		indices.push_back(c0_idx_1);
		indices.push_back(c1_idx_1);
	}
	//find an elegant way to close the top here

	return;
}

Ogre::Vector3 perpendicular(const Ogre::Vector3& dir) {
	Ogre::Vector3 crossDir = Ogre::Vector3::UNIT_X;
	if (Ogre::Math::RealEqual(dir.dotProduct(crossDir), 0.0f)) {
		crossDir = Ogre::Vector3::UNIT_Y;
	}
	return dir.crossProduct(crossDir);
}

void ProcTree::generateBranch(Vertices & vertices, Indices & indices, const BranchProperties & properties, int32 level, RandomGenerator & randomGenerator, const HeadTail& parent)
{
	if (level >= properties.branchingLevel) return; //Reached the max level -> return

	int32 branchCount = int32(float32(properties.maxBranchesPerLevel) * randomGenerator()) + properties.minBranchesPerLevel;
		
	for (int32 branchLevel = 0; branchLevel < branchCount; ++branchLevel) {

		// probability to skip this branch
		// the higher the branching level, the higher the propability to skip a branch
		if (randomGenerator() < float32(level) / float32(properties.branchingLevel + 1)) continue;

		float32 angleOff = Constants::c_2Pi / float32(properties.numSegmentsPerBranchRing + 1); ;
		Ogre::Vector3 branch_pos = (parent.tail - parent.head) * randomGenerator() + parent.head; //a position on the parent tail
		Ogre::Vector3 normal_direction = perpendicular(parent.tail - parent.head).normalisedCopy();
		
		Ogre::Vector3 dir = normal_direction; //@TODO some computation to randomize the direction
		Ogre::Matrix4 trans;
		Ogre::Quaternion quat;
		{ //Generate quaternion
			Ogre::Vector3 o = Ogre::Vector3::UNIT_Y.crossProduct(dir);
			quat.x = o.x;
			quat.y = o.y;
			quat.z = o.z;
			quat.w = Ogre::Vector3::UNIT_Y.dotProduct(dir);
			quat.normalise();
		}
		trans.makeTransform(branch_pos, Ogre::Vector3::UNIT_SCALE, quat);

		float32 factor = float32(level) / float32(properties.branchingLevel + 1);
		//float32 branchLength = factor * ((properties.maxBranchLength - properties.minBranchLength) * randomGenerator() + properties.minBranchLength); // computer a random branch length
		
		float32 maxBranchRadius = (properties.maxRadius) * factor;
		float32 minBranchRadius = Ogre::Math::Clamp( maxBranchRadius * randomGenerator(), properties.minRadius, properties.maxRadius);

		float32 branchLength = Ogre::Math::Clamp(properties.maxBranchLength * factor * randomGenerator(), properties.minBranchLength, properties.maxBranchLength);
		float32 heightOff = branchLength / properties.numSegmentsPerBranch;

		HeadTail next = { branch_pos,branch_pos + dir * branchLength };

		for (int32 i = 0; i < properties.numSegmentsPerBranch; ++i) {
			float32 currentRadius = (1.0f - float32(i) / float32(properties.numSegmentsPerBranch)) * (maxBranchRadius - minBranchRadius) + minBranchRadius;
			Ogre::Vector3 start(-currentRadius, 0.0, 0.0f);
			Ogre::Vector3 height(0.0f, i * heightOff, 0.0f);
			for (int32 j = 0; j < properties.numSegmentsPerBranchRing; ++j) {
				Ogre::Matrix3 m;
				m.FromAngleAxis(Ogre::Vector3::UNIT_Y, Ogre::Radian(j * angleOff));
				Ogre::Vector3 pos = m * start + height;
				pos = trans.transformAffine(pos);
				vertices.push_back(pos);
			}
		}

		int32 indexOff = *std::max_element(indices.begin(), indices.end()) + 1;
		//generate indices for this branch
		//Do a triangulation 
		//
		//    x--x--x
		// ...| /| /|...
		//    |/ |/ |
		//    x--x--x
		//
		for (int32 i = 0; i < properties.numSegmentsPerBranch - 1; ++i) {
			int32 ringStartIdx = i * properties.numSegmentsPerBranchRing;
			int32 nextRingStartIdx = (i + 1) * properties.numSegmentsPerBranchRing;
			for (int32 j = 0; j < properties.numSegmentsPerBranchRing - 1; ++j) {
				int32 c0_idx_0 = indexOff + ringStartIdx + j;
				int32 c0_idx_1 = indexOff + ringStartIdx + j + 1;
				int32 c1_idx_0 = indexOff + nextRingStartIdx + j;
				int32 c1_idx_1 = indexOff + nextRingStartIdx + j + 1;

				//triangle 1
				indices.push_back(c0_idx_0);
				indices.push_back(c1_idx_1);
				indices.push_back(c1_idx_0);
				//triangle 2
				indices.push_back(c0_idx_0);
				indices.push_back(c0_idx_1);
				indices.push_back(c1_idx_1);
			}
			//connect both ring ends
			int32 c0_idx_0 = indexOff + ringStartIdx + properties.numSegmentsPerBranchRing - 1;
			int32 c0_idx_1 = indexOff + ringStartIdx;
			int32 c1_idx_0 = indexOff + nextRingStartIdx + properties.numSegmentsPerBranchRing - 1;
			int32 c1_idx_1 = indexOff + nextRingStartIdx;

			//triangle 1
			indices.push_back(c0_idx_0);
			indices.push_back(c1_idx_1);
			indices.push_back(c1_idx_0);
			//triangle 2
			indices.push_back(c0_idx_0);
			indices.push_back(c0_idx_1);
			indices.push_back(c1_idx_1);
		}
		//generateBranch(vertices, indices, properties, level + 1, randomGenerator, next);
	}
}

size_type ProcTree::VertexBufferSize() const
{

	return size_type();
}

size_type ProcTree::IndexBufferSize() const
{
	return size_type();
}

void ProcTree::generateSegment(Vertices & out_vertices, Indices & out_indices, float32 radius, int32 numFaces)
{

}

ENDNAMESPACE
*/
