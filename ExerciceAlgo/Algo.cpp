#include "Algo.h"

#include <chrono>
#include <iostream>

Algo::Algo(const std::string& name) : Name(name)
{}

void Algo::StartComputation(unsigned nbElementInList)
{
	sTimeCompare t;
	t.NbElement = nbElementInList;
	t.TimeAllRetry = 0;
	t.TimeAvg = 0;
	ExecutionTimes.push_back(std::move(t));
}

void Algo::NewTestDuringComputation(const sTeam& team, std::vector<sEntity>& entities)
{
	std::vector<sEntity*> e;
	auto start = std::chrono::high_resolution_clock::now();
	ListEntities(team, entities, e);
	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double, std::milli> d = end - start;
	ExecutionTimes.back().TimeAllRetry += d.count();
}

void Algo::EndComputation(unsigned nbRetry)
{
	sTimeCompare& t = ExecutionTimes.back();
	t.TimeAvg = t.TimeAllRetry / nbRetry;
}

void Algo::PrintResult() const
{
	std::cout << Name << std::endl;
	for (const sTimeCompare& t : ExecutionTimes)
	{
		std::cout << "Nb Element : " << t.NbElement << "\tAvg Time(ms) " << t.TimeAvg << std::endl;
	}
}

unsigned TestAlgo::ListEntities(const sTeam& team, std::vector<sEntity>& entities, std::vector<sEntity*>& out) {

	out.reserve(entities.size());
	
	for (auto& e : entities) {
		if (e.Team.hash != team.hash) continue;
			out.push_back(&e);
	}

	return out.size();
}
