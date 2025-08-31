#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <typeinfo>
#include "Core.h"

class Shader;
class Engine_API ShaderLoader
{
public:

	ShaderLoader();
	~ShaderLoader() = default;

	template<typename T, typename std::enable_if<std::is_base_of<Shader, T>::value>::type* = nullptr>
	bool Load(std::weak_ptr<T>& outShader);

	static ShaderLoader& Get();

private:
	static ShaderLoader* instance;
	std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
};

//#include "ShaderLoader.inl"