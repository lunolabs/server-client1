#include "unp.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
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
}
