#include "unp.hpp"
#include "tools.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

void switcher(string &s, bool &on)
{
    if(s.empty())
        return;
    if(s=="q")
        on=false;
}

int main(int argc, char **argv)
{
	int	sockfd;
    struct sockaddr_in	servaddr;
	if (argc != 2 && argc!=3)
    {
		cerr << "usage: a.out <IPaddress>";
        return 0;
    }
    string username("Client");
    if(argc == 3)
    {
        username = argv[2];    
    }
	char *recvline = argv[1];
    hostent *record = gethostbyname(recvline);
    if(record == NULL)
    {
        cerr << "no server";
        return 0;
    }
    else
    {
        cout << "IP server: " << (char*) record->h_name<< endl;    
    }    
    in_addr *addressptr = (in_addr *) record->h_addr;

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		cerr << "socket error";

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr = *addressptr;
	servaddr.sin_port   = htons(1352);	/* daytime server */
   
    int r = connect(sockfd,(sockaddr*) &servaddr, sizeof(servaddr)); 
    if(r < 0)
		cerr << "connect error" << endl;
    else
        cout << "connect success" << endl;
    write(sockfd, username.c_str(),username.size()+1);
    
    char servname[100];
    read(sockfd, servname, 999);

    char line[1000];
    bool flagbuf=false;
    bool flagconnect=true;
    string buf("");
    string s("||||");   

    while(flagconnect) 
    {
        if( kbhit() )
            getline(cin,s);
	    else
            s="||||";
        if( s == "\\\\")
        {
            flagbuf=true;
            s="||||";
        }
        if(!s.compare(0,1,"\\"))
            switcher(s.replace(0,1,""),flagconnect);
        if( s != "||||" && flagbuf==1 )
        {
            if( buf!="" )
            {
                system("echo -n \"\\033[34m\"");
                cout << buf;
                system("echo -n \"\\033[0m\"");
                system("mpg123 -q ./sound.mp3");
                buf="";
            }
            flagbuf=false;
        }
        write(sockfd, s.c_str(),s.size()+1);
	     
        if(read(sockfd,line,9999))
        {
            s=line;
            if(s!="||||") 
            {
                if(!flagbuf)
                {
                   system("echo -n \"\\033[34m\"");
                   cout << servname << ": " << s << endl;
                   system("echo -n \"\\033[0m\"");
                }
                else
                {
                   buf+=servname;
                   buf+=": ";
                   buf+=s;
                   buf+="\n";
                 }
                system("mpg123 -q ./sound.mp3");
            }
        }
        else
        {
            cout << "disconnected" << endl;
            flagconnect=false;; 
        }
        usleep(10000);
    }
	return 0;
}
