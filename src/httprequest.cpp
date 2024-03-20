#include "httprequest.h"
#include"log.h"

// auto HttpRequest::_DEFAULT_HTML{
//             "/index", "/register", "/login",
//              "/welcome", "/video", "/picture", };

// auto HttpRequest::_DEFAULT_HTML_TAG {
//             {"/register.html", 0}, {"/login.html", 1},  };

// HttpRequest::HttpRequest():_state{REQUEST_LINE}
// {

    
// }

// bool HttpRequest::IsKeepAlive() const{
//     if(_header.count("Connection") == 1){
//         return _header.find("Connection")->second == "keep-alive" && _version == "1.1";
//     }
//     return false;
// }
// bool HttpRequest::parse(Buffer& buffer)
// {
//     const char CRLF[] = "\r\n";
//     while(buffer.ReadableBytes() &&_state != FINISH){
//         const char* lineEnd = search(buffer.Peek(), buffer.BeginWriteConst(), CRLF, CRLF+2);
//         std::string line(buffer.Peek(), lineEnd);
//         switch(_state){
//             //有限状态机，从请求行开始，每处理完后会自动转入到下一个状态  
//             case REQUEST_LINE:
//                 if(!_ParseRequestLine(line)){
//                     return false;
//                 }
//                 _ParsePath();
//                 break;
//             case HEADERS:
//                 _ParseHeader(line);
//                 if(buffer.ReadableBytes() <= 2){
//                     _state = FINISH;
//                 }
//                 break;
//             case BODY:
//                 _ParseBody(line);
//                 break;
//             default:
//                 break;
//         }
//         if(lineEnd == buffer.BeginWrite()){  // 读完了
//             break;
//         }else{
//             buffer.RetrieveUntil(lineEnd+2);   // 跳过回车换行
//         }
//     }
//     LOG_DEBUG("[%s], [%s], [%s]", _method.c_str(), _path.c_str(), _version.c_str());
//     return true;
// }

// void HttpRequest::_ParsePath{
//     if(_path == "/"){
//         _path = "/index.html";
//     }else{
//         for(auto &item:_DEFAULT_HTML){
//             if(_path == item){
//                 _path += ".html";
//                 break;
//             }
//         }
//     }
// }


// bool HttpRequest::ParseRequestLine(const char *begin)
// {
//     return false;
// }

