#ifndef LIBNET_PLATFORM_H
# define LIBNET_PLATFORM_H

# ifdef _WIN32
#  define PLATFORM_WINDOWS
# elif defined __linux__
#  define PLATFORM_LINUX
# endif

#endif
