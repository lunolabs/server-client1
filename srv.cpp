#include	"../key/unp.h"
#include	<ctime>
#include <cstdlib>
#include <iostream>
#include <sstream>
using namespace std;

int main(int argc, char **argv)
{
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	char buff[MAXLINE];
	time_t ticks;
    cout << "Setting up socket" << endl;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(1352);	/* daytime server */

    cout << "Binding servaddr" << endl;
	bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

    cout << "Preparing to listen" << endl;
	listen(listenfd, LISTENQ);
    cout << "Listening" << endl;
	int counter = 1;
	char line[1000];
    cout << "Working..." << endl;
    string s("");
    sockaddr ipsave;
    ipsave.sa_family = AF_INET;
    stringstream str;
    string fixedip("");
    while(true)
    {
    //	connfd = accept(listenfd, (SA *) &ipsave, NULL);
    	connfd = accept(listenfd, (SA *) NULL, NULL);
        int len = sizeof(struct sockaddr);
        getsockname(connfd, (SA *) &servaddr,(socklen_t *) &len);
        
        str << int(servaddr.sin_addr.s_addr&0xFF) <<"."
        <<int((servaddr.sin_addr.s_addr&0xFF00)>>8)<<"."
       << int((servaddr.sin_addr.s_addr&0xFF0000)>>16)<<"."
       << int((servaddr.sin_addr.s_addr&0xFF000000)>>24);            
        str >> fixedip;
        snprintf(buff, sizeof(buff), "Hello, %s, say something\n",fixedip.c_str());
        write(connfd, buff, strlen(buff));
        s = "New client ";
        
        s+=fixedip;
        s+="\n";
        cout << s;
	    while(true)
        {
	        if(read(connfd, line, 9999))
	        {
                s = line;
    	    	cout << s << endl;
    	    }
	        else
	        {
	        	cout << "Client disconnected\n";    
                break;
            }   
		}
		close(connfd);
    }
}
