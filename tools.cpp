#include "tools.hpp"
#include <cstdlib>
#include <unistd.h>
<<<<<<< HEAD
#include <string>
#include <iostream>
=======
>>>>>>> 65c32818b2cafcc18c5b5e033cc3c56761a2f0d0

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
<<<<<<< HEAD

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
=======
>>>>>>> 65c32818b2cafcc18c5b5e033cc3c56761a2f0d0
