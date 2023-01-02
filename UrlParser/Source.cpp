#include <iostream>
#include <regex>
#include <string>
#include <vector>

// Структура за съхранение на частите на URL
struct URL {
    std::string protocol;
    std::string hostname;
    std::string resource;
    std::string username;
    std::string password;
};

void split(std::string str, std::string splitBy, std::vector<std::string>& tokens)
{
    /* Store the original string in the array, so we can loop the rest
     * of the algorithm. */
    tokens.push_back(str);

    // Store the split index in a 'size_t' (unsigned integer) type.
    size_t splitAt;
    // Store the size of what we're splicing out.
    size_t splitLen = splitBy.size();
    // Create a string for temporarily storing the fragment we're processing.
    std::string frag;
    // Loop infinitely - break is internal.
    while (true)
    {
        /* Store the last string in the vector, which is the only logical
         * candidate for processing. */
        frag = tokens.back();
        /* The index where the split is. */
        splitAt = frag.find(splitBy);
        // If we didn't find a new split point...
        if (splitAt == std::string::npos)
        {
            // Break the loop and (implicitly) return.
            break;
        }
        /* Put everything from the left side of the split where the string
         * being processed used to be. */
        tokens.back() = frag.substr(0, splitAt);
        /* Push everything from the right side of the split to the next empty
         * index in the vector. */
        tokens.push_back(frag.substr(splitAt + splitLen, frag.size() - (splitAt + splitLen)));
    }
}

int isItIp(std::string str) {
    std::vector<std::string> results;
    split(str, ".", results);

    bool inValid = false;
    bool outOfRange = false;

    if (results.size() < 4 || results.size() > 4)
        inValid = true;

    if(!inValid)
        for (auto& i : results) {
            std::string prefix = i.substr(0, i.find(":"));
            bool isInt = std::regex_match(prefix, std::regex(("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?")));
            if (!isInt) {
                inValid = true;
                break;
            } else {
                int num = stoi(i);
                if (num < 0 || num > 255)
                    outOfRange = true;
            }
        }

    if (inValid)
        return 0;

    if (outOfRange)
        return 2;

    return 1;
}

// Парсване на URL и връщане на неговите части
URL parse_url(const std::string& url) {
    URL result;

    // Намиране на протокол
    size_t protocol_end = url.find("://");
    if (protocol_end != std::string::npos) {
        result.protocol = url.substr(0, protocol_end);
        // check me if protocol is not valid
    } else {
        // Не е специфициран протокол, задаваме HTTP
        result.protocol = "http";
    }

    // Определяне на хостнейм и пътя на ресурса
    size_t hostname_end = url.find('/', protocol_end + 3);
    if (hostname_end != std::string::npos) {
        result.hostname = url.substr(protocol_end + 3, hostname_end - (protocol_end + 3));
        result.resource = url.substr(hostname_end);
        if (isItIp(result.hostname) == 2) {
            std::cout << "ERROR: THE HOSTNAME IP IS OUT OF RANGE\n";
        }
    } else {
        result.hostname = url.substr(protocol_end + 3);
        if (isItIp(result.hostname) == 2) {
            std::cout << "ERROR: THE HOSTNAME IP IS OUT OF RANGE\n";
        }
        result.resource = "/";
    }



    // Валидиране на хостнейма
    if (result.protocol != "ftp") {
        std::regex ip_regex("^(\\d{1,3}\\.){3}\\d{1,3}$");
        std::regex hostname_regex("^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\\-]*[A-Za-z0-9])[:80]*$");
        if (!std::regex_match(result.hostname, hostname_regex)) {
            try {
                throw std::invalid_argument("[ERROR NAME] Invalid hostname");
            } catch (const std::exception& e) {
                std::cout << e.what() << "\n";
            }
        }
    }

    // Валидиране и декодиране на пътя на ресурса
    std::string resource_path;
    for (size_t i = 0; i < result.resource.size(); i++) {
        char c = result.resource[i];
        if (c == '%') {
            if (i + 2 >= result.resource.size()) {
                try {
                    throw std::invalid_argument("[ERROR SIZE]Invalid URL encoding");
                } catch (const std::exception& e) {
                    std::cout << e.what() << "\n";
                }
                return result;
            }

            char hex_str[3] = { result.resource[i + 1], result.resource[i + 2], '\0' };
            char* endptr;
            long value = strtol(hex_str, &endptr, 16);
            if (*endptr != '\0') {
                try {
                    throw std::invalid_argument("[ERROR END] Invalid URL encoding");
                }
                catch (const std::exception& e) {
                    std::cout << e.what() << "\n";
                }
            }
            resource_path += static_cast<char>(value);
            i += 2;
        }
        else {
            resource_path += c;
        }
    }
    result.resource = resource_path;

    // Екстрактване на данните за вход за FTP ако са предоставени
    if (result.protocol == "ftp") {
        size_t username_start = result.hostname.find('@');
        if (username_start != std::string::npos) {
            result.username = result.hostname.substr(0, username_start);
            result.hostname = result.hostname.substr(username_start + 1);

            size_t password_start = result.username.find(':');
            if (password_start != std::string::npos) {
                result.password = result.username.substr(password_start + 1);
                result.username = result.username.substr(0, password_start);
            } else {
                try {
                    throw std::invalid_argument("[ERROR FINDING] :");
                } catch (const std::exception& e) {
                    std::cout << e.what() << "\n";
                }
            }
        }
        else {
            try {
                throw std::invalid_argument("[ERROR FINDING] @");
            } catch (const std::exception& e) {
                std::cout << e.what() << "\n";
            }
        }
    }

    return result;
}

int main() {
    std::string wrongIP = "http://127.1234.0.1:80/path%2d%2d%2d%2d%2d.html";

    std::vector < std::string > vec1{
         "https://www.example.com/path/to/resource",
         "htp://do-ma-in.com/path%20%2d%3c%2f%4b.html",
         "http://do-ma-in.com/path%20%2d%3c%2f%4b.html",
         "http://do-ma-in.com/path%0%2d%3c%2f%4b.html",
         "http://do-main.com:80/path%2d%2d%2d%2d%2d.html",
         "http://127.30.0.1/path%2d%2d%2d%2d%2d.html",
         "http://ascii-code.com",
         "ftp://user1!a:pass1w!ord@121.0.0.1/path",
         "ftp://user1!apass1w!ord@121.0.0.1/path",
         "ftp://user1!:apass1w!ord121.0.0.1/path",
         "ftp://user:password@ftp.example.com/path/to/resource",
         "https://www.example.com/path/to/resource",
         "http://127.1234.0.1:80/path%2d%2d%2d%2d%2d.html"
    };

    URL parsed_url;

    int counter = 1;
    for (auto& i : vec1) {
        parsed_url = parse_url(i);

        std::cout << "URL: (" << counter << ") | " << i << std::endl;
        std::cout << "Protocol: " << parsed_url.protocol << std::endl;

        if(parsed_url.username != "")
            std::cout << "Username: " << parsed_url.username << std::endl;

        if (parsed_url.password != "")
            std::cout << "Password: " << parsed_url.password << std::endl;
        std::cout << "Hostname: " << parsed_url.hostname << std::endl;
        std::cout << "Resource: " << parsed_url.resource << std::endl << std::endl;

        counter++;
    }

    //checkIpOnly(wrongIP);

    return 0;
}