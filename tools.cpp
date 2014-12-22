#include "tools.hpp"
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <iostream>

int kbhit (void)
{
    struct timeval tv;
    fd_set rdfs;
    tv.tv_sec = 0;
    tv.tv_usec = 0;          
    FD_ZERO(&rdfs);
    FD_SET (STDIN_FILENO, &rdfs);
    select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &rdfs);                    
}

std::string str_proc (std::string str)
{
	if(str.compare(0,3,".?/") || str.empty())
		return str;
	else if(!str.compare(0,7,".?/off:"))
	{
		return str_proc(str.replace(0,7,""));
	}
	else if(!str.compare(0,8,".?/from:"))
	{
		str.replace(0,8,"");
	}
	else
	{
		std::cout << "command not found!" << std::endl;
		return str;
	}
	return str_proc(str);
}
