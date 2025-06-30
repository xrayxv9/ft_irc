#pragma once

#include <vector>

#include "Command.hpp"
#include "../channel/Channel.hpp"

class Mode : public Command
{
	public :
		Mode();
		virtual int execute( const std::string &, Client * ) const;
		void iCommand( bool, Channel *, Client * ) const;
		void kCommand( std::string, bool, Channel *, Client * ) const;
		void tCommand( bool, Channel *, Client * ) const;
		void oCommand( bool, Channel *, Client * , Client * ) const;
		void lCommand( int, bool, Channel *, Client * ) const;
		std::vector<std::string> getAllArgs(std::string  &command) const;
};
