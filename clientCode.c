#include "clientCode.h"

void connectToServer(char * serverIP, int portNo, clientCodeCallback recvCallback, clientCodeCallback newClientCallback, clientCodeCallback leftClientCallback, char * user, int ico) {
	struct hostent *hp;
	struct sockaddr_in server;
	char  *host, **pptr;
	char str[16], newuser[USERNAMELEN+1];
	host = serverIP;
	port = portNo;
	recvMessage = recvCallback;
	newUser = newClientCallback;
	leftUser = leftClientCallback;

	strcpy(username, user);
	icon = ico;

	// Create the socket
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
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

	threadID = pthread_create(&thread, NULL, receiveThread, NULL);

	sprintf(newuser, "%c%s%c%lu", NEWUSER, user, MESSAGEDELIMITER, icon);

	send (sd, newuser, BUFLEN, 0);
}

void sendMessage(const char * message) {
	char sbuf[BUFLEN];
  //gets(sbuf); // get user's text
  sprintf(sbuf, "%s%c%lu%c%s", username, MESSAGEDELIMITER, icon, MESSAGEDELIMITER, message);

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
	pthread_cancel(thread);
	close (sd);
}

void * receiveThread(void * ptr) {
	char buf[BUFLEN], newbuf[BUFLEN];
	while(1) {
		strcpy(buf, receiveMessage());
		if (strlen(buf) > 1) {
			switch(buf[0]) {
				case NEWUSER:
					memmove(newbuf, buf + 1, strlen(buf)-1);
					newbuf[strlen(buf)-1] = '\0';
					newUser(newbuf);
					break;
				case USERLEFT:
					memmove(newbuf, buf + 1, strlen(buf)-1);
					newbuf[strlen(buf)-1] = '\0';
					leftUser(newbuf);
					break;
				default:
					recvMessage(buf);
					break;
			}
		}
	}
}
