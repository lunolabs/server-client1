#include	"../key/unp.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
int main(int argc, char **argv)
{
	int	sockfd, n;
//	char buff[100];
    struct sockaddr_in	servaddr;
	if (argc != 2)
    {
		cerr << "usage: a.out <IPaddress>";
        return 0;
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
        cout <<" "<< (char*) record->h_name<< endl;    
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
        cout << " connect success " << endl;
    FILE * r_connection=fdopen(sockfd, "r");
    FILE * w_connection=fdopen(sockfd, "w");
    char line[1000];
	string s("");
    while(true) 
    { 
        char *ch=fgets(line, 9999, r_connection);
        if (ch==NULL)
        { 
            printf("Disconnected\n");
            break; 
        }
	    else
	    {
            cout << line;	
        //	fputs(line, w_connection);
        //	write(sockfd, buff, strlen(buff));
	        while(true)
            {	
       	        getline(cin,s);
	    	    write(sockfd, s.c_str(),s.size()+1);
	        }
        }
    }
	return 0;
}
