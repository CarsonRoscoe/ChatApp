#include "clientCode.h"

void connectToServer(char * serverIP, int portNo, clientCodeCallback recvCallback, char * user) {
	struct hostent *hp;
	struct sockaddr_in server;
	char  *host, **pptr;
	char str[16];
	host = serverIP;
	port = portNo;
	callback = recvCallback;

	strcpy(username, user);

	// Create the socket
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Cannot create socket");
		exit(1);
	}
	bzero((char *)&server, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	if ((hp = gethostbyname(host)) == NULL)
	{
		fprintf(stderr, "Unknown server address\n");
		exit(1);
	}
	bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);

	// Connecting to the server
	if (connect (sd, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		fprintf(stderr, "Can't connect to server\n");
		perror("connect");
		exit(1);
	}
	printf("Connected:    Server Name: %s\n", hp->h_name);
	pptr = hp->h_addr_list;
	printf("\t\tIP Address: %s\n", inet_ntop(hp->h_addrtype, *pptr, str, sizeof(str)));

	threadID = pthread_create(&thread, NULL, receiveThread, NULL);
}

void sendMessage(const char * message) {
	char sbuf[BUFLEN];
  //gets(sbuf); // get user's text
  sprintf(sbuf, "[%s] %s", username, message);

  // Transmit data through the socket
  send (sd, sbuf, BUFLEN, 0);
}

char * receiveMessage() {
	char * bp;
	int bytes_to_read = BUFLEN;
	int n = 0;

	bp = rbuf;

	while ((n = recv (sd, bp, bytes_to_read, 0)) < BUFLEN) {
		bp += n;
		bytes_to_read -= n;
	}

	return rbuf;
}

void closeConnection() {
	sendMessage("fail");
	sleep(1);
	pthread_cancel(thread);
	close (sd);
}

void * receiveThread(void * ptr) {
	while(1) {
		callback(receiveMessage());
	}
}
