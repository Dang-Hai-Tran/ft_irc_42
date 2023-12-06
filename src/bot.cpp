#include "irc.hpp"
// Make sur to install libcurl4-openssl-dev
#include <curl/curl.h>
#include <iostream>
#include <string>

static std::string sendPromptToOpenAI(const std::string &prompt);

// Implementation a bot chat connect to OpenAI endpoint to reply to user
// Api endpoint : https://api.openai.com/v1/chat/completions
void commandBot(Server *server, Client *client, std::string input) {
    std::string message = cmdFindMessage(input);
    std::vector<std::string> args = cmdFindArgs(input);
    if (message.empty() || args.size() != 1) {
        server->sendData(client, "(!) Usage: /BOT :<message>\r\n");
        return;
    }
    std::string res = sendPromptToOpenAI(message) + "\r\n";
    server->sendData(client, res);
}

std::string extractMessage(std::string response) {
    std::string key = "\"content\": \"";
    size_t start = response.find(key);
    if (start == std::string::npos) {
        return "";
    }
    start += key.length();
    size_t end = response.find("\"\n", start);
    if (end == std::string::npos) {
        return "";
    }
    std::string ret = response.substr(start, end - start);
    std::string remove = "";
    for (size_t i = 0; i < ret.length(); i++) {
        if (ret[i] == '\n') {
            continue;
        }
        remove += ret[i];
    }
    return remove;
}

size_t WriteCallback(char *contents, size_t size, size_t nmemb, std::string *response) {
    size_t totalSize = size * nmemb;
    response->append(contents, totalSize);
    return totalSize;
}

// Function to send a prompt to the OpenAI endpoint
std::string sendPromptToOpenAI(const std::string &prompt) {
    CURL *curl;
    CURLcode res;
    std::string ret;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    const char *apiKey = std::getenv("OPENAI_API_KEY");
    if (apiKey == NULL) {
        std::cout << "OPENAI_API_KEY is not set" << std::endl;
        curl_easy_cleanup(curl);
        return ret;
    }

    if (curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        std::string auth_header = "Authorization: Bearer " + std::string(apiKey);
        headers = curl_slist_append(headers, auth_header.c_str());

        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        std::string data = std::string("{\n") +
                           "    \"model\": \"gpt-3.5-turbo\",\n" +
                           "    \"messages\": [\n" +
                           "        {\n" +
                           "            \"role\": \"system\",\n" +
                           "            \"content\": \"You're a helpful assistant. Please keep your response under 50 words.\"\n" +
                           "        },\n" +
                           "        {\n" +
                           "            \"role\": \"user\",\n" +
                           "            \"content\": \"" + prompt + "\"\n" +
                           "        }\n" +
                           "    ]\n" +
                           "}";

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            ret = extractMessage(response);
            std::cout << "Response: " << ret << std::endl;
        }
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return ret;
}
