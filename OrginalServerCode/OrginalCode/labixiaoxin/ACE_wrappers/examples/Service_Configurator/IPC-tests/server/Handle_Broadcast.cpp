// $Id: Handle_Broadcast.cpp 91670 2010-09-08 18:02:26Z johnnyw $

#include "Handle_Broadcast.h"

#if defined (SunOS4)
extern "C"
{
  int init (void);
  int fini (void);
  void __sti__Handle_Broadcast_C_init_();
  void __std__Handle_Broadcast_C_init_();
}

int
init (void)
{
  __sti__Handle_Broadcast_C_init_();
  return 0;
}

int
fini (void)
{
  __std__Handle_Broadcast_C_init_();
  return 0;
}
#endif /* SunOS4 */

unsigned short Handle_Broadcast::DEFAULT_PORT = ACE_DEFAULT_BROADCAST_PORT;

#if !defined (__ACE_INLINE__)
#include "Handle_Broadcast.inl"
#endif /* __ACE_INLINE__ */

Handle_Broadcast remote_broadcast;
ACE_Service_Object_Type rb (&remote_broadcast, ACE_TEXT("Remote_Brdcast"));
