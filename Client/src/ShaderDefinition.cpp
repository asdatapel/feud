#include "ShaderDefinition.hpp"

#include <fstream>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string readFile(std::string filePath)
{
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open())
	{
		throw std::runtime_error(std::string("failed to read file: " + filePath));
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

ShaderDefinition ShaderDefinition::load(std::string name)
{
	std::string descriptionFile = readFile(name + ".desc");
	std::string vertFile = readFile(name + ".vs");
	std::string fragFile = readFile(name + ".fs");

	auto descriptionJson = json::parse(descriptionFile);

	std::list<Attribute> attributes;
	auto attributesJson = descriptionJson["attributes"];
	for (auto ja : attributesJson.items())
	{
		Attribute a = {ja.value()["name"], ja.value()["length"], ja.value()["stride"], ja.value()["distanceToFirst"]};
		attributes.push_back(a);
	}
	
	std::list<std::string> uniforms;
	auto uniformsJson = descriptionJson["uniforms"];
	for (auto ju : uniformsJson.items())
	{
		uniforms.push_back(ju.value());
	}

	return {vertFile, fragFile, attributes, uniforms};
}