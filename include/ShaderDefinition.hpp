#ifndef FEUD_SHADER_H
#define FEUD_SHADER_H

#include <string>
#include <list>
#include <unordered_map>


struct Attribute {
	std::string name;
	int length;
	int stride;
	int distanceToFirst;
};

struct ShaderDefinition {
    std::string vert;
    std::string frag;
	std::list<Attribute> attributes;
    std::list<std::string> uniforms;

    static ShaderDefinition load(std::string name);
};


#endif //FEUD_SHADER_H
