#include "clientCode.h"
#include <signal.h>

void receiveMessageCallback(char * message);
void catchSignal(int num);
int keepRunning;

int main (int argc, char **argv)
{
	char message[BUFLEN];
	keepRunning = 1;
	connectToServer("127.0.0.1", 7000, receiveMessageCallback);
	while(keepRunning == 1) {
		scanf("%s", message);
		sendMessage(message);
	}

	printf("Cleanup");
	closeConnection();

	return (0);
}

void receiveMessageCallback(char * message) {
	printf("%s\n", message);
}

void catchSignal(int num) {
	keepRunning = 0;
}
