#ifndef BUFFER_OVERFLOW_EXCEPTION_H
# define BUFFER_OVERFLOW_EXCEPTION_H

# include <exception>

namespace libnet
{

	class BufferOverflowException : public std::exception
	{

	private:
		std::string err;

	public:
		BufferOverflowException(std::string str): err(str) {};
		~BufferOverflowException() throw() {};
		const char *what() const throw() {return (this->err.c_str());};

	};

}

#endif
