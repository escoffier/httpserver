#ifndef _CHANNEL_H_
#define _CHANNEL_H_

class Channel
{
public:
   typedef void(* func)(void *arg ) funtor
   explicit Channel(int fd);
   ~Channel() {}
   inline void SetReadCallback(funtor cb, void* arg) {readcb_ = cb; userdata_ = arg;}
   inline void SetReadCallback(funtor cb, void *arg) {writecb_ = cb;userdata_ = arg;}
   
   inline void HandleRead() { readcb_(userdata_); };
   inline void HandleWrite() { writecb_(userdata_) };
private:
   int                      fd_;    
   funtor                   readcb_;
   funtor                   writecb_;
   void*                    userdata_;
}£»
#endif