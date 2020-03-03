#include <kipr/botball.h>
#include "gyrolib.h"


const int reg = 1; //mainServo
const int neg = 3; //offMainServo

const int neginit = 2047 - 50;
const int reginit = 0;



const int deg = 654;

const int lm = 2;

const int rm = 3;

const int bm = 1;

const int llf = 2;
const int rlf = 1;

const int blackThreshold = 3000;
const int blackOnBrownThreshold = 2300;



void moveRel(int pos){

    int initPosReg = get_servo_position(reg);
    int initPosNeg = get_servo_position(neg);

    printf("init pos: %i,%i\n", initPosReg, initPosNeg);

    int mult = 1;

    if(pos < 0){
        mult = -1;
        pos = -pos;
    }

    int chunkSize = 50;

    int steps = pos / chunkSize;

    double remainder = steps % chunkSize;

    int i;
    for(i = 0; i < steps; i++){

        set_servo_position(reg, get_servo_position(reg) + (chunkSize * mult));
        printf("Setting reg pos to %i\n", get_servo_position(reg) + (chunkSize * mult) );

        set_servo_position(neg, get_servo_position(neg) - (chunkSize * mult));
        printf("Setting neg pos to %i\n", get_servo_position(neg) - (chunkSize * mult));

        msleep(30);
    }
    msleep(200);


    set_servo_position(reg, initPosReg + (pos*mult));

    printf("Setting final reg pos %i\n", initPosReg + (pos*mult));

    set_servo_position(neg, initPosNeg - (pos * mult));
    printf("Setting final neg pos %i\n", initPosNeg - (pos * mult));




}



void doubleSquareUp(int time, int flip){


    printf("entered sq up \n");

    int startSeconds = seconds();


    while(seconds() - startSeconds < time){

        int lmfinished = 0;

        int rmfinished = 0;
        if(analog(rlf) > blackThreshold){
            mav(rm, 200 * flip);   
        }else{
            //lmfinished = 1;
            mav(rm, -200 * flip);
        }
        if(analog(llf) > blackThreshold){
            mav(lm, 200 * flip);   
        }else{

            mav(lm, -200 * flip); 
            //rmfinished = 1; 
        }

        //stop = lmfinished && rmfinished;

    }

    printf("exited squp\n");
    freeze(lm);
    freeze(rm);
    msleep(500);   
}


void balMove(int pos){
    set_servo_position(reg, reginit + pos);
    set_servo_position(neg, neginit - pos);
    msleep(1000);
}


double osc1(int x){
    int period = 1000;
    int mult = (((x / period) % 2) == 0) ? 1 : 1;
    return mult * cos((2 * M_PI * x)/(period));
}
double osc2(int x){
    int period = 1000;
    int mult = (((x / period) % 2) == 0) ? 1 : -1;
    return mult * sin((2 * M_PI * (x + M_PI))/(period));
}

void hardAlign(int et1,int et2, int tolerance)
{
    //int et1start = analog(et1);
    int et2start = analog(et2);
    while(analog(et2) > et2start + tolerance && analog(et2) < et2start - tolerance)
    {
            dwg(500, 500);
    }
}

