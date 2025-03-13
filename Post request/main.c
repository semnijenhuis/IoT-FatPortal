// C library headers
#include "curl.h"

static const char* webServer = "https://test.fatportal.nl/api/Ping";
static const char* ProductID = "acbf264b076c43c9aeb9a1d6d45a32ae";

int main() {
  char* jsonToSend = "{\\\"LOAD\\\":\\\"Test\\\"}";
  sendPostRequest(jsonToSend, webServer, ProductID);

  return 0;
}
