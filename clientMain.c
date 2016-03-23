//Example usage of the clientCode.c "library". Tentative and will be replaced by a proper GUI application//

#include "clientCode.h"
#include <signal.h>

void receiveMessageCallback(char * message);
void receiveNewUserCallback(char * message);
void receiveUserLeftCallback(char * message);
void catchSignal(int num);
int keepRunning;

int main (int argc, char **argv)
{
	char message[BUFLEN];
	keepRunning = 1;

	if (argc < 4) {
		fprintf(stderr, "Usage: Server IP, Nickname and then icon #");
		return 0;
	}

	connectToServer(argv[1], receiveMessageCallback, receiveNewUserCallback, receiveUserLeftCallback, argv[2], atoi(argv[3]));

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

void receiveNewUserCallback(char * message) {
	printf("New User: %s\n", message);
}

void receiveUserLeftCallback(char * message) {
	printf("Lost user: %s\n", message);
}

void catchSignal(int num) {
	keepRunning = 0;
}
