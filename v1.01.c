#include <kipr/botball.h>



const int reg = 1; //mainServo
const int neg = 3; //offMainServo

const int neginit = 2047;

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


void balMove(int pos){
    set_servo_position(reg, get_servo_position(reg) + pos);
    set_servo_position(neg, get_servo_position(neg) - pos);
    msleep(1000);
}

int main()
{

    const int deg = 619;

    const int lm = 0;

    const int rm = 3;

    const int bm = 1;

    enable_servos();

    int initialServoPosition = 0;

    //set_servo_position(reg, 0 + initialServoPosition);
    //set_servo_position(neg, 2047 - initialServoPosition);

    balMove(850);

    printf("%i", get_servo_position(reg));

    //moveRel(300);
    msleep(1000);


    int iters = 0;
    while(1){

        /*if(iters % 10000 > 9000 && iters % 10000 < 9500){
            moveRel(5);
            //msleep(200);
        }else if(iters % 10000 > 9000){
            moveRel(-5);
            //msleep(200);
        }*/
        if(iters % (5000) > 4800){
            mav(0, 1000);

        }else{
            mav(0, -1000);
        }


        //printf("%f\n", sin((2*3.14 * iters)/(1500)));
        mav(2, 150 * sin((2*3.14 * iters)/(3000)));
        mav(3, -150 * sin((2*3.14 * iters)/(3000)));

        iters++;
    }
    msleep(30000);

    msleep(5000);

    printf("%i", get_servo_position(2));
    //ig(lm, rm, deg);

    disable_servos();
    msleep(500);
    ao();
    return 0;
}
