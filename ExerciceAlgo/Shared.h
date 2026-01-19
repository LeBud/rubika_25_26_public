#pragma once

#include <string>
#include <unordered_map>

struct sTeam
{
	sTeam(const std::string& s)
	{
		Name = s;
		hash = std::hash<std::string>{}(Name);
	}

	std::string Name;
	std::size_t hash;
};

struct sEntity
{
	sEntity(unsigned id, const sTeam& team) :
		Id(id), Team(team)
	{}

	unsigned Id;
	sTeam Team;
};