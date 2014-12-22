#include "unp.hpp"
#include "tools.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
<<<<<<< HEAD
#include <poll.h>

using namespace std;

class Client
{
	pollfd sock[1];
    sockaddr_in	servaddr;
    bool flag_work;
    bool flag_outmss;
    string username;
public:
	void init (int Nport, char *recvline, string usnm);
	void worked (void);
    void sound() {system("mpg123 -q ./sound.mp3");};
};

void Client::init (int Nport, char *recvline, string usnm) //Nport=1352
{
	flag_work = false;
	username = usnm;
    hostent *record = gethostbyname(recvline);
    if(record == NULL) {
        cerr << "no server";
        return;
    } else {
        cout << "IP server: " << (char*) record->h_name<< endl;    
    }    
    in_addr *addressptr = (in_addr *) record->h_addr;
	if((sock[0].fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		cerr << "socket error";
		return;
	}
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
>>>>>>> 65c32818b2cafcc18c5b5e033cc3c56761a2f0d0

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr = *addressptr;
<<<<<<< HEAD
	servaddr.sin_port = htons(Nport);
   
    int r = connect(sock[0].fd,(sockaddr*) &servaddr, sizeof(servaddr)); 
	sock[0].events = (POLLRDNORM | POLLWRNORM);
	
	if(r < 0) {	
		cerr << "connect error" << endl;
		return;
	} else {
        cout << "connect success" << endl; }

	flag_work = true;
	flag_outmss = false;
}	

void Client::worked(void)
{
	string buff("");
	while(flag_work) 
    {
        poll(sock,1,1000);
		if( kbhit() ) {
			getline(cin,buff);
			flag_outmss = true;
		}
		if(sock[0].revents & (POLLRDNORM | POLLIN)) {
			char line[10000] = "";	
			if(read(sock[0].fd,line,9999)) {
				system("echo -n \"\\033[32m\"");
				cout << line << endl;
				system("echo -n \"\\033[0m\"");
			    sound();
            } else {
				cout << "disconnected" << endl;
				flag_work = false; 
			}
		}
		if( (sock[0].revents & (POLLWRNORM | POLLOUT)) && flag_outmss) {
			write(sock[0].fd, buff.c_str(),buff.size());
			flag_outmss = false;
			buff = "";	
		}
    }
}
int main(int argc, char **argv)
{
	if (argc < 2) {
		cerr << "usage: ./programm <IPaddress>";
        return 0; }
	char *recvline = argv[1];
    string username = "client ";
	username += recvline;
	if(argc == 3) {
        username = argv[2];	}
	Client cli;
	cli.init(PORT, recvline, username);
	cli.worked();
=======
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
                system("./mpg123/mpg123 -q ./sound.mp3");
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
                system("./mpg123/mpg123 -q ./sound.mp3");
            }
        }
        else
        {
            cout << "disconnected" << endl;
            flagconnect=false;; 
        }
        usleep(10000);
    }
>>>>>>> 65c32818b2cafcc18c5b5e033cc3c56761a2f0d0
	return 0;
}
