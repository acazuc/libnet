#ifndef PLATFORM_H
# define PLATFORM_H

# ifdef _WIN32
#  define PLATFORM_WINDOWS
# elif defined __linux__
#  define PLATFORM_LINUX
# endif

#endif
