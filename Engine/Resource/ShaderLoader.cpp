#include "ShaderLoader.h"
#include "Shader/Shader.h"

ShaderLoader* ShaderLoader::instance = nullptr;

ShaderLoader::ShaderLoader()
{
	instance = this;
}

ShaderLoader& ShaderLoader::Get()
{
	return *instance;
}