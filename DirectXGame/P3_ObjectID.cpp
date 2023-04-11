#include "P3_ObjectID.h"

P3_ObjectID::P3_ObjectID(int ID, P3_ObjectType objectType, int sceneIndex, Vector3D scale)
{
	this->ID = ID;
	this->objectType = objectType;
	this->sceneIndex = sceneIndex;
	this->scale = scale;
}

P3_ObjectID::~P3_ObjectID()
{
}
