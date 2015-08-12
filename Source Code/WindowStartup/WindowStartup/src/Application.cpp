#include "Application.h"
#include <random>
#include "stb-master\stb_image.h"


void OnMouseButton(GLFWwindow*, int b, int a, int m)
{
	TwEventMouseButtonGLFW(b, a);
}
void OnMousePosition(GLFWwindow*, double x, double y)
{
	TwEventMousePosGLFW((int)x, (int)y);
}
void OnMouseScroll(GLFWwindow*, double x, double y)
{
	TwEventMouseWheelGLFW((int)y);
}
void OnKey(GLFWwindow*, int k, int s, int a, int m)
{
	TwEventKeyGLFW(k, a);
}
void OnChar(GLFWwindow*, unsigned int c)
{
	TwEventCharGLFW(c, GLFW_PRESS);
}
void OnWindowResize(GLFWwindow*, int w, int h)
{
	TwWindowSize(w, h);
	glViewport(0, 0, w, h);
}



Application::Application()
{
	if(glfwInit() == false)
		return;

	window = glfwCreateWindow(1200,800,"Computer Graphics" , nullptr,nullptr);
	camera = new FlyCamera();
	m_previousTime = 0;
	m_currentTime = 0;
	m_DeltaTime = 0;
	m_cooldown = 2;

	if(window == nullptr)
	{
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);

	if(ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwMakeContextCurrent(window);
		glfwDestroyWindow(window);
		glfwTerminate();
		return;
	}



	//m_skybox = new Skybox();


#pragma region Applying forces to other Objects
	//m_rigidbodies.push_back(new RigidBody(vec3(0, 50, 0), vec3(500.0f, 0, 0), 2, 10));
	//m_rigidbodies[0]->SetID(0);
	//
	////m_rigidbodies.push_back(new RigidBody(vec3(100, 50, 0), vec3(0.0f, 0, 0), 2, 10));
	////m_rigidbodies[1]->SetID(1);
	//
	//m_rigidbodies.push_back(new RigidBody(vec3(100, 50, 0), vec3(0, 0, 0), 2,15.0f,15.0f,15.0f));
	//m_rigidbodies[1]->SetID(1);

#pragma endregion

#pragma region Collision with Plane
	//m_rigidbodies.push_back(new RigidBody(vec3(-30, 100, 0), vec3(0.0f, 0, 0), 2, 10));
	//m_rigidbodies[2]->SetID(2);
	////
	//m_rigidbodies[2]->SetGravity(vec3(0, -5, 0));
	//
	//
	//
	//m_rigidbodies.push_back(new RigidBody(vec3(0, 40, 0), vec3(0,1,0)));
	//m_rigidbodies[3]->SetID(3);
#pragma endregion

#pragma region SPRING JOINT
	//m_rigidbodies.push_back(new RigidBody(vec3(20, 40, 50), vec3(0.0f, 0, 0), 2, 10));
	//m_rigidbodies[4]->SetID(4);
	////
	//
	//m_rigidbodies.push_back(new RigidBody(vec3(20, 40, 90), vec3(1.0f, 0, 0), 2, 10));
	//m_rigidbodies[5]->SetID(5);
	////
	//m_rigidbodies[5]->SetColour(vec4(1, 1, 0, 1));
	//m_rigidbodies[4]->SetColour(vec4(1, 1, 0, 1));
	//
	//m_rigidbodies[5]->SetGravity(vec3(0, -1, 0));
	//m_rigidbodies[4]->SetGravity(vec3(0, -1, 0));
	//
	//m_springjoint.push_back(new SpringJoint(m_rigidbodies[4], m_rigidbodies[5], .05f, 1.0f, 20));
	
	#pragma endregion

#pragma region SPRINGJOINT AABB

	//m_rigidbodies.push_back(new RigidBody(vec3(150, 100, 100), vec3(0, 0, 0), 2, 15.0f, 15.0f, 15.0f));
	//m_rigidbodies[m_rigidbodies.size() - 1]->SetID(m_rigidbodies.size());
	//m_rigidbodies[m_rigidbodies.size() - 1]->SetGravity(vec3(0, -1, 0));
	//m_rigidbodies[m_rigidbodies.size() - 1]->SetColour(vec4(1, 1, 0, 1));
	//
	//
	//m_rigidbodies.push_back(new RigidBody(vec3(50, 100, 100), vec3(0, 0, 0), 2, 15.0f, 15.0f, 15.0f));
	//m_rigidbodies[m_rigidbodies.size() - 1]->SetID(m_rigidbodies.size());
	//m_rigidbodies[m_rigidbodies.size() - 1]->SetGravity(vec3(0, -1, 0));
	//
	//m_springjoint.push_back(new SpringJoint(m_rigidbodies[m_rigidbodies.size() - 1], m_rigidbodies[m_rigidbodies.size() - 2], .05f, 1.0f, 20));
	

#pragma endregion


	//for (unsigned int i = 0; i < m_rigidbodies.size(); i++)
	//{
	//	m_rigidbodies[i]->SetTimeStep(.1f);
	//
	//	if (i == 1)
	//		m_rigidbodies[i]->SetColour(vec4(0, 1, 0, 1));
	//}



	//m_RigidSphere->SetTimeStep(.5f);
	//m_RigidSphere1->SetTimeStep(.5f);

	Gizmos::create();
	
#pragma region Physx

	m_physics = new Physx1();
	m_ragdoll = new RagDoll(m_physics);
	m_particles = new PhysxFluid(m_physics);
	m_player = new KinematicController(5.0f, 5.0f, camera->getPosition(), m_physics);

#pragma endregion
	
	for (unsigned int i = 0; i < 10; i++)
	{
		m_entities.push_back(new Entity(vec3(0, 15, 0), 10));
	}

	//boy do I like to eat some fresh penguin

	glfwSetMouseButtonCallback(window, OnMouseButton);
	glfwSetCursorPosCallback(window, OnMousePosition);
	glfwSetScrollCallback(window, OnMouseScroll);
	glfwSetKeyCallback(window, OnKey);
	glfwSetCharCallback(window, OnChar);
	glfwSetWindowSizeCallback(window, OnWindowResize);
	
	TwInit(TW_OPENGL_CORE, nullptr);
	TwWindowSize(1200, 800);

	m_bar = TwNewBar("Debug Console");
	TwAddSeparator(m_bar, "Camera position:", "");
	
	TwAddVarRW(m_bar, "X position: ", TW_TYPE_FLOAT, &camera->xPosition, "");
	TwAddVarRW(m_bar, "y position: ", TW_TYPE_FLOAT, &camera->yPosition, "");
	TwAddVarRW(m_bar, "z position: ", TW_TYPE_FLOAT, &camera->zPosition, "");
	TwAddVarRW(m_bar, "Camera Speed: ", TW_TYPE_FLOAT, &camera->Speed, "");



}

Application::~Application()
{
	glfwDestroyWindow(window);
	Gizmos::destroy();
	glfwTerminate();
	TwTerminate();


}

void Application::RunGame()
{

	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) 
	{
		
		
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);
		glClearColor(0.25f,0.25f,0.25f,1);

		Update();
		Draw();

		////Update Loop Put all Crap here
		//currentTime = (float)glfwGetTime();
		//DeltaTime = currentTime - previousTime;
		//previousTime = currentTime;
	
		TwDraw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}	
}

void Application::Update()
{


		

	m_currentTime = (float)glfwGetTime();
	m_DeltaTime = m_currentTime - m_previousTime;
	m_previousTime = m_currentTime;
	m_cooldown -= m_DeltaTime;


	for (unsigned int i = 0; i < m_entities.size(); i++)
	{
		m_entities[i]->Update(m_DeltaTime);
	}


	

	if (m_cooldown < 0)
	{
		m_cooldown = .90f;
		m_physics->MakeBlocks();	
	}

	camera->update(m_DeltaTime);
	camera->getWorldTransform();

#pragma region MyPhysics

	//for (unsigned int i = 0; i < m_rigidbodies.size(); i++ )
	//{
	//	m_rigidbodies[i]->Update(m_DeltaTime);
	//
	//for (unsigned int j = 0; j < m_rigidbodies.size(); j++)
	//	{
	//		if (i == j)
	//			continue;
	//	
	//		else
	//			m_rigidbodies[i]->CheckCollision(m_rigidbodies[j]);
	//	}
	//
	//
	//}
	//
	//for (unsigned i = 0; i < m_springjoint.size(); i++)
	//{
	//	m_springjoint[i]->Update(m_DeltaTime);
	//}
#pragma endregion

//#pragma region PHYSXUPDATES
//	//m_RigidSphere->Update(m_DeltaTime);
//	m_physics->Update(m_DeltaTime);
//	m_player->Update(camera);
//
//#pragma endregion

	
}

void Application::Draw()
{




	Gizmos::clear();
	m_particles->Update(m_DeltaTime);

	//Gizmos::addAABBFilled(vec3(0,2,0), vec3(250, 1, 250), vec4(0, 0, 1, 1), nullptr);
	//Gizmos::addAABB(vec3(0, 0, 250), vec3(250, 250, 10), vec4(0, 0, 1, 1), nullptr);
	//Gizmos::addAABB(vec3(0, 0, -250), vec3(250, 250, 10), vec4(0, 0, 1, 1), nullptr);
	//Gizmos::addAABB(vec3(250, 0, 0), vec3(10, 250, 250), vec4(0, 1, 1, 1), nullptr);
	//Gizmos::addAABB(vec3(-250, 0, 0), vec3(10, 250, 250), vec4(0, 0, 1, 1), nullptr);


	//Gizmos::addAABB(vec3(0, 2, 0), vec3(250, 1, 250), vec4(0, 0, 1, 1), nullptr);
	//m_RigidSphere->Draw();

#pragma region Myphysics

	//for (unsigned int i = 0; i < m_rigidbodies.size(); i++)
	//{
	//	m_rigidbodies[i]->Draw();
	//}
	//
	//for (unsigned i = 0; i < m_springjoint.size(); i++)
	//{
	//	m_springjoint[i]->Draw();
	//}

#pragma endregion
	
	m_ragdoll->Draw();
	m_physics->Draw();
	//m_skybox->Draw(camera);
	//for (unsigned int i = 0; i < m_entities.size(); i++)
	//{
	//	m_entities[i]->Draw();
	//}

	Gizmos::draw(camera->getProjectionTransform());
}

