#include "unp.hpp"
#include "tools.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>

using namespace std;

void switcher(string &s, bool &on)
{
    if(s.empty())
        return;
    if(s=="q")
        on=false;
	s="||||";
}

int main(int argc, char **argv)
{
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	//char buff[MAXLINE];
    string buff("");
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
	bind(listenfd, (struct sockaddr  *) &servaddr, sizeof(servaddr));
	listen(listenfd, LISTENQ);
    
    cout << "Working..." << endl; 
    
    while(true)
    {
    	connfd = accept(listenfd, (struct sockaddr *) NULL, NULL);
        int len = sizeof(struct sockaddr);
        getsockname(connfd, (struct sockaddr *) &servaddr,(socklen_t *) &len);
        
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

        //snprintf(buff, sizeof(buff), "Hello, %s, say something\n",fixedip.c_str());
        buff+="Hello, ";
        buff+=fixedip;
        buff+=" say something\n";

        write(connfd, buff.c_str(), buff.size());
  
        cout << "New client: " << fixedip << endl;
	  
        bool flagbuf=false;
        bool flagconnect=true; 
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
                flagbuf=true;
                s="||||";
            }
            if(!s.compare(0,1,"\\"))
                switcher(s.replace(0,1,""),flagconnect);

            if( s != "||||" && flagbuf)
            {
                system("echo -n \"\\033[34m\"");
                cout << buf;
                system("echo -n \"\\033[0m\"");
                system("./mpg123/mpg123 -q ./sound.mp3");
                buf="";
                flagbuf=false;
            }
            write(connfd, s.c_str(),s.size()+1);
             
            if(read(connfd,line,9999))
            {
                s=line;
                if(s!="||||") 
                {
                    if(!flagbuf)
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
                    system("./mpg123/mpg123 -q ./sound.mp3");
                }
            }
	        else
	        {
	            cout << "Client disconnected\n";    
                flagconnect=false;
            }   
    	}
		close(connfd);
    }
}
