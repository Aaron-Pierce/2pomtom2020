#include <kipr/botball.h>
#include <pthread.h>


thread accelThread;


void watchCollision(){
    int iters = 0;
    int gracePeriod = 3000;
    double average = 0;
    double last = 0;
    while(1){
        
        if(iters < gracePeriod){
            average += (last - accel_y());
        }
        
        iters++;
    }
}


void startWatch(){
    accelThread = thread_create(watchCollision);
    thread_start(accelThread);
}