#ifndef _CHANNEL_H_
#define _CHANNEL_H_

class Channel
{
public:
   typedef void(* functor)(void *);
   explicit Channel(int fd);
   ~Channel() {}
   inline void SetReadCallback(functor cb, void* arg) {readcb_ = cb; userdata_ = arg;}
   inline void SetWriteCallback(functor cb, void *arg) {writecb_ = cb;userdata_ = arg;}
   
   inline void HandleRead() { readcb_(userdata_); };
   inline void HandleWrite() { writecb_(userdata_); };
private:
   int                      fd_;    
   functor                  readcb_;
   functor                  writecb_;
   void*                    userdata_;
};
#endif
