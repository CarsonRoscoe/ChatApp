/*------------------------------------------------------------------------------------------------------------------
-- PROGRAM: ChatApp
--
-- FILE: clientCode.c
--
-- DATE: March 20th, 2016
--
-- REVISIONS: March 20th, 2016: Created
--						March 22nd, 2016: Added more functionality
--						March 23rd, 2016: Commented
--
-- DESIGNER: Carson Roscoe
--
-- PROGRAMMER: Carson Roscoe
--
-- FUNCTIONS: void connectToServer(Server IP, callback, callback, callback, nickname, icon)
--					  void sendMessage(message to send)
--						char * receiveMessage()
--						void closeConnection()
--						void * receiveThread(void * ptr) (NOTE: Both return and param are unused)
--
-- NOTES:
-- The client code was designed to be blackboxed out of all GUI logic. It was designed to be reusable in future
-- applications, and as such is completely isolated from the rest of the application with the exception
-- of the connectToServer, sendMessage and closeConnection calls.
--
-- To update the GUI with received data, the client code utilizes callbacks passed in to the connectToServer function.
-- These function callbacks return void and simply take in a char * of the message received. There are three callbacks,
-- one for receiving a chat message, one for receiving a control message saying a user has connected, and a third for
-- receiving a control message saying a user has disconnected.
--
-- Control messages start with a control flag (DC2 for new user, DC3 for disconnected user), and all information being
-- send is delimited by a DC1 flag.
----------------------------------------------------------------------------------------------------------------------*/

#include "clientCode.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: connectToServer
--
-- DATE: March 20th, 2016
--
-- REVISIONS: March 20th, 2016: Created
--						March 22nd, 2016: Added more functionality
--						March 23rd, 2016: Commented
--
-- DESIGNER: Carson Roscoe
--
-- PROGRAMMER: Carson Roscoe
--
-- INTERFACE: void connectToServer(Server's IP address
--																 Callback that is invoked when another user sends a message
--																 Callback that is invoked when someone new joins the chatroom
--																 Callback that is invoked when someone left the chatroom
--																 Username we want displayed to other users
--																 The index value of the icon we want displayed to other users)
--
-- RETURN: void
--
-- NOTES:
-- Function called to establish a connection to the server. The first parameter is the IP address needed to establish
-- a TCP connection to the server. Next are three callbacks that are invoked by our read thread whenever new information
-- arrises. The first one is called only when someone sends a message over chat, the next when someone connects to the
-- chatroom and the third when someone disconnects. The final two parameters are used for telling the server our nickname
-- to display when we send messages, as well as what icon index we will be using when we type.
--
-- The function will establish a connection to the server, instantiate the proper variables and then start a read
-- thread that will read continously and invoke the callbacks listed when appropriate.
----------------------------------------------------------------------------------------------------------------------*/
void connectToServer(char * serverIP, clientCodeCallback recvCallback, clientCodeCallback newClientCallback, clientCodeCallback leftClientCallback, char * user, int ico) {
	struct hostent *hp;
	struct sockaddr_in server;
	char  *host, **pptr;
	char str[16], newuser[USERNAMELEN+1];

	//Set our connection data & callbacks
	host = serverIP;
	port = PORTNO;
	recvMessage = recvCallback;
	newUser = newClientCallback;
	leftUser = leftClientCallback;
	icon = ico;
	strcpy(username, user);


	// Create the socket
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Cannot create socket");
		exit(1);
	}

	bzero((char *)&server, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	//Get the server address
	if ((hp = gethostbyname(host)) == NULL) {
		fprintf(stderr, "Unknown server address\n");
		exit(1);
	}

	bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);

	//Connect to the server
	if (connect (sd, (struct sockaddr *)&server, sizeof(server)) == -1) {
		fprintf(stderr, "Can't connect to server\n");
		perror("connect");
		exit(1);
	}

	//Create our read thread that will read data on the socket
	threadID = pthread_create(&thread, NULL, receiveThread, NULL);

	//Send a packet to the server stating we are a new user, which will be echo'd to all connected clients
	sprintf(newuser, "%c%s%c%lu", NEWUSER, user, MESSAGEDELIMITER, icon);
	send (sd, newuser, BUFLEN, 0);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: sendMessage
