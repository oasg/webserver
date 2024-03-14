#include "httprequest.h"

auto HttpRequest::_DEFAULT_HTML{
            "/index", "/register", "/login",
             "/welcome", "/video", "/picture", };

auto HttpRequest::_DEFAULT_HTML_TAG {
            {"/register.html", 0}, {"/login.html", 1},  };

HttpRequest::HttpRequest():_state{REQUEST_LINE}
{

    
}

bool HttpRequest::IsKeepAlive() const{
    
}
bool HttpRequest::ParseRequestLine(const char *begin)
{
    return false;
}