int main()
{
    enable_servos();

    ig(lm, rm, deg);
    msleep(500);
    int initBalMoveVal = 1600;
    balMove(initBalMoveVal);
    msleep(500);
    
     /* int itersf = 0;
    int startfSeconds = seconds();
    int last = 0;
    while(seconds() - startfSeconds < 20){
     	mav(2, -1400);
        mav(3, -1400);
        
        if(itersf%10 == 0){
         	printf("tps:%i\n", (last - ((gmpc(2) + gmpc(3))/2)) / 10);   
            last = (gmpc(2) + gmpc(3))/2;
        }
        
        itersf++;
        msleep(10);
    }
    
    return 0;*/

    twg(-45);

    dua(1400, llf, blackThreshold);


    /*if(analog(rlf) < blackThreshold){
        while(analog(rlf) < blackThreshold){
            mav(rm, 500);  
        }
    }else{
        while(analog(rlf) > blackThreshold){
            mav(rm, -100);  
        }
        freeze(rm);
    }*/

    doubleSquareUp(1, -1);
    dwg(500, 500);

    int startSeconds = seconds();

    doubleSquareUp(1.5, 1);

    set_absolute_theta(45);
    msleep(50);
    printf("abbytheta%d\n", get_absolute_theta());

    msleep(50);

    twg(13);

    dwg(1000, 3100);

    printf("\n%f\n", get_absolute_theta());

    /* int abc = 0;

    while(1){
     	if(abc % 3000 == 0){
    		printf("\n%f\n", get_absolute_theta());
        }

    }

    return 0;*/

    while(get_absolute_theta() > 5){
        mav(lm, 1500);
        mav(rm, -1500);   
    }
    freeze(lm);
    freeze(rm);
    printf("\nabsThetaAfterCornerPipe:%f\n", get_absolute_theta());
    msleep(250);

    int startTime = seconds();
    while(seconds() - startTime < 1.5){
        mav(lm, 600);
        mav(rm, 600);
    }
    msleep(500);

    set_mode(1);
    twg(45);
    set_mode(0);

    msleep(500);

    printf("reversedwg\n");
    dwg(-1350, 2000);
    printf("findwg\n");

    printf("beginning fouble square up \n");
    doubleSquareUp(2, 2);
    msleep(500);
    set_absolute_theta(0);
    msleep(250);

    msleep(50);

    twg(-170);

    msleep(250);
    dwg(-800, 2000);

    doubleSquareUp(5, 1);
    msleep(500);
    set_absolute_theta(0);
    msleep(250);

    dwg(-1000, 1200);
    msleep(250);

    twg(70);
    msleep(500);
    dwg(-750, 1500);
    while(get_absolute_theta() < 90){
        mav(rm, 300);   
        mav(lm, -150);
    }
    freeze(rm);
    freeze(lm);
    msleep(250);

    dwg(-1200, 2000);

    if(analog(llf) < blackOnBrownThreshold){
        while(analog(llf) < blackOnBrownThreshold){
            mav(lm, 300);
        }
        freeze(lm);
    }

    freeze(lm);

    printf("\n\n\n\nAbsolute Theta: %f", get_absolute_theta());
    set_mode(1);
    msleep(50);
    
    twg(91);
    set_mode(0);
    freeze(lm);
    freeze(rm);
    printf("\n\n\n\nAbsolute Theta: %f", get_absolute_theta());

    //the worst idea ever
    /*dwg(-1000, 1000);
    msleep(50);
    twg(-90);

    mav(lm, -800);
    mav(rm, -800);
    msleep(2000);
    twg(90);*/

    int startDuaSeconds = seconds();

    int duaLength = 8000;

    if(duaa(-1200, llf, blackThreshold, 12, duaLength)){
        printf("\n\n*****************************DUA BROKEN****************************\n");

        int timeElapsed = seconds() - startDuaSeconds;
            mav(lm, 500);
            msleep(250);
        if(analog(llf) > blackThreshold){
            while(analog(llf) > blackThreshold){ //be more tolerant of black
                mav(lm, 500);
            }
            mav(lm, 0);
            msleep(50);
        }

        printf("\nDWG for remaining time %f\n", (duaLength) - (timeElapsed*1000));
        dwg(-1200, (duaLength) - (timeElapsed*1000));
        printf("Now outside of compensating dwg\n");
    }

    freeze(lm);
    freeze(rm);

    msleep(500);

    mav(lm, -800);
    mav(rm, -800);
    msleep(3000);

    dwg(500, 500);

    twg(-90);

    mav(lm, -800);
    mav(rm, -800);
    msleep(1500);

    dwg(1000, 2000);

    msleep(50);



    int moveRelDelay = seconds();


    //linefollow for two seconds
    while(seconds() - moveRelDelay < 2){
        if(analog(rlf) < blackThreshold){
            mav(rm, 800);
            mav(lm, 200);
        }else{
            mav(lm, 800);
            mav(rm, 200);

        } 
    }

    mav(lm, 0);
    mav(rm, 0);

    //stop lf to moverel
    moveRel(-1 * (initBalMoveVal - 600));


    //continue until rangefinder
    while(analog(0) < 2200){


        if(analog(rlf) < blackThreshold){
            mav(rm, 800);
            mav(lm, 200);
        }else{
            mav(lm, 800);
            mav(rm, 200);

        }


    }
    freeze(lm);
    freeze(rm);
    msleep(250);
    dwg(800, 500);
    msleep(50);

    moveRel(-get_servo_position(reg));
    msleep(750);
    balMove(0);
    msleep(250);

    set_absolute_theta(0);

    set_mode(1);

    int iters = 0;
    startTime = seconds();
    printf("%i", seconds);
    while(seconds() - startTime < 8){

        /*if(iters % 10000 > 9000 && iters % 10000 < 9500){
            moveRel(5);
            //msleep(200);
        }else if(iters % 10000 > 9000){
            moveRel(-5);
            //msleep(200);
        }*/
        if(iters % (5000) > 4800){
            mav(0, 1100);

        }else{
            mav(0, -1100);
        }


        //printf("%f\n", sin((2*3.14 * iters)/(1500)));
        mav(2, 170 * osc1(iters));
        mav(3, -170 * osc1(iters));

        iters++;
    }

    twg(0);

    mav(0, 0);

    set_mode(0);
    msleep(250);

    moveRel(500);
    msleep(500);
    dwg(-850, 2000);

    /* if(analog(rlf) > blackThreshold){
     	while(analog(rlf) > blackThreshold){
         	mav(rm, -200);   
        }
    }else{
     	 while(analog(rlf) < blackThreshold){
         	mav(lm, -200);   
        }  
    }*/


    if(drive_until_analog_advanced(-1400, llf, blackThreshold, 12, 2250)){ //returns 1 if hit black
        printf("broke at line \n");

        while(analog(llf) > blackThreshold){
            mav(lm, 400);   
        }
        mav(lm, 0);
    }


    moveRel(-200);
    mav(lm, -800);
    mav(rm, -800);
    msleep(2000);
    moveRel(200);
    moveRel(-200);
    moveRel(600);
    mav(lm, -800);
    mav(rm, -800);
    msleep(3000);

    dwg(1000, 500);
    msleep(50);
    twg(90);

    dwg(-500, 500);


    while(analog(rlf) > 200){ //arbitrary threshold for white board. Brown seemed to always exceed 200
        if(analog(rlf) < blackThreshold){
            mav(rm, 800);
            mav(lm, 0);
        }else{
            mav(lm, 800);
            mav(rm, 0);
        }
    }

    freeze(lm);
    freeze(rm);

    mav(lm, 800);
    mav(rm, 800);
    msleep(2000);

    twg(-90);

    twg(30);

    dwg(800, 1000);

    twg(-30);

    dwg(800, 2000);

    dua(-1000, llf, blackThreshold);
    dwg(-200, 700);

    doubleSquareUp(3.5, -1);

    twg(15);

    dwg(-400, 500);

    moveRel(-(get_servo_position(reg) - 750));
    mav(0, 1000);
    msleep(5000);
    mav(0, -1000);
    msleep(2000);
    mav(0, 1000);
    msleep(3500);
    
    return 0;

    int beginBackLf = seconds();
    while(seconds() - beginBackLf < 4){
        if(analog(llf) < blackThreshold){
            mav(rm, -1400);
            mav(lm, -100);
        }else{
            mav(lm, -1400);
            mav(rm, -100);

        }

    }

    int beginBackHardalign = seconds();
    while(seconds() - beginBackHardalign < 3){
        mav(rm, -1400);
        mav(lm, -1400);
    }




    disable_servos();
    msleep(500);
    ao();
    return 0;
}
