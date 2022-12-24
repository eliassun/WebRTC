
#ifndef CAPTURE_H
#define CAPTURE_H
#include <string>
std::string encode_image(const std::string& path);
void decode_image(const std::string& input);
std::string capture_screen();
void open_image();
#endif