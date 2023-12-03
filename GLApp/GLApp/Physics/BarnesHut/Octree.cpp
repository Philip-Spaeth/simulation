#include "Octree.h"
#include <iostream>

Octree::Octree(glm::dvec3 center, double radius, double theta) 
{
	this->center = center;
	this->radius = radius;
	this->theta = theta;
	root = new Node(center, radius, theta, 0);
}

Octree::~Octree() {
}

glm::dvec3 Octree::calculateForces(Particle& particle, double& totalEnergie, int& calulations)
{
	return root->calcForce(particle, totalEnergie, calulations);
}

void Octree::buildTree(std::vector<Particle>& particles) 
{
	for (int i = 0; i < particles.size(); i++) 
	{
		if (particles[i].mass != 0)
		{
			root->insert(particles[i]);
		}
	}
}

void Octree::clearTree() 
{
	root->clear();
}