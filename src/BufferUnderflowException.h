#ifndef BUFFER_UNDERFLOW_EXCEPTION_H
# define BUFFER_UNDERFLOW_EXCEPTION_H

# include <exception>

namespace libnet
{

	class BufferUnderflowException : public std::exception
	{

	private:
		std::string err;

	public:
		BufferUnderflowException(std::string str): err(str) {};
		~BufferUnderflowException() throw() {};
		const char *what() const throw() {return (this->err.c_str());};

	};

}

#endif
