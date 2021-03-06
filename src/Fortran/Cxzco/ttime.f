* Copyright (c) Colorado School of Mines, 2010.
* All rights reserved.

c-------------------------------------------------------------------
      subroutine ttime(d,v,n,time)
c...................................................................
c     Calculates the traveltime along a ray.
c...................................................................
      integer    n
      real       d(n+1),      v(n+1),       time
c...................................................................
cc    local  variables
c     K      loop variable
c...................................................................
      integer    k
c
      time = 0.0
      do 100 k = 1, n + 1
         time  = time  + d(k) / v(k)
100   continue
c
      return
      end
