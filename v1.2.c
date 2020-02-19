#include <kipr/botball.h>
#include "gyrolib.h"


const int reg = 1; //mainServo
const int neg = 3; //offMainServo

const int neginit = 2047;



const int deg = 619;

const int lm = 2;

const int rm = 3;

const int bm = 1;

const int llf = 2;
const int rlf = 1;

const int blackThreshold = 3000;



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
    set_servo_position(reg, pos);
    set_servo_position(neg, 2047 - pos);
    msleep(1000);
}

int main()
{
    enable_servos();

	
    ig(lm, rm, deg);
    msleep(500);
    balMove(800);
    msleep(500);

    twg(-45);

    dua(850, llf, blackThreshold);
    msleep(500);

    if(analog(rlf) < blackThreshold){
        while(analog(rlf) < blackThreshold){
            mav(rm, 500);  
        }
    }else{
        while(analog(rlf) > blackThreshold){
            mav(rm, -100);  
        }
        freeze(rm);
    }
    msleep(500);

    dwg(500, 500);

    int startSeconds = seconds();

    doubleSquareUp(3, 1);
    
    set_absolute_theta(45);
    printf("abbytheta%d", get_absolute_theta());

    msleep(500);

    twg(10);

    dwg(500, 6200);

    while(get_absolute_theta() > 25){
        mav(lm, 1000);
        mav(rm, -1000);   
    }

    msleep(500);

    int startTime = seconds();
    while(seconds() - startTime < 3){
        mav(lm, 300);
        mav(rm, 300);
    }
    msleep(500);

    set_mode(1);
    twg(45);
    set_mode(0);
    
    msleep(500);
    
    printf("reversedwg\n");
    dwg(-800, 3500);
    printf("findwg\n");
    
    printf("beginning fouble square up \n");
    doubleSquareUp(2, 2);
	msleep(500);
    set_absolute_theta(0);
    twg(-180);
    dwg(-800, 2000);
    
    doubleSquareUp(5, 1);
    msleep(500);
    
    awg(-600, -300, 75);
    
    set_mode(1);
    twg(90);
    set_mode(0);
    
    dwg(750, 3);
   
    
    /*dwg(850, 3250);
    
    twg(70);
    
    while(analog(llf) < blackThreshold)
    {
        mav(rm,1000);
        mav(lm,1000);
    }
    twg(15);*/

    
    
    disable_servos();
    msleep(500);
    ao();
    return 0;
}
