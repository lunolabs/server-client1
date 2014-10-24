#include "../key/unp.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <sstream>
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
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	char buff[MAXLINE];
    string username("Server");
    if(argc == 2)
    {
        username = argv[1];    
    }
    
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(1352);	/* daytime server */
	bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
	listen(listenfd, LISTENQ);
    
    cout << "Working..." << endl;
    
    sockaddr ipsave;
    ipsave.sa_family = AF_INET;
    
    
    while(true)
    {
    	connfd = accept(listenfd, (SA *) NULL, NULL);
        int len = sizeof(struct sockaddr);
        getsockname(connfd, (SA *) &servaddr,(socklen_t *) &len);
        
        stringstream str;
        str << int(servaddr.sin_addr.s_addr&0xFF)        <<"."
        << int((servaddr.sin_addr.s_addr&0xFF00)>>8)     <<"."
        << int((servaddr.sin_addr.s_addr&0xFF0000)>>16)  <<"."
        << int((servaddr.sin_addr.s_addr&0xFF000000)>>24);
        
        string fixedip(""); 
        str >> fixedip;
        char cliname[100];
        read(connfd, cliname, 100);
        write(connfd, username.c_str(), 999);

        snprintf(buff, sizeof(buff), "Hello, %s, say something\n",fixedip.c_str());
        
        write(connfd, buff, strlen(buff));
  
        cout << "New client: " << fixedip << endl;
	  
        short flagbuf=0;
        short flagconnect=1; 
	    char line[1000];
        string s("||||");    
        string buf("");

        while(flagconnect)
        {
            //cout << "Server: "; 
            if (kbhit())
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
                system("mpg123 -q ./sound.mp3");
                buf="";
                flagbuf=0;
            }
            write(connfd, s.c_str(),s.size()+1);
             
            if(read(connfd,line,9999))
            {
                s=line;
                if(s!="||||") 
                {
                    if(flagbuf==0)
                    {
                        system("echo -n \"\\033[34m\"");
                        cout << cliname << ": " << s << endl;
                        system("echo -n \"\\033[0m\"");
                    }
                    else
                    {
                        buf+=cliname;
                        buf+=": ";
                        buf+=s;
                        buf+="\n";
                    }
                    system("mpg123 -q ./sound.mp3");
                }
            }
	        else
	        {
	            cout << "Client disconnected\n";    
                flagconnect=0;
            }   
    	}
		close(connfd);
    }
}
