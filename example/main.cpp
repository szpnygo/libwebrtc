#include "connection.h"
#include "master.h"

int main() {

  std::cout << "server starting" << std::endl;
  Master<Connection> master;
  master.bindUser("example");
  master.start();
}