#include <chrono>
#include <vector>
#include <string>
#include <random>
#include <iostream>

#include "Shared.h"
#include "Algo.h"

std::vector<sTeam> g_vTeamNames =
{
	{ "Les plus forts" },
	{ "TOTOTATATUTU" },
	{ "A" },
	{ "Ireland Lions" },
	{ "Black Donkeys" },
	{ "Courageous Elephants" },
	{ "Selfish Tadpols" },
};

#define SEED 0x64f8e1aa02

unsigned PickupRandomTeamIndex()
{
	return rand() % g_vTeamNames.size();
}

void GenerateEntities(unsigned size, std::vector<sEntity>& entities)
{
	entities.clear();
	entities.reserve(size);
	for (unsigned i = 0; i < size; ++i)
	{
		unsigned teamIndex = PickupRandomTeamIndex();
		entities.push_back(sEntity(i, g_vTeamNames[teamIndex]));
	}
}

int main()
{
	srand(time(NULL));
	//srand(SEED);

	std::vector<Algo*> algos;
	{
		// Add an implementation of Algo class here, using new
		algos.push_back(new TestAlgo());
	}

	const unsigned retry = 25;
	for (unsigned nb = 10; nb <= 10'000'000; nb *= 10)
	{
		std::cout << "Test with " << nb << " elements" << std::endl;

		for (Algo* algo : algos)
		{
			algo->StartComputation(nb);
		}

		for (unsigned i = 0; i < retry; ++i){
			std::cout << "\tTry " << i + 1 << " / " << retry << std::endl;
			const sTeam& team = g_vTeamNames[PickupRandomTeamIndex()];

			std::vector<sEntity> allEntities;
			GenerateEntities(nb, allEntities);

			for (Algo* algo : algos){
				algo->NewTestDuringComputation(team, allEntities);
			}
		}
		
		for (Algo* algo : algos){
			algo->EndComputation(retry);
		}
	}



	std::cout << "Compare " << algos.size() << " algo" << std::endl;
	for (Algo* algo : algos)
	{
		algo->PrintResult();
		std::cout << std::endl;
	}

	for (Algo* algo : algos)
	{
		delete algo;
	}
}