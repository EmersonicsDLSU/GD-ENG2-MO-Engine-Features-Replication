#pragma once
#include "Vector3D.h"

enum class P3_ObjectType;

class P3_ObjectID
{
public:
	P3_ObjectID(int ID, P3_ObjectType objectType, int sceneIndex, Vector3D scale = { 1,1,1 });
	~P3_ObjectID();
public:
	int ID;
	P3_ObjectType objectType;
	int sceneIndex;
	Vector3D scale;
};

