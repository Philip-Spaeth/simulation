#include "Node.h"
#include <iostream> 

Node::Node(glm::dvec3 center, double radius, double theta, int index)
{
	this->center = center;
	this->radius = radius;
	this->theta = theta;
	this->index = index;
}

Node::~Node()
{
}

glm::dvec3 Node::calcForce(Particle& p, double& totalEnergie, int& calulations)
{
	double G = 6.67408e-11;

	glm::dvec3 force = {0,0,0};

	if (isLeaf) 
	{
		if (particle.position != p.position)
		{
			glm::dvec3 delta = particle.position - p.position;
			double distance = glm::length(delta);
			double forceMagnitude = (G * particle.mass * p.mass) / (distance * distance);
			glm::dvec3 Force = forceMagnitude * glm::normalize(delta);
			force += Force;
			totalEnergie += p.calcPotentialEnergie(particle, G, 0);
			calulations++;
		}
	}
	else 
	{
		glm::dvec3 delta = center - p.position;
		double r = glm::length(delta);

		// Calculate height of the node
		double d = radius * 2; 

		if (d / r < theta && r > radius && mass != p.mass)
		{
			calcMass();
			glm::dvec3 delta = massCenter - p.position;
			double distance = glm::length(delta);
			double forceMagnitude = (G * mass * p.mass) / (distance * distance);
			glm::dvec3 Force = forceMagnitude * glm::normalize(delta);
			force += Force;
			Particle p2 = Particle(massCenter, mass);
			totalEnergie += p.calcPotentialEnergie(p2, G, 0);
			calulations++;
		}
		else
		{
			// For all child nodes n
			for (Node* child : child) 
			{
				if(child != nullptr)
				{
					force += child->calcForce(p, totalEnergie, calulations);
				}
			}
		}
	}
	return force;
}

void Node::insert(Particle& p)
{
	if (mass == 0)
	{
		mass = p.mass;
		particle = p;
		isLeaf = true;
	}
	else
	{
		isLeaf = false;
		//check in wich quadrant the particle is
		int quadrant = 0;
		if (p.position.x > center.x)
		{
			quadrant += 1;
		}
		if (p.position.y > center.y)
		{
			quadrant += 2;
		}
		if (p.position.z > center.z)
		{
			quadrant += 4;
		}

		// Print debugging information
		//std::cout << "Inserting particle in node " << index << " mass: " << mass << " radius: " << radius << std::endl;


		if (child[quadrant] == nullptr) 
		{
			//create new node
			glm::dvec3 newCenter = center;
			double newRadius = radius / 2;
			switch (quadrant)
			{
			case 0:
				newCenter.x -= newRadius;
				newCenter.y -= newRadius;
				newCenter.z -= newRadius;
				break;
			case 1:
				newCenter.x += newRadius;
				newCenter.y -= newRadius;
				newCenter.z -= newRadius;
				break;
			case 2:
				newCenter.x -= newRadius;
				newCenter.y += newRadius;
				newCenter.z -= newRadius;
				break;
			case 3:
				newCenter.x += newRadius;
				newCenter.y += newRadius;
				newCenter.z -= newRadius;
				break;
			case 4:
				newCenter.x -= newRadius;
				newCenter.y -= newRadius;
				newCenter.z += newRadius;
				break;
			case 5:
				newCenter.x += newRadius;
				newCenter.y -= newRadius;
				newCenter.z += newRadius;
				break;
			case 6:
				newCenter.x -= newRadius;
				newCenter.y += newRadius;
				newCenter.z += newRadius;
				break;
			case 7:
				newCenter.x += newRadius;
				newCenter.y += newRadius;
				newCenter.z += newRadius;
				break;
			}
			child[quadrant] = new Node(newCenter, newRadius, theta, index+1);
		}

		child[quadrant]->insert(p);

		if(particle.position != p.position)
		{
			//check in wich quadrant the particle is
			quadrant = 0;
			if (particle.position.x > center.x)
			{
				quadrant += 1;
			}
			if (particle.position.y > center.y)
			{
				quadrant += 2;
			}
			if (particle.position.z > center.z)
			{
				quadrant += 4;
			}

			if (child[quadrant] == nullptr)
			{
				//create new node
				glm::dvec3 newCenter = center;
				double newRadius = radius / 2;
				switch (quadrant)
				{

					// Adjust newCenter based on the quadrant
				case 0:
					newCenter.x -= newRadius;
					newCenter.y -= newRadius;
					newCenter.z -= newRadius;
					break;
				case 1:
					newCenter.x += newRadius;
					newCenter.y -= newRadius;
					newCenter.z -= newRadius;
					break;
				case 2:
					newCenter.x -= newRadius;
					newCenter.y += newRadius;
					newCenter.z -= newRadius;
					break;
				case 3:
					newCenter.x += newRadius;
					newCenter.y += newRadius;
					newCenter.z -= newRadius;
					break;
				case 4:
					newCenter.x -= newRadius;
					newCenter.y -= newRadius;
					newCenter.z += newRadius;
					break;
				case 5:
					newCenter.x += newRadius;
					newCenter.y -= newRadius;
					newCenter.z += newRadius;
					break;
				case 6:
					newCenter.x -= newRadius;
					newCenter.y += newRadius;
					newCenter.z += newRadius;
					break;
				case 7:
					newCenter.x += newRadius;
					newCenter.y += newRadius;
					newCenter.z += newRadius;
					break;
				}
				// Create the new child node
				child[quadrant] = new Node(newCenter, newRadius, theta, index + 1);
			}
			// Recursively insert the particle into the appropriate child node
			child[quadrant]->insert(p);

			// Update mass (mass is the sum of the masses of all particles in the node)
			mass += p.mass;
		}
	}
	//std::cout << "inserted particle " << "in node " << index <<"  mass: " << mass << "radius: "<< radius << std::endl;
}

void Node::calcMass() 
{
	if (isLeaf)
	{
		massCenter = particle.position;
	}
	else
	{
		glm::dvec3 massCenterSum = { 0,0,0 };
		double massSum = 0;
		for (Node* child : child)
		{
			if (child != nullptr)
			{
				child->calcMass();
				massCenterSum += child->massCenter * child->mass;
				massSum += child->mass;
			}
		}
		massCenter = massCenterSum / massSum;
		mass = massSum;
	}
}

void Node::clear()
{
	if (!isLeaf)
	{
		for (Node* child : child)
		{
			if (child != nullptr)
			{
				child->clear();
				delete child;
			}
		}
	}
}