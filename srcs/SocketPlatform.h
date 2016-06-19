#include "Platform.h"
# ifdef PLATFORM_WINDOWS
#  include <winsock2.h>
#  include <ws2tcpip.h>
   typedef int SOCKLEN_T;
# elif defined PLATFORM_LINUX
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <unistd.h>
#  include <netdb.h>
#  include <fcntl.h>
#  define closesocket(s) ::close(s)
#  define INVALID_SOCKET -1
#  define SOCKET_ERROR -1
   typedef struct sockaddr_in SOCKADDR_IN;
   typedef socklen_t SOCKLEN_T;
   typedef int SOCKET;
# else
#  error Unsupported platform
# endif
