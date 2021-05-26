//
// Created by Sveto Copy on 23.05.2021.
//

#ifndef CURL_GET_INFO_H
#define CURL_GET_INFO_H

#include <curl/curl.h>
#include <string>
#include <iostream>
#include <vector>


class Get_info {
private:
std::string readBuffer;
std::vector<int> data_mass;
std::vector<int> commiter_mass;
std::vector<int> message_mass;
std::vector<int> exception_commiter_mass;
std::vector<int> exception_message_mass;
public:
    explicit Get_info(const char *path);
    void get_file();
};


#endif //CURL_GET_INFO_H
