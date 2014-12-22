#include "unp.hpp"
<<<<<<< HEAD
=======
#include "tools.hpp"
>>>>>>> 65c32818b2cafcc18c5b5e033cc3c56761a2f0d0
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
<<<<<<< HEAD
#include <string>
#include <poll.h>
#include <vector> 

using namespace std;

class server
{
private:
	int listenfd, connfd;
	sockaddr_in servaddr;
	bool flag_work;
    void write_cli(size_t, string const &);
    int read_cli(size_t, string &);
    vector<pollfd> cli;
public:
	void init (int port);
	void working (void);
};

void server::write_cli(size_t no, string const  &mess)
{
    write(cli[no].fd, mess.c_str(), mess.size()+1);    
}

int server::read_cli(size_t no, string &buf)
{
    char temp[10000]="";
    int n = read(cli[no].fd, temp, 9999);
    buf = string(temp);
    return n;
}

void server::init (int port=1352)
{
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(port);
	bind(listenfd, (sockaddr *)&servaddr, sizeof(servaddr));
	listen(listenfd, LISTENQ);
	pollfd tmp;
    tmp.fd=listenfd;
	tmp.events=POLLRDNORM;
	cli.push_back(tmp);
	flag_work = true;
}

void server::working (void)
{
    cout << "Working... " << endl; 
	while(flag_work)
    {
		poll(&cli[0], cli.size(), INFTIM);
		if(cli[0].revents & POLLRDNORM)
		{
			connfd=accept(listenfd,(sockaddr*)NULL, NULL);	
			cout << "New cli: sock No " << (int)connfd << endl;
			pollfd tmp;
            tmp.fd = connfd;
			tmp.events = POLLRDNORM;
			cli.push_back(tmp);
			write_cli(cli.size()-1,string("Welcome!"));
            continue;
		}

        for(size_t i = 1; i < cli.size(); ++i)
        {
			if(cli[i].fd == -1 or cli[i].revents == 0)
                continue;
			cout <<"cli #" << i << " revents: 0x"<< hex << cli[i].revents << endl;
			string buf = "Can't read";
			if(cli[i].revents & (POLLRDNORM | POLLIN | POLLERR))
            {
                if(read_cli(i, buf) <= 0)
                {
                    close(cli[i].fd);
                    cout << "cli #" << i << " disconnected" << endl;
                    cli.erase(cli.begin()+i);
                    continue;
                }
                if(i == 2) {
                    write_cli(1,buf);
                } else if(i == 1) {
                    write_cli(2,buf);
                }
            }
        } 
	}
}

int main(int argc, char **argv)
{
	server srv;
	srv.init();
	srv.working();
    return 0;
=======

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
>>>>>>> 65c32818b2cafcc18c5b5e033cc3c56761a2f0d0
}
