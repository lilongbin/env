#ifndef __WEBSOCKET_REQUEST__
#define __WEBSOCKET_REQUEST__

#include <stdint.h>
#include <arpa/inet.h>

class Websocket_Request {
public:
	Websocket_Request();
	~Websocket_Request();
	int parse_websocket_info(char *msg);
	int fill_response_websocket_msg(char *msg);
	int set_fin(uint8_t fin_);
	int set_opcode(uint8_t opcode_);
	int set_mask(uint8_t mask_);
	int set_masking_key(uint8_t masking_key_[]);
	int set_payload_length(uint64_t payload_length_);
	int set_payload(char *payload_);
	uint8_t get_fin();
	uint8_t get_opcode();
	uint8_t get_mask();
	uint8_t *get_masking_key();
	uint64_t get_payload_length();
	char* get_payload();
	void print();
	void reset();

private:
	int fetch_fin(char *msg, int &pos);
	int fetch_opcode(char *msg, int &pos);
	int fetch_mask(char *msg, int &pos);
	int fetch_masking_key(char *msg, int &pos);
	int fetch_payload_length(char *msg, int &pos);
	int fetch_payload(char *msg, int &pos);

	int fill_fin(char *msg, int &pos, uint8_t fin_);
	int fill_opcode(char *msg, int &pos, uint8_t opcode_);
	int fill_mask(char *msg, int &pos, uint8_t mask_);
	int fill_masking_key(char *msg, int &pos, uint8_t masking_key_[]);
	int fill_payload_length(char *msg, int &pos, uint64_t payload_length_);
	int fill_payload(char *msg, int &pos, uint8_t mask, char *payload, uint64_t size);
private:
	uint8_t fin_;
	uint8_t opcode_;
	uint8_t mask_;
	uint8_t masking_key_[4];
	uint64_t payload_length_;
	char payload_[2048];
};

#endif
