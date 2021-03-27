#include "Sandbox.hpp"

#include <iostream>

void Sandbox::Initialize()
{
	std::cout << "Initialize" << std::endl;
}

void Sandbox::OnDestroy()
{
	std::cout << "Destroy" << std::endl;
}
