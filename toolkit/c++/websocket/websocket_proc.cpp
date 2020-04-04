#include <stdio.h>
#include <string.h>
#include "websocket_proc.h"

Websocket_Request::Websocket_Request():
		fin_(),
		opcode_(),
		mask_(),
		masking_key_(),
		payload_length_(),
		payload_()
{
}

Websocket_Request::~Websocket_Request(){

}

int Websocket_Request::parse_websocket_info(char *msg){
	int pos = 0;
	fetch_fin(msg, pos);
	fetch_opcode(msg, pos);
	fetch_mask(msg, pos);
	fetch_payload_length(msg, pos);
	fetch_masking_key(msg, pos);
	return fetch_payload(msg, pos);
}

int Websocket_Request::fill_response_websocket_msg(char *msg)
{
	int pos = 0;
	printf("\nReponse:");
	print();
	fill_fin(msg, pos, fin_);
	fill_opcode(msg, pos, opcode_);
	fill_mask(msg, pos, mask_);
	fill_payload_length(msg, pos, payload_length_);
	fill_masking_key(msg, pos, masking_key_);
	return fill_payload(msg, pos, mask_, payload_, payload_length_);
}

int Websocket_Request::set_fin(uint8_t fin_)
{
	this->fin_ = fin_;
}
int Websocket_Request::set_opcode(uint8_t opcode_)
{
	this->opcode_ = opcode_;
}
int Websocket_Request::set_mask(uint8_t mask_)
{
	this->mask_ = mask_;
}
int Websocket_Request::set_masking_key(uint8_t masking_key_[])
{
	int i = 0;
	for (i=0; i<4; i++)
	{
		this->masking_key_[i] = masking_key_[i];
	}
}
int Websocket_Request::set_payload_length(uint64_t payload_length_)
{
	this->payload_length_ = payload_length_;
}
int Websocket_Request::set_payload(char *payload_)
{
	strncpy(this->payload_, payload_, payload_length_);
}
uint8_t Websocket_Request::get_fin()
{
	return fin_;
}
uint8_t Websocket_Request::get_opcode()
{
	return opcode_;
}
uint8_t Websocket_Request::get_mask()
{
	return mask_;
}
uint8_t * Websocket_Request::get_masking_key()
{
	return (uint8_t *)&masking_key_;
}
uint64_t Websocket_Request::get_payload_length()
{
	return payload_length_;
}
char* Websocket_Request::get_payload()
{
	return payload_;
}

void Websocket_Request::print(){
	printf("\nWEBSOCKET PROTOCOL\n FIN: %d\n OPCODE: %d\n MASK: %d\n PAYLOADLEN: %lu\n PAYLOAD: %s\n",
				fin_, opcode_, mask_, payload_length_, payload_);
	// reset();
}

void Websocket_Request::reset(){
	fin_ = 0;
	opcode_ = 0;
	mask_ = 0;
	memset(masking_key_, 0, sizeof(masking_key_));
	payload_length_ = 0;
	memset(payload_, 0, sizeof(payload_));
}

int Websocket_Request::fetch_fin(char *msg, int &pos){
	fin_ = (unsigned char)msg[pos] >> 7;
	return 0;
}

int Websocket_Request::fetch_opcode(char *msg, int &pos){
	opcode_ = msg[pos] & 0x0f;
	pos++;
	return 0;
}

int Websocket_Request::fetch_mask(char *msg, int &pos){
	mask_ = (unsigned char)msg[pos] >> 7;
	return 0;
}

int Websocket_Request::fetch_masking_key(char *msg, int &pos){
	if(mask_ != 1) return 0;
	for(int i = 0; i < 4; i++)
		masking_key_[i] = msg[pos + i];
	pos += 4;
	return 0;
}

int Websocket_Request::fetch_payload_length(char *msg, int &pos){
	payload_length_ = msg[pos] & 0x7f;
	pos++;
	if(payload_length_ == 126){
		uint16_t length = 0;
		memcpy(&length, msg + pos, 2);
		pos += 2;
		payload_length_ = ntohs(length);
	}
	else if(payload_length_ == 127){
		uint32_t length = 0;
		memcpy(&length, msg + pos, 4);
		pos += 4;
		payload_length_ = ntohl(length);
	}
	return 0;
}

int Websocket_Request::fetch_payload(char *msg, int &pos){
	memset(payload_, 0, sizeof(payload_));
	if(mask_ != 1){
		memcpy(payload_, msg + pos, payload_length_);
	}
	else {
		for(uint i = 0; i < payload_length_; i++){
			int j = i % 4;
			payload_[i] = msg[pos + i] ^ masking_key_[j];
		}
	}
	pos += payload_length_;
	return 0;
}

int Websocket_Request::fill_fin(char *msg, int &pos, uint8_t fin_)
{
	msg[pos] ^= (0x1 << 7);
	msg[pos] |= (fin_ << 7);
}

int Websocket_Request::fill_opcode(char *msg, int &pos, uint8_t opcode_)
{
	msg[pos] &= 0xF0;
	msg[pos] |= opcode_;
	pos++;
}
int Websocket_Request::fill_mask(char *msg, int &pos, uint8_t mask_)
{
	msg[pos] &= (0xFF >> 1);
	msg[pos] |= (mask_ << 7);
}
int Websocket_Request::fill_masking_key(char *msg, int &pos, uint8_t masking_key_[])
{
	for(int i = 0; i < 4; i++)
		msg[pos + i] = masking_key_[i];
	pos += 4;
	return true;
}
int Websocket_Request::fill_payload_length(char *msg, int &pos, uint64_t payload_length_)
{
	if (payload_length_ < 126)
	{
		msg[pos] &= 0x1 << 7; //清零
		msg[pos] |= payload_length_;
		pos++;
	}
	else if (payload_length_ >= 0xFFFF)
	{
		msg[pos] ^= 127;
		msg[pos] |= 127;
		pos++;
		*(uint64_t *)(msg + pos) = payload_length_;
		pos += 4;
	}
	else if (payload_length_ == 127) {
		msg[pos] ^= 127;
		msg[pos] |= 126;
		pos++;
		*(short *)(msg + pos) = payload_length_;
		pos += 2;
	}
	return 0;
}
int Websocket_Request::fill_payload(char *msg, int &pos, uint8_t mask_, char *payload, uint64_t size)
{
	if (mask_ != 1)
	{
		memcpy(msg + pos, payload, size);
	}
	else
   	{
		for (uint i = 0; i < size; i++){
			int j = i % 4;
			msg[pos + i] =  payload[i] ^ masking_key_[j];
		}
	}
	pos += size;
	return 0;

}

