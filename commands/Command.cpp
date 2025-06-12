#include "Command.hpp"
#include <iostream>

Command::Command(const std::string &name, const std::string &desc): name(name), desc(desc)
{
	std::cout << "Command " << this->name << " loaded !" << std::endl;
}

Command::~Command()
{
	std::cout << "Unloaded command " << this->name << "." << std::endl;
}

const std::string &Command::getName() const
{
	return this->name;
}

const std::string &Command::getDescription() const
{
	return this->desc;
}
