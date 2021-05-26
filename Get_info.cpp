//
// Created by Sveto Copy on 23.05.2021.
//

#include "Get_info.h"

//использование namespace обусловлено тем, чтобы из других файлов эти функции не были видны (и другого способа я не знаю)
//the use of namespace is due to the fact that these functions are not visible from other files (and I don’t know any other way)
namespace {
    //перевод с вывода на консоль в строку.
    //translation from console output to string.
    size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp){
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }


    //поиск нужного контента по ключевому слову
    //search for the desired content by keyword
    void item_number(std::string &sentence, std::vector<int> &c, std::string &word){
        int i = 0;
        for (i = sentence.find(word, i++); i != std::string::npos; i = sentence.find(word, i + 1)){
            c.push_back(i);
        }
    }

    //получение объекта по ключевому слову
    //getting an object by keyword
    void info(std::vector<int> &c, std::string &readBuffer,char symbol_first, char symbol_end, int i){
        bool result = false;
            for (int j = c[i]; readBuffer[j] != symbol_end; j++) {
               if (result)
                    std::cout << readBuffer[j];
               if (readBuffer[j] == symbol_first)
                   result = true;
               else
                   continue;
            }
    }

    //проверка на тему кем было загружено сообщение (кто автор, а кто коммитер)
    //check on the topic by whom the message was uploaded (who is the author and who is the committer)
    void check(std::vector<int> &exception_mass, std::vector<int> &sentence, std::string &readBuffer, std::string message, int &i){
        for (int j = 0; j < exception_mass.size(); j++){
            if ((sentence[i] < exception_mass[j]) && (exception_mass[j] < sentence[i+1])) {
                std::cout << message;
                info(sentence, readBuffer, '>', '<', i+1);
                sentence.erase(sentence.begin() + i);
            }
        }
    }

    //проверка на указатель на какой либо коммит
    //checking for a pointer to some commit
    void check_message(std::vector<int> &sentence, std::vector<int> &data, std::vector<int> &exception_message_mass, std::string &readBuffer, int &i){
        int k = 0;
        if ((sentence[i] < exception_message_mass[k]) && (exception_message_mass[k] < data[i])) {
            std::cout << " >";
            info(exception_message_mass, readBuffer, '>', '/', k);
            std::cout << " ";
            exception_message_mass.erase(exception_message_mass.begin() + k);
        }
        if ((data[i] > sentence[i]) && (data[i] > sentence[i+1])) {
            info(sentence, readBuffer, '>', '<', i + 1);
            sentence.erase(sentence.begin() + i);
        }
    }
}

//получение информации с сайта при помощи libcurl
//retrieving information from a site using libcurl
Get_info::Get_info(const char *path) {
    CURL *curl;
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, path);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        CURLcode result = curl_easy_perform(curl);
        if (result != CURLE_OK){
            std::cerr << "download problem: " << curl_easy_strerror(result) << std::endl;
        }
        curl_easy_cleanup(curl);
    }
}

//задание ключевых слов, символов для вывода на консоль
//setting keywords, characters for output to the console
void Get_info::get_file() {
    std::string word_date =  "datetime=";
    std::string word_commiter = "commit-author user-mention";
    std::string word_message = "markdown-title";
    std::string word_exception_commiter = " authored and ";
    std::string word_exception_message = "<tt>";
    item_number(readBuffer, exception_message_mass, word_exception_message);
    item_number(readBuffer, exception_commiter_mass, word_exception_commiter);
    item_number(readBuffer, data_mass, word_date);
    item_number(readBuffer, commiter_mass, word_commiter);
    item_number(readBuffer, message_mass, word_message);
    for (int i = 0; i < data_mass.size(); i++) {
        //",>,Z,< -- sort symbols to look pretty
        std::cout << "date_time: ";
        info(data_mass, readBuffer, '"', 'Z', i);
        std::cout << '\n';
        std::cout << "message: ";
        info(message_mass, readBuffer, '>', '<', i);
        check_message(message_mass, data_mass, exception_message_mass, readBuffer, i);
        std::cout << "\n";
        info(commiter_mass, readBuffer, '>', '<', i);
        check(exception_commiter_mass, commiter_mass,readBuffer, word_exception_commiter, i);
        std::cout << " commit";

        std::cout << "\n\n";
    }
}