--
-- DATE: March 20th, 2016
--
-- REVISIONS: March 20th, 2016: Created
--						March 23rd, 2016: Commented
--
-- DESIGNER: Carson Roscoe
--
-- PROGRAMMER: Carson Roscoe
--
-- INTERFACE: void sendMessage(message to send to the server grabbed from the UI's chat box)
--
-- RETURN: void
--
-- NOTES:
-- Function used to send a message to the server when the user has typed one into the chat box and clicked the send
-- button. It takes the message string, stores it into a buffer along with the username * icon index for displaying
-- purposes, and then writes that to the connected socket.
----------------------------------------------------------------------------------------------------------------------*/
void sendMessage(const char * message) {
	char sbuf[BUFLEN];
  //Format packet
  sprintf(sbuf, "%s%c%lu%c%s", username, MESSAGEDELIMITER, icon, MESSAGEDELIMITER, message);
  //Send formatted packet through the socket
  send (sd, sbuf, BUFLEN, 0);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: receiveMessage
--
-- DATE: March 20th, 2016
--
-- REVISIONS: March 20th, 2016: Created
--						March 23rd, 2016: Commented
--
-- DESIGNER: Carson Roscoe
--
-- PROGRAMMER: Carson Roscoe
--
-- INTERFACE: char * receiveMessage()
--
-- RETURN: char * that is the message read from the server
--
-- NOTES:
-- Reads the next packet from the server and returns it as a char * string
----------------------------------------------------------------------------------------------------------------------*/
char * receiveMessage() {
	char * bufPointer;
	int bytesToRead = BUFLEN;
	int bytesRead = 0;

	bufPointer = rbuf;

	while ((bytesRead = recv (sd, bufPointer, bytesToRead, 0)) < BUFLEN) {
		bufPointer += bytesRead;
		bytesToRead -= bytesRead;
	}

	return rbuf;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: closeConnection
--
-- DATE: March 20th, 2016
--
-- REVISIONS: March 20th, 2016: Created
--						March 23rd, 2016: Commented
--
-- DESIGNER: Carson Roscoe
--
-- PROGRAMMER: Carson Roscoe
--
-- INTERFACE: void closeConnection()
--
-- RETURN: void
--
-- NOTES:
-- Called when we are disconnecting from the server. It first terminates our reading thread, and then closes
-- the socket that is connected to the server.
----------------------------------------------------------------------------------------------------------------------*/
void closeConnection() {
	pthread_cancel(thread);
	close (sd);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: receiveThread
--
-- DATE: March 20th, 2016
--
-- REVISIONS: March 20th, 2016: Created
--						March 23rd, 2016: Commented
--
-- DESIGNER: Carson Roscoe
--
-- PROGRAMMER: Carson Roscoe
--
-- INTERFACE: void * receiveThread(a void pointer that is unused)
--
-- RETURN: void * that is unused
--
-- NOTES:
-- After a successful connection has been made to the server, a posix thread is created and given this function as
-- it's starting point. The function loops forever hanging on a receiveMessage call until a packet is received. Once
-- a packet is received, it is then checked to see what type of packet it is. If it starts with a NEWUSER flag (DC2),
-- it is a packet telling us a new user has joined and we pass it over to the appropriate callback. If it starts with
-- a USERLEFT flag (DC3), it is passed over to the appropriate callback. If there is no flag, it is a raw message and
-- passed to the recvMessage callback.
----------------------------------------------------------------------------------------------------------------------*/
void * receiveThread(void * ptr) {
	char buf[BUFLEN], newbuf[BUFLEN];
	while(1) {
		//Hang waiting for a packet to be received
		strcpy(buf, receiveMessage());

		//On successful receive and on valid packet, check what type of packet it is
		if (strlen(buf) > 1) {
			switch(buf[0]) {
				//If it is a packet regarding a new user joining, pass it to the newUser callback
				case NEWUSER:
					//First, remove the NEWUSER control flag
					memmove(newbuf, buf + 1, strlen(buf)-1);
					newbuf[strlen(buf)-1] = '\0';
					newUser(newbuf);
					break;
				//If it is a packet regarding a user leaving, pass it to the leftUser callback
				case USERLEFT:
					//First, remove the USERLEFT control flag
					memmove(newbuf, buf + 1, strlen(buf)-1);
					newbuf[strlen(buf)-1] = '\0';
					leftUser(newbuf);
					break;
				//If no flag, it is a genuine chat message and is passed to the recvMessage callback as is
				default:
					recvMessage(buf);
					break;
			}
		}
	}
}
