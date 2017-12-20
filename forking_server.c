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
  signal(SIGINT, sighandler);
  
  while(1) {
    int from_client = server_setup();
    if(!fork()) {
      subserver(from_client);
      exit(0);
    }
  }
}

void subserver(int from_client) {
  int to_client = server_connect(from_client);
  char buffer[BUFFER_SIZE];
  
  while (read(from_client, buffer, sizeof(buffer))) {
    process(buffer);    
    write(to_client, buffer, sizeof(buffer));
  }
  
  close(to_client);
  close(from_client);
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
