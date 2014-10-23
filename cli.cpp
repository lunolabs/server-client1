#include "../key/unp.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

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


int main(int argc, char **argv)
{
	int	sockfd, n;
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
    short flagbuf=0;
    string buf("");
    string s("||||");   
    while(true) 
    {
        //cout << "Client: "; 
        if(kbhit())
             getline(cin,s);
	    else
            s="||||";
        if( s == "\\\\")
        {
        //    cout << "writing mode" << endl;
            flagbuf=1;
            s="||||";
        }
        if( s != "||||" && flagbuf==1)
        {
            system("echo -n \"\\033[34m\"");
            cout << buf;
            system("echo -n \"\\033[0m\"");
            system("mpg123 -q ./song.mp3");
            buf="";
            flagbuf=0;
        }
        write(sockfd, s.c_str(),s.size()+1);
	     
        if(read(sockfd,line,9999))
        {
            s=line;
            if(s!="||||") 
            {
                if(flagbuf==0)
                {
                    system("echo -n \"\\033[34m\"");
                    cout << servname << ": " << s << endl;
                    system("echo -n \"\\033[0m\"");
                }
                else
                {
                    buf+=servname;
                    buf+=s;
                    buf+="\n";
                }
                system("mpg123 -q ./song.mp3");
            }
        }
        else
        {
            cout << "disconnected" << endl;
            break; 
        }
        usleep(200000);
    }
	return 0;
}
