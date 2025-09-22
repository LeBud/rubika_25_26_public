#pragma once

#include <string>

struct sTeam
{
	sTeam(const std::string& s)
	{
		Name = s;
	}

	std::string Name;
};

struct sEntity
{
	sEntity(unsigned id, const sTeam& team) :
		Id(id), Team(team)
	{}

	unsigned Id;
	sTeam Team;
};