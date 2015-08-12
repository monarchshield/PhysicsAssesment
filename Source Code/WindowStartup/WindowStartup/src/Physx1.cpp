#include "Physx1.h"

void* myAllocator::allocate(size_t size, const char* typeName, const char* filename, int line)
{
	void* pointer = _aligned_malloc(size, 16);
	return pointer;
}

void myAllocator::deallocate(void* ptr)
{
	_aligned_free(ptr);
}

Physx1::Physx1()
{
	m_maxBlocks = 10;
	m_boxCount = 0;
	setUpPhysX();
	setUpTutorial();
	setUpVisualDebugger();

	
	

}

Physx1::~Physx1()
{
	m_PhysicsScene->release();
	m_Physics->release();
	m_PhysicsFoundation->release();
}


PxFilterFlags myFliterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	// let triggers through
	if (PxFilterObjectIsTrigger(attributes0) ||
		PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}
	// generate contacts for all that were not filtered above
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;
	// trigger the contact callback for pairs (A,B) where
	// the filtermask of A contains the ID of B and vice versa.
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 &
		filterData0.word1))
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND |
		PxPairFlag::eNOTIFY_TOUCH_LOST;
	return PxFilterFlag::eDEFAULT;
}

void Physx1::Update(float deltaTime)
{
	
	
	m_maxBlocks = collisioncallback->GetMaxBlocks();
	if (deltaTime <= 0)
		return;

	m_PhysicsScene->simulate(deltaTime);

	

	while (m_PhysicsScene->fetchResults() == false)
	{
		

		//Dont need to do anything here yet but we have to fetch results
	}

}

void Physx1::MakeBlocks()
{

	if (m_boxActors.size() < m_maxBlocks)
	{

		PxTransform m_transform(PxVec3(10, 40, 0));
		PxBoxGeometry box(2, 2, 2);
		float density = 50;


		m_dynamicActor = PxCreateDynamic(*m_Physics, m_transform, box, *m_PhysicsMaterial, density);
		m_boxActors.push_back(m_dynamicActor);

		int size = m_boxActors.size();

		m_PhysicsScene->addActor(*m_boxActors[size - 1]);

		m_boxCount++;

		std::cout << "Boxes in Scene: " << m_boxCount << "\n";
	}
}

void Physx1::setUpVisualDebugger()
{
	//Check if  pvdconnection manager is available on this platform
	if (m_Physics->getPvdConnectionManager() == NULL)
		return;

	//Setup connection parameters
	const char* pvd_host_ip = "127.0.0.1";

	//ip of the pc which is running pvd 
	int port = 5425;

	//TCP port to connect to, where pvd is listening
	unsigned int timeout = 100;

	//Timeout in milliseconds to wait for pvd to respond
	//consoels and remote pcs need  a higher timeout
	PxVisualDebuggerConnectionFlags connectionFlags = PxVisualDebuggerExt::getAllConnectionFlags();

	//and now try to connect pxvisauldebuggerext
	auto theConnection = PxVisualDebuggerExt::createConnection(m_Physics->getPvdConnectionManager(), pvd_host_ip, port, timeout, connectionFlags);


}

void Physx1::setUpPhysX()
{
	m_myCallback = new myAllocator();
	m_PhysicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *m_myCallback, m_defaultErrorCallback);
	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_PhysicsFoundation, PxTolerancesScale());
	PxInitExtensions(*m_Physics);
	
	//Create physics material
	m_PhysicsMaterial = m_Physics->createMaterial(0.5f, 0.5f, .5f);
	PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());


	sceneDesc.gravity = PxVec3(0, -10.0f, 0);
	sceneDesc.filterShader = &physx::PxDefaultSimulationFilterShader;
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	m_PhysicsScene = m_Physics->createScene(sceneDesc);


	collisioncallback = new TriggerCollider();
	m_PhysicsScene->setSimulationEventCallback(collisioncallback);
	sceneDesc.filterShader = myFliterShader;

	

	
	
}

void Physx1::Draw()
{
	
	for (int i = 0; i < m_boxActors.size(); i++)
	{
		mat4* identity;
		


		PxTransform position = m_boxActors[i]->getGlobalPose();
		glm::vec3 pos = vec3(position.p.x, position.p.y, position.p.z);
		

		vec3 rot1 = vec3(position.q.getBasisVector0().x, position.q.getBasisVector0().y, position.q.getBasisVector0().z);
		vec3 rot2 = vec3(position.q.getBasisVector1().x, position.q.getBasisVector1().y, position.q.getBasisVector1().z);
		vec3 rot3 = vec3(position.q.getBasisVector2().x, position.q.getBasisVector2().y, position.q.getBasisVector2().z);

		identity = new mat4(rot1.x, rot1.y, rot1.z, 1,
							rot2.x, rot2.y, rot2.z, 1,
							rot3.x, rot3.y, rot3.z, 1,
							pos.x, pos.y, pos.z, 1);




		//std::cout << "Cube Position: " << pos.x << "," << pos.y << "," << pos.z << "\n";

	
		//Just render out stuff with gizmos here
		Gizmos::addAABBFilled(pos, vec3(2, 2, 2), vec4(1, 0, 0, 1), identity);
		//Position  //Width (X), Height(Y), Length(Z)  //Colour(vec4) //mat4 Identity Matrix
	}

	Gizmos::addAABBFilled(vec3(30.0f, 10, 0), vec3(2, 2, 2), vec4(0, 1, 0, 1), nullptr);
	
}

void Physx1::setUpTutorial()
{
	//add a plane
	PxTransform pose = PxTransform(PxVec3(0, 5, 0), PxQuat(PxHalfPi * 1.0f, PxVec3(0, 0, 1)));
	PxRigidStatic* plane = PxCreateStatic(*m_Physics, pose, PxPlaneGeometry(), *m_PhysicsMaterial);

	//add it to the physx scene
	m_PhysicsScene->addActor(*plane);

	PxBoxGeometry side1(2, 2, 2);
	PxBoxGeometry side2(2, 2, 2);

	PxRigidStatic* box = PxCreateStatic(*m_Physics, pose, side1, *m_PhysicsMaterial);

	pose = PxTransform(PxVec3(30.0f, 10, 0));
	box = PxCreateStatic(*m_Physics, pose, side2, *m_PhysicsMaterial);
	
	setShapeAsTrigger(box);
	setupFiltering(box, FilterGroup::ePLAYER, FilterGroup::ePLATFORM);
	m_PhysicsScene->addActor(*box);
	
	//m_boxActors.push_back(box);
	//m_PhysXActors.push_back(box);
	//m_phy

}


void Physx1::setShapeAsTrigger(PxRigidActor* actorIn)
{
	PxRigidStatic* staticActor = actorIn->is<PxRigidStatic>();
	assert(staticActor);
	const PxU32 numShapes = staticActor->getNbShapes();
	PxShape** shapes = (PxShape**)_aligned_malloc(sizeof(PxShape*)*numShapes, 16);
	staticActor->getShapes(shapes, numShapes);
	
	for (PxU32 i = 0; i < numShapes; i++)
	{
		shapes[i]->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		shapes[i]->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}
}

void TriggerCollider::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	for (PxU32 i = 0; i < nbPairs; i++)
	{
		const PxContactPair& cp = pairs[i];
		//only interested in touches found and lost
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			std::cout << "Collision Detected between: ";
			std::cout << pairHeader.actors[0]->getName();
			std::cout << pairHeader.actors[1]->getName() << std::endl;
		}
	}
}

void TriggerCollider::onTrigger(PxTriggerPair* pairs, PxU32 nbPairs)
{
	for (PxU32 i = 0; i < nbPairs; i++)
	{
		PxTriggerPair* pair = pairs + i;
		PxActor* triggerActor = pair->triggerActor;
		PxActor* otherActor = pair->otherActor;
		//std::cout << otherActor->getName();
		std::cout << " Entered Trigger ";
		std::cout << "Max Blocks: " << m_blockCount;
		IncrimentMaxBlock(5);

		//std::cout << triggerActor->getName()<< std::endl;
	}
}
void Physx1::setupFiltering(PxRigidActor* actor, PxU32 filterGroup, PxU32 filterMask){	PxFilterData filterData;
	filterData.word0 = filterGroup; // word0 = own ID
	filterData.word1 = filterMask; // word1 = ID mask to filter pairs that trigger a contact callback;
	
	const PxU32 numShapes = actor->getNbShapes();
	PxShape** shapes = (PxShape**)_aligned_malloc(sizeof(PxShape*)*numShapes, 16);
	
	actor->getShapes(shapes, numShapes);
	
	for (PxU32 i = 0; i < numShapes; i++)
	{
		PxShape* shape = shapes[i];
		shape->setSimulationFilterData(filterData);
	}
	_aligned_free(shapes);}