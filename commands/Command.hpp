#pragma once

#include <string>

class Client;

class Command
{
	public:
		Command(const std::string &name, const std::string &desc);
		virtual ~Command();
		const std::string &getName() const;
		const std::string &getDescription() const;

		virtual int execute(const std::string &command, Client &cli) const = 0;

	private:
		std::string name;
		std::string desc;
};
