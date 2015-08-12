#ifndef PHYSICS_H
#define PHYSICS_H

#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include <pvd\PxVisualDebugger.h>
#include <Gizmos.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <vector>
#include <iostream>
//#include "TriggerCollider.h"

using namespace physx;
using namespace glm;


#define MAX_BLOCKS = 10;

struct FilterGroup
{
	enum Enum
	{
		ePLAYER = (1 << 0),
		ePLATFORM = (1 << 1),
		eGROUND = (1 << 2)
	};
};


class TriggerCollider : public PxSimulationEventCallback
{
public:
	//TriggerCollider();
	//~TriggerCollider();

	int GetMaxBlocks() { return m_blockCount; }
	void IncrimentMaxBlock(int val) { m_blockCount += val; }


	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs);
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 nbPairs);
	virtual void onConstraintBreak(PxConstraintInfo* info, PxU32 nbPairs){};
	virtual void onWake(PxActor** actor, PxU32){};
	virtual void onSleep(PxActor** actor, PxU32){};

	
private:

	int m_blockCount = 10;

};


class myAllocator : public PxAllocatorCallback
{
public:

	virtual ~myAllocator() {}

	virtual void* allocate(size_t size, const char* typeName, const char* filename, int line);
	virtual void deallocate(void* ptr);

	
private:

};

class Physx1
{
public:
	Physx1();
	~Physx1();



	void setUpPhysX();
	void Update(float deltaTime);
	void setUpVisualDebugger();
	void setUpTutorial();
	void MakeBlocks();
	void Draw(); //Lol use gizmos for this
	void setShapeAsTrigger(PxRigidActor* actorIn);

	void setupFiltering(PxRigidActor* actor, PxU32 filterGroup, PxU32 filterMask);
	//PxFilterFlags myFliterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	//	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize);
	PxScene* m_PhysicsScene;
	PxMaterial* m_PhysicsMaterial;
	PxPhysics* m_Physics;
	std::vector<PxRigidDynamic*>m_boxActors;

	PxRigidActor* m_rigidactor;

private:

	


	//Creating a box;
	//-------------------------


	PxBoxGeometry* m_box;
	PxRigidDynamic* m_dynamicActor;
	PxTransform* m_transform;

	float density;
	int m_boxCount;
	int m_maxBlocks;
	//-------------------------


	PxFoundation* m_PhysicsFoundation;
	//PxPhysics* m_Physics;
	TriggerCollider* collisioncallback;


	PxDefaultErrorCallback m_defaultErrorCallback;
	PxDefaultAllocator m_DefaultAllocatorCallback;
	PxSimulationFilterShader m_defaultFilterShader = PxDefaultSimulationFilterShader;
	
	PxMaterial* m_boxMaterial;
	PxCooking* m_PhysicsCooker;

	PxAllocatorCallback* m_myCallback;

};







#endif


