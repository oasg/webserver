#include "buffer.h"
#include <sys/uio.h>
#include <assert.h>
#include <unistd.h> 
#include <strings.h>
Buffer::Buffer(int bufsize):_buffer(bufsize), _rpos(0), _wpos(0)
{

}

size_t Buffer::WritableBytes() const
{
    return _buffer.size() - _wpos;
}

size_t Buffer::ReadableBytes() const
{
    return _wpos - _rpos;
}

size_t Buffer::PrependableBytes() const
{
    return _rpos;
}

void Buffer::Append(const std::string &str)
{
    Append(str.c_str(), str.size());
}

void Buffer::Append(const char *str, size_t len)
{
    assert(str);
    EnsureWriteable(len);
    std::copy(str,str+len,BeginWrite());
    HasWritten(len);
}

void Buffer::Append(const void *data, size_t len)
{
    Append(static_cast<const char*>(data), len);
}

void Buffer::Append(const Buffer &buff)
{
    Append(buff.Peek(), buff.ReadableBytes());
}

ssize_t Buffer::ReadFd(int fd, int *saveErrno)
{
    char buff[65535];
    struct iovec iov[2];
    const size_t writable = WritableBytes();
    iov[0].iov_base = BeginPtr() + _wpos;
    iov[0].iov_len = writable;
    iov[1].iov_base = buff;
    iov[1].iov_len = sizeof(buff);
    //ensure to read all data
    //if write buffer is full, wite into the temp buff
    const ssize_t len = readv(fd, iov, 2);
    if(len < 0){
        *saveErrno = errno;
    }else if(len<=writable){
        _wpos+=len;
    }else{   //buffer not enough
        _wpos = _buffer.size();
        Append(buff, len - writable); 
    }
    return len;
}

ssize_t Buffer::WriteFd(int fd, int *saveErrno)
{
    size_t readSize = ReadableBytes();
    ssize_t len = write(fd, Peek(), readSize);
    if(len<0){
        *saveErrno = errno;
        return len;
    }
    _rpos += len;
    return len;
}

char *Buffer::BeginPtr()
{
    return &_buffer[0];
}

const char *Buffer::BeginPtr() const
{
    return &_buffer[0];
}

void Buffer::MakeSpace(int len)
{
    if(WritableBytes() + PrependableBytes() < len){  //no enough reserved space
        _buffer.resize(_wpos + len+1);  //grow the buffer
    }else{
        size_t readable = ReadableBytes();
        std::copy(BeginPtr()+_rpos, BeginPtr()+_wpos, BeginPtr());  
        _rpos = 0;
        _wpos = readable;
    }
}

const char *Buffer::Peek() const
{
    return BeginPtr() + _rpos;
}

void Buffer::EnsureWriteable(size_t len)
{
    if(WritableBytes()<len){
        MakeSpace(len);
    }
    assert(WritableBytes()>=len);
}

void Buffer::HasWritten(size_t len)
{
    _wpos+=len;
}

void Buffer::Retrieve(size_t len)
{
    assert(len<=ReadableBytes());
    _rpos+=len;
}

void Buffer::RetrieveUntil(const char *end)
{
    assert(Peek()<=end);
    Retrieve(end-Peek());
}


void Buffer::RetrieveAll()
{
    _buffer.clear(); //?
    _rpos = 0;
    _wpos = 0;
}

std::string Buffer::RetrieveAllToStr()
{
    std::string str(Peek(),ReadableBytes());
    RetrieveAll();
    return str;
}

char *Buffer::BeginWrite()
{
    return BeginPtr() + _wpos;
}

