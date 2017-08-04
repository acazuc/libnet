#ifndef LIBNET_PACKET_H
# define LIBNET_PACKET_H

namespace libnet
{

	class Packet
	{

	private:
		std::vector<uint8_t> datas;
		uint8_t position;

	public:
		Packet();

	};

}

#endif
