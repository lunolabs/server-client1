#include "unp.hpp"
#include "tools.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
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

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr = *addressptr;
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
	return 0;
}
