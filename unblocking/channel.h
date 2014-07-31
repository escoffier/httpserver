#ifndef _CHANNEL_H_
#define _CHANNEL_H_

typedef void(*functor)(void* );
class Channel
{
public:
   explicit Channel(int fd);
   ~Channel() {}
   inline void SetReadCallback(functor cb, void* arg) {readcb_ = cb; userdata_ = arg;}
   inline void SetWriteCallback(functor cb, void *arg) {writecb_ = cb;userdata_ = arg;}
   
   void HandleRead();
   inline void HandleWrite() { writecb_(userdata_); };
private:
   int                      fd_;    
   functor                  readcb_;
   functor                  writecb_;
   void*                    userdata_;
};
#endif
