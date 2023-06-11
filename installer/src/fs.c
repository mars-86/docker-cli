#include <curl/curl.h>
#include "../inc/fs.h"

#define FS_URL "https://dl-cdn.alpinelinux.org/alpine/v3.17/releases/x86_64/alpine-minirootfs-3.17.1-x86_64.tar.gz"

struct memory {
    char *response;
    size_t size;
};
 
static size_t cb(void *data, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct memory *mem = (struct memory *)userp;

    char *ptr = realloc(mem->response, mem->size + realsize + 1);
    if(ptr == NULL)
        return 0;  /* out of memory! */

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;
 
    return realsize;
}

struct memory chunk = {0};

void get_fs(void)
{
    char full_path[512];
    const char *file_name = "alpine-minirootfs-3.17.1-x86_64.tar.gz";
    sprintf(full_path, "%s\\%s", getenv("TMP"), file_name);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL *curl = curl_easy_init();
    if (curl) {
        CURLcode res;
        char errbuff[CURL_ERROR_SIZE];
        curl_easy_setopt(curl, CURLOPT_URL, FS_URL);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuff);
#ifdef __DEBUG
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
#endif
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        
        if (res == 0) {
            FILE *f = fopen(full_path, "wb");
            int i = 0;
#ifdef __DEBUG
            printf("Saving filesystem in %s...\n", full_path);
#endif
            while (i < chunk.size)
                fputc(chunk.response[i++], f);
            fclose(f);
        } else {
            printf("%s\n", errbuff);
        }
        free(chunk.response);
    }
    curl_global_cleanup();
}
