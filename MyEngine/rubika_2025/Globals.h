#pragma once
#include <iostream>
#include <mutex>

// This is a forward declaration :)
// This is VERY useful and you must learn about it and use it as much as possible
class TextureMgr;

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

	static Globals* getInstance() {
		if (InstancePtr == nullptr)
				InstancePtr = new Globals();
		
		return InstancePtr;
	}

	void Print() {
		std::cout << "Instance generated" << std::endl;
	}
	
	
private:
	TextureMgr* textureMgr;

public:
	TextureMgr* GetTextureMgr() const;
};
