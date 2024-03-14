#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H
#include"buffer.h"
#include <string>
#include <map>
#include <set>
class HttpRequest{
    public:
        enum PARSE_STATE{
            REQUEST_LINE,
            HEADERS,
            BODY,
            FINISH,
        };
        HttpRequest();
        ~HttpRequest();
        void Init();
        bool parse(Buffer& buffer);

        std::string path() const;
        std::string path();
        std::string method() const;
        std::string version() const;
        std::string GetPost(const std::string & key) const;
        std::string GetPost(const char* key) const;
        bool IsKeepAlive() const;
    private:
        bool _ParseRequestLine(const std::string & line );
        void _ParseHeaders(const std::string & line);
        void _ParseBody(const std::string & line);

        void _ParsePath();
        void _ParsePost();
        void _ParseFromUrlencoded();

        PARSE_STATE _state;
        std::string _method;
        std::string _path;
        std::string _version;
        std::string _body;

        std::map<std::string, std::string> _post;
        std::map<std::string, std::string> _header;

        const static std::set<std::string> _DEFAULT_HTML;
        const static std::map<std::string, int> _DEFAULT_HTML_TAG;
        static int ConvertHex(char ch);

};

#endif // !HTTP_REQUEST_H
