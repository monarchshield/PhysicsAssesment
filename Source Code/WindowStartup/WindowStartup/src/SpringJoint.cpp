#include "SpringJoint.h"

SpringJoint::SpringJoint(RigidBody* a_object1, RigidBody* a_object2, float springCoeffecient, float damping, float restlength)
:m_object1(a_object1), m_object2(a_object2), m_springCostant(springCoeffecient), m_damping(damping), m_restlength(restlength)
{
	

}

SpringJoint::~SpringJoint()
{

}

void SpringJoint::Update(float deltatime)
{
	/*
	float length = glm::length(m_object1->GetPosition() - m_object2->GetPosition());
	float displacement = length - m_restlength;

	m_force = (-m_springCostant * displacement) - (m_damping * m_object1->GetVelocity());
	m_object1->applyForce(m_force);

	m_force = (-m_springCostant * displacement) - (m_damping * m_object2->GetVelocity());
	m_object2->applyForce(-m_force);
	*/

	vec3 direction = m_object1->GetPosition() - m_object2->GetPosition();
	direction -= normalize(direction) * m_restlength;
	direction *= -m_springCostant;
	m_object1->applyForce(direction);
	m_object2->applyForce(-direction);
}

void SpringJoint::Draw()
{
	Gizmos::addLine(m_object1->GetPosition(), m_object2->GetPosition(), vec4(1, 0, 0, 1));
}

