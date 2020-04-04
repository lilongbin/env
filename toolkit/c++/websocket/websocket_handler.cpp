#include <string.h>
#include <unistd.h>
#include "websocket_handler.h"

Websocket_Handler::Websocket_Handler(int fd):
	buff_(),
	status_(WEBSOCKET_UNCONNECT),
	header_map_(),
	fd_(fd),
	request_(new Websocket_Request)
{
}

Websocket_Handler::~Websocket_Handler()
{
}

int Websocket_Handler::process()
{
	if (status_ == WEBSOCKET_UNCONNECT) {
		return handshake();
	}

	response();
	// request_->print();
	memset(buff_, 0, sizeof(buff_));

	return 0;
}

int Websocket_Handler::handshake()
{
	char reply[1024] = { };
	status_ = WEBSOCKET_HANDSHARKED;
	fetch_http_info();
	fill_handshake_msg(reply);
	memset(buff_, 0, sizeof(buff_));

	return websocket_reply(reply);
}

int Websocket_Handler::disconnect()
{
	char reply[1024] = { };
	request_->set_fin(1);
	request_->set_opcode(8);
	request_->set_mask(1);
	request_->set_payload_length(0);
	// set_masking_key();
	request_->fill_response_websocket_msg(reply);
	websocket_reply(reply);
	return true;
}
int Websocket_Handler::ping_pong()
{
	char reply[1024] = { };
	request_->set_fin(1);
	if (request_->get_opcode() == 9)
	{
		request_->set_opcode(10);
	}
	else
	{
		request_->set_opcode(9);
	}
	request_->set_mask(1);
	request_->set_payload_length(0);
	// set_masking_key();
	request_->fill_response_websocket_msg(reply);
	websocket_reply(reply);
	request_->parse_websocket_info(reply);
	request_->print();
	return true;
}

int Websocket_Handler::post_data(char *buf, uint64_t size)
{
	char reply[4096] = {0};
	uint64_t len = size;
	uint64_t pos = 0;

	reply[0] = 0x81;
	// 回复的数据均不带mask
	if (len < 126)
	{
		reply[1] = (char)len;
		pos += 2;
	}
	else if (len < 0xFFFF)
	{
		reply[1] = 126;
		reply[2] = (char)(len & 0xFF);
		reply[3] = (char)(len >> 8 & 0xFF);
		pos += 4;
	}
	else
	{
		reply[1] = 127;
		reply[2] = (char)(len >> 0 & 0xFF);
		reply[3] = (char)(len >> 8 & 0xFF);
		reply[4] = (char)(len >> 16 & 0xFF);
		reply[5] = (char)(len >> 24 & 0xFF);
		reply[6] = (char)(len >> 32 & 0xFF);
		reply[7] = (char)(len >> 40 & 0xFF);
		reply[8] = (char)(len >> 48 & 0xFF);
		reply[9] = (char)(len >> 56 & 0xFF);
		pos += 10;
	}
	strncpy(&reply[pos], buf, len);
	reply[pos+len] = '\0';

	websocket_reply(reply);
	return true;
}

int Websocket_Handler::main_proc(char *payload_)
{
	printf("[%s] processing ...\n", __func__);
	printf("Payload:\n%s\n", payload_);
	char buf0[512] = {0};
	snprintf(buf0, sizeof(buf0), "Server has recieved:\n%s", payload_);
	char buf1[512] = "Server is processing, please wait ...";
	char buf2[512] = "Yes, Just do IT.";
	uint64_t size0 = strlen(buf0);
	uint64_t size1 = strlen(buf1);
	uint64_t size2 = strlen(buf2);
	post_data(buf0, size0);
	post_data(buf1, size1);
	post_data(buf2, size2);
	return true;
}

int Websocket_Handler::response()
{
	char payload_[4096] = {0};
	request_->parse_websocket_info(buff_);
	request_->print();
	strncpy(payload_, request_->get_payload(), sizeof(payload_));
	// printf("Payload:\n%s\n", payload_);

	uint8_t opcode_ = request_->get_opcode();
	switch (opcode_)
	{
		case 1:
			main_proc(payload_);
			break;
		case 8:
			disconnect();
			break;
		case 9:
		case 10:
			ping_pong();
			break;
		default:
			printf("default\n");
			break;
	}
}

void Websocket_Handler::fill_handshake_msg(char *reply)
{
	strcat(reply, "HTTP/1.1 101 Switching Protocols\r\n");
	strcat(reply, "Connection: upgrade\r\n");
	strcat(reply, "Sec-WebSocket-Accept: ");
	std::string server_key = header_map_["Sec-WebSocket-Key"];
	server_key += MAGIC_KEY;

	SHA1 sha;
	unsigned int message_digest[5];
	sha.Reset();
	sha << server_key.c_str();

	sha.Result(message_digest);
	for (int i = 0; i < 5; i++)
	{
		message_digest[i] = htonl(message_digest[i]);
	}
	server_key = base64_encode(reinterpret_cast < const unsigned char *>(message_digest), 20);
	server_key += "\r\n";
	strcat(reply, server_key.c_str());
	strcat(reply, "Upgrade: websocket\r\n\r\n");
}

int Websocket_Handler::fetch_http_info()
{
	std::istringstream s(buff_);
	std::string request;

	std::getline(s, request);
	if (request[request.size() - 1] == '\r')
	{
		request.erase(request.end() - 1);
	}
	else
	{
		return -1;
	}

	std::string header;
	std::string::size_type end;

	while (std::getline(s, header) && header != "\r")
	{
		if (header[header.size() - 1] != '\r')
		{
			continue;		//end
		}
		else
		{
			header.erase(header.end() - 1);	//remove last char
		}

		end = header.find(": ", 0);
		if (end != std::string::npos)
		{
			std::string key = header.substr(0, end);
			std::string value = header.substr(end + 2);
			header_map_[key] = value;
		}
	}

	return 0;
}

int Websocket_Handler::websocket_reply(char *buff)
{
	usleep(1000);
	return write(fd_, buff, strlen(buff));
}
