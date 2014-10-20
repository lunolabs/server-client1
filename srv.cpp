#include	"../key/unp.h"
#include	<ctime>
#include <iostream>
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
	while(true)
    {
        cout << "Working..." << endl;
		connfd = accept(listenfd, (SA *) NULL, NULL);

        ticks = time(NULL);

        snprintf(buff, sizeof(buff), "Hello, client #%i \n",counter++);
        write(connfd, buff, strlen(buff));

		close(connfd);
	}
}
