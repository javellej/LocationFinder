#include "tools.h"
#include "error.h"
#include "algo.h"

int random( i_maxDist, i_maxTime, t_point **o_candidates)
{
    int maxTime = 45*60;
//    int maxRadius = maxDistance*(0xffffffff/(float)EARTH_CIRCUMFERENCE);

    /* find several candidates for good locations */
    //srand( time( NULL));
    for ( i=0; i<NUMPOINTS; i++ )
    {
        int r_lat = ( rand() % (2*maxRadius) ) - maxRadius;
        int r_lon = ( rand() % (2*maxRadius) ) - maxRadius;
        end.m_latitude = start.m_latitude + r_lat;
        end.m_longitude = start.m_longitude + r_lon;
        CHECK( getDistance( curl, start, end, &distance_m, &time_s));
#if 0
        printf( "step %d : distance %d m, time %d s\n", i, distance_m, time_s);
#endif
//        if ( distance_m < maxDistance )
        if ( time_s < maxTime )
        {
            candidates[numCandidates++] = end;
        }
    }
}
