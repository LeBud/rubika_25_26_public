#pragma once
#include <iostream>
#include "Manager/RandomMgr.h"
#include "Manager/TaskMgr.h"

// This is a forward declaration :)
// This is VERY useful and you must learn about it and use it as much as possible
class TextureMgr;
class GameMgr;

class Globals {
private:
	static inline Globals* InstancePtr = nullptr;
	
	TextureMgr* textureMgr;
	GameMgr* gameMgr;
	RandomMgr* randomMgr;
	TaskMgr* taskMgr;
	
public:
	/*
	* Constructor & Destructor are here for a reason
	*/
	Globals();
	~Globals();

	/*
	* Init & Destroy of the class
	* They must be implemented in the right way to (de)initialize members
	*/
	void Init();
	void Shut();
	void Destroy();
	
	static Globals* GetInstance() {
		if (InstancePtr == nullptr)
				InstancePtr = new Globals();
		
		return InstancePtr;
	}

	void CreateBaseEntity(std::string baseEntityName);

	TextureMgr* GetTextureMgr() const;
	GameMgr* GetGameMgr() const;
	RandomMgr* GetRandomMgr() const;
	TaskMgr* GetTaskMgr() const;
};

extern Globals gData;