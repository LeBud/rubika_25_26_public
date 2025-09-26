#pragma once
#include <iostream>
// This is a forward declaration :)
// This is VERY useful and you must learn about it and use it as much as possible
class TextureMgr;
class GameMgr;

class Globals {
	
private:
	
	static inline Globals* InstancePtr = nullptr;
	
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
	void Destroy();
	
	static Globals* GetInstance() {
		if (InstancePtr == nullptr)
				InstancePtr = new Globals();
		
		return InstancePtr;
	}

	void CreateBaseEntity(std::string baseEntityName);

private:
	TextureMgr* textureMgr;
	GameMgr* gameMgr;

public:
	TextureMgr* GetTextureMgr() const;
	GameMgr* GetGameMgr() const;
};