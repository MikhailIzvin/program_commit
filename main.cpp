#include "Get_info.h"


int main(){

    //list of commits for the master branch, for another branch put another path or create a similar algorithm
    //algorithm
    //список коммитов для ветки мастер, для другой ветки поместите другой путь или создайте подобный алгоритм
    //алгоритм
    const char *path = "https://github.com/curl/curl/commits/master";
    Get_info c(path);
    c.get_file();

    return 0;
}
