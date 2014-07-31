#include "channel.h"

Channel::Channel(int fd)
{
    fd_ = fd;

}

void Channel::HandleRead() 
{ 
    readcb_(userdata_); 
}
