// $Id: Adaptive_Lock_Performance_Test_Base.h 91670 2010-09-08 18:02:26Z johnnyw $

#ifndef ACE_ADAPTIVE_LOCK_PERFORMANCE_TEST_BASE_H
#define ACE_ADAPTIVE_LOCK_PERFORMANCE_TEST_BASE_H
#include "Benchmark_Performance.h"

#if defined (ACE_HAS_THREADS)

class ACE_Svc_Export Adaptive_Lock_Performance_Test_Base : public Benchmark_Performance
{
public:
  virtual int init (int, ACE_TCHAR *[]) = 0;
  virtual int fini (void);
  virtual int svc (void);

  int set_lock (ACE_Lock *lock);

private:
  static ACE_Lock *lock_;
};
#endif /* ACE_HAS_THREADS */
#endif /* ACE_ADAPTIVE_LOCK_PERFORMANCE_TEST_BASE_H */
