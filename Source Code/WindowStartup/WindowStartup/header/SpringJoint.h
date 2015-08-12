#ifndef SPRINGJOINT_H
#define SPRINGJOINT_H

#include "RigidBody.h"


class SpringJoint
{
public:
	SpringJoint(RigidBody* a_object1, RigidBody* a_object2, float springCoeffecient, float damping, float restlength);
	~SpringJoint();

	void Update(float deltatime);
	void Draw();


private:
	float m_damping;
	float m_restlength;
	float m_springCostant;

	vec3 m_force;

	RigidBody* m_object1;
	RigidBody* m_object2;

};
#endif