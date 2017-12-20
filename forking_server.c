#include "pipe_networking.h"
#include <signal.h>

void process(char *s);
void subserver(int from_client);

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove("luigi");
    exit(0);
  }
}

int main() {
  int from_client, f;
  while (from_client = server_setup()) {
    f = fork();
    if(!f){
      subserver(from_client);
      exit(0);
    }
  }
}

void subserver(int from_client) {
  int to_client = server_connect(from_client);
  char buffer[BUFFER_SIZE];
  
  while (read(from_client, buffer, sizeof(buffer))) {
    
    printf("[server] received: %s\n", buffer);
    process(buffer);
    printf("[server] modified: %s\n", buffer);
    
    write(to_client, buffer, sizeof(buffer));
  }
}

void process(char * s) {
  int i;
  for (i = 0; s[i]; i++) {
    if (i % 2 == 0) {
      s[i] = toupper(s[i]);
    }
    else {
      s[i] = tolower(s[i]);
    }
  }
}
