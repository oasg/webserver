#ifndef BUFFER_H
#define BUFFER_H
#include<vector>
#include<atomic>
#include<string>

struct iovec
{
    void* iov_base;
    int iov_len;
};

class Buffer{
    public:
        Buffer(int bufsize = 1024);
        ~Buffer() = default;
        size_t WritableBytes() const; //bytes available to write
        size_t ReadableBytes() const;
        size_t PrependableBytes() const;
        //find the start of the readable space
        const char* Peek() const;
        void EnsureWriteable(size_t len);
        void HasWritten(size_t len);
        void Retrieve(size_t len);
        void RetrieveUntil(const char* end);
        void RetrieveAll();
        std::string RetrieveAllToStr();
        char* BeginWrite();
        const char* BeginWriteConst() const;


        void Append(const std::string& str);
        void Append(const char* str, size_t len);
        void Append(const void* data, size_t len);
        void Append(const Buffer& buff);

        ssize_t ReadFd(int fd, int* saveErrno);
        ssize_t WriteFd(int fd, int* saveErrno);

        void EnsureWriteableLength(int len);
        
    private:
        char* BeginPtr();
        const char* BeginPtr() const;
        void MakeSpace(int len);
    private:
        std::vector<char> _buffer;
        std::atomic<std::size_t> _rpos;
        std::atomic<std::size_t> _wpos;
};
#endif