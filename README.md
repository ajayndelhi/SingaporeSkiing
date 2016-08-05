# SingaporeSkiing
Find best Skiing route
Date: Thursday, 4th August 2016

Problem Definition:

- http://geeks.redmart.com/2015/01/07/skiing-in-singapore-a-coding-diversion/

Code Details:

- Language: C++ (Visual Studio 2012/2015)
- Main Entry point: SingaporeSkiing\SingaporeSkiing.cpp
- Dll Project: SingaporeSkiingDll
- Test Project: SingaporeSkiingTest

Sample Output: 
- ProgramOutput.png

Test Data: 
- SmallTestData.txt 
- LargeTestData.txt

Challenges Solved:
- Performance
- Memory usage

Optimizations Done:
- We are processing a skiing route in the main loop (SingaporeSkiingDll\SkiResort.cpp\FindBestRoute), only if the route can't be reached from its sorrounding points.
Because if the poing is reachable from its sibling points, then the current point can not be the longest path.

- Since a skiing point can be reached from multiple sibling points, we cache the longest path length from a skiinng point on first iteration.  Subsequently, if we visit
same skiing point again, we first check the longest path from that point.  We go down that path again to find best route, only if it make sense (that we will get better path from what we have in hand so far).

- The code was initially written by making use of STL clases like Vector and then tested with List as well.  Realizing the slow performance, I reverted back to arrays.

Limitations:
- To achieve efficiency, better memory usage - I have assumed that the max path length will be 100.  If the program crosses this limit, it throws exception in ProcessElevationPoint method.
The limit can be increased easily by changing Max_Ski_Path_Size in SkiResort.h

Pending Enhancements:

- Better error handing
- some more encapsulation

*** End of Document ***



