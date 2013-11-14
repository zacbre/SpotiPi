#include "main.h"

int port = 5555;
char *prt;
int main(int argc , char *argv[])
{
    //cout << getSpotifySong() <<endl;
    //exit(1);
    if(argc != 2)
    {
        cout << "Usage: " << argv[0] << " <spotilocal-port (usually 4381)>" <<endl;
        exit(1);
    }
    prt = argv[1];
    int sock, clisock;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in spotipi_addr, cli_addr;
    cout << "Starting up sockets listener..."<<endl;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
        error("Error while opening socket!");
    memset(&spotipi_addr, 0, sizeof(spotipi_addr));
    spotipi_addr.sin_family = AF_INET;
    spotipi_addr.sin_port = htons(port);
    spotipi_addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(sock, (struct sockaddr *)&spotipi_addr, sizeof(spotipi_addr)) < 0)
        error("Could not bind to port 5555 on localhost.");
    else
        cout <<"Successfully bound to port 5555."<<endl;
    listen(sock, 100);
    clilen = sizeof(cli_addr);
    while(true)
    {
        clisock = accept(sock, (struct sockaddr *)&cli_addr, &clilen);
        if(clisock < 0)
            error("Could not accept client.");
        memset(buffer, 0, 256);
        int total = read(clisock, buffer, 255);
        if(total < 0) error("Could not read from client.");
        char *msg = (char*)malloc(1024);
        char *spot = getSpotifySong();
        cout << spot <<endl;
        memset(msg, 0, 1024);
        strcat(msg, "HTTP/1.1 200\r\nContent-Length: ");
        sprintf(msg, "%s%i", msg, strlen(spot));
        strcat(msg, "\r\nContent-Type: text/html\r\n\r\n");
        strcat(msg, spot);
        total = write(clisock, msg, strlen(msg));
        free(msg);
        free(spot);
        if(total < 0) error("Could not write to socket.");
        close(clisock);
    }
    return 0;
}

void error(char *msg)
{
    perror(msg);
    exit(1);
}

char *getSpotifySong()
{
    //first, GET our CSRF token.
    string h = GET("https://embed.spotify.com/openplay/?uri=spotify:track:5Zp4SWOpbuOdnsxLqwgutt");
    //look for tokenData;
    char *x = (char*)h.c_str();//str_replace(h.c_str(), " ", "");
    //get our tokendata.
    int tokenstart = str_indexof(x, "tokenData = '");
    if(tokenstart != -1)
    {
        //get end of token
        int tokenend = IndexOf(x, "'", tokenstart + 1);
        if(tokenend != -1)
        {
            //get token.
            char *token = substr_s(x, tokenstart + 1, tokenend - tokenstart - 1);
            //cout << token <<endl;
            //now we have our token. let's use it to get our CFID.
            char url[512];
            memset(url, 0, 512);
            strcat(url, "http://localhost:");
            strcat(url, prt);
            strcat(url, "/simplecsrf/token.json");
            //cout << url <<endl;
            string v = GET(url);
            //get token.
            char *cs = (char*)v.c_str();
            int startcsrf = str_indexof(cs, "token\": \"");
            int endcsrf = IndexOf(cs, "\"", startcsrf + 1);
            char *csrf = substr_s(cs, startcsrf + 1, endcsrf - startcsrf - 1);
            //cout << csrf <<endl;
            //start a request to status.
            char buff[512];
            memset(buff, 0, 512);
            strcat(buff, "http://localhost:");
            strcat(buff, prt);
            strcat(buff, "/remote/status.json?&ref=&cors=");
            strcat(buff, "&oauth=");
            strcat(buff, token);
            strcat(buff, "&csrf=");
            strcat(buff, csrf);
            //cout << buff <<endl;
            string status = GET(buff);
            //cout << status <<endl;
            //get track and artist name.
            char *p = (char*)status.c_str();
            //locate "name"
            int starttrack = str_indexof(p, "name\": \"", 0);
            int endtrack = IndexOf(p, "\"", starttrack + 1);
            char *track = substr_s(p, starttrack + 1, endtrack - 1 - starttrack);
            //cout << track <<endl;
            int startartist = str_indexof(p, "name\": \"", starttrack);
            int endartist = IndexOf(p, "\"", startartist + 1);
            char *artist = substr_s(p, startartist + 1, endartist - 1 - startartist);
            //cout << artist <<endl;
            free(token);
            free(csrf);
            //take our shit and return it.
            char *output = (char*)malloc(strlen(track) + strlen(artist) + strlen("\n"));
            bzero(output, strlen(track) + strlen(artist) + strlen("\n"));
            strcat(output, artist);
            strcat(output, "\n");
            strcat(output, track);
            free(track);
            free(artist);
            return output;
        }
    }
}

string outputData;
/* the function to invoke as the data recieved */
size_t static write_callback_func(void *buffer,
                        size_t size,
                        size_t nmemb,
                        void *userp)
{
    outputData = string((char*)buffer);
    return size*nmemb;
}
string GET(char *url)
{
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *chunk = NULL;
        chunk = curl_slist_append(chunk, "Origin: https://embed.spotify.com");
        chunk = curl_slist_append(chunk, "Referer: https://embed.spotify.com/?uri=spotify:track:5Zp4SWOpbuOdnsxLqwgutt");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback_func);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
        {
            cout << res <<endl;
            error("Error, Make sure Spotify is Running.");
        }
        curl_easy_cleanup(curl);
    }
    if(outputData.length() > 0)
        return outputData;
    else
        return NULL;
}
