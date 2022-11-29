#include "movement.h"

uint32_t xTimeBeforeWhile=0;

DendoStepper *Xs;
DendoStepper *Ys;
DendoStepper *Zs;

enum{X,Y,Z};

static pos_t pos;

bool homed[3] = {false,false,false};
bool moving = false;
bool disabled = true;

static void Xint(void*p){
    if(moving)
        disabled = true;
}

static void Yint(void*p){
    if(moving)
        disabled = true;
}

static void Zint(void*p){
    if(moving)
        disabled = true;
}

void initMov(DendoStepper *Xc, DendoStepper *Yc, DendoStepper *Zc){
    Xs = Xc;
    Ys = Yc;
    Zs = Zc;

    Xs->init(X_STEP, X_DIR, X_EN, TIMER_GROUP_0, TIMER_0, MICROSTEP_32, 200);
    Ys->init(Y_STEP, Y_DIR, Y_EN, TIMER_GROUP_0, TIMER_1, MICROSTEP_32, 200);
    Zs->init(Z_STEP, Z_DIR, Y_EN, TIMER_GROUP_1, TIMER_0, MICROSTEP_32, 200);

    Xs->setSpeed(500, 1000);
    Ys->setSpeed(500, 1000);
    Zs->setSpeed(500, 1000);

    gpio_config_t sleep_c = {
        .pin_bit_mask = 1ULL << GPIO_NUM_2,
        .mode = GPIO_MODE_OUTPUT,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&sleep_c);
    gpio_set_level(GPIO_NUM_2,1);

    gpio_config_t end_c = {
        .pin_bit_mask = 1ULL << X_ENDSW_PIN | 1ULL << Y_ENDSW_PIN | 1ULL << Z_ENDSW_PIN,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .intr_type = GPIO_INTR_NEGEDGE
    };
    gpio_config(&end_c);
    
    //gpio_isr_handler_add(X_ENDSW_PIN,Xint,NULL);
    //gpio_isr_handler_add(Y_ENDSW_PIN,Yint,NULL);
    //gpio_isr_handler_add(Z_ENDSW_PIN,Zint,NULL);
}
static void homeZ(){
    //1st stop
     //1st stop
    Zs->stop();
    vTaskDelay(1);
    //start moving towards endsw
    Zs->setSpeed(1000, 1000);
    Zs->runPos(INT32_MIN);

    WHILE_BLOCK(gpio_get_level(Z_ENDSW_PIN),30000L);
    Zs->stop();

    WHILE_BLOCK(Zs->getState()>IDLE,100);
    //move a bit in
    Zs->runPos(100);

    WHILE_BLOCK(!gpio_get_level(Z_ENDSW_PIN),20000L);
    Zs->stop();

    WHILE_BLOCK(Zs->getState()>IDLE,100);
    //move slowly back
    Zs->setSpeed(5000, 1000);
    Zs->runPos(-10000);

    WHILE_BLOCK(gpio_get_level(Z_ENDSW_PIN),20000L);
    Zs->stop();

    WHILE_BLOCK(Zs->getState()>IDLE,100);

    //done
    Zs->setSpeed(3000, 1000);
    pos.Z=0;

}

static void homeY(){
    //1st stop
    Ys->stop();
    vTaskDelay(1);
    //start moving towards endsw
    Ys->setSpeed(10000, 2000);
    Ys->runPos(INT32_MAX);

    WHILE_BLOCK(gpio_get_level(Y_ENDSW_PIN),60000L);
    Ys->stop();

    WHILE_BLOCK(Ys->getState()>IDLE,100);
    //move a bit in
    Ys->runPos(-500);

    WHILE_BLOCK(!gpio_get_level(Y_ENDSW_PIN),20000L);
    Ys->stop();

    WHILE_BLOCK(Ys->getState()>IDLE,100);
    //move slowly back
    Ys->setSpeed(1000, 2000);
    Ys->runPos(10000);

    WHILE_BLOCK(gpio_get_level(Y_ENDSW_PIN),20000L);
    Ys->stop();

    WHILE_BLOCK(Ys->getState()>IDLE,100);

    //done
    Ys->setSpeed(10000, 1000);
    pos.Y=0;

}

static void homeX(){
     //1st stop
    Xs->stop();
    vTaskDelay(1);
    //start moving towards endsw
    Xs->setSpeed(10000, 1000);
    Xs->runPos(INT32_MIN);

    WHILE_BLOCK(gpio_get_level(X_ENDSW_PIN),60000L);
    Xs->stop();

    WHILE_BLOCK(Xs->getState()>IDLE,100);
    //move a bit in
    Xs->runPos(100);

    WHILE_BLOCK(!gpio_get_level(X_ENDSW_PIN),20000L);
    Xs->stop();

    WHILE_BLOCK(Xs->getState()>IDLE,100);
    //move slowly back
    Xs->setSpeed(5000, 1000);
    Xs->runPos(-10000);

    WHILE_BLOCK(gpio_get_level(X_ENDSW_PIN),20000L);
    Xs->stop();

    WHILE_BLOCK(Xs->getState()>IDLE,100);

    //done
    Xs->setSpeed(10000, 1000);
    pos.X=0;


}

int moveXY(int X, int Y){
    if(pos.Z != 0){
        homeZ();
    }
    moving = true;
    Xs->runPos(X*X_STEPS_MM);
    WHILE_BLOCK(Xs->getState()>IDLE,20000);
    Ys->runPos(-Y*Y_STEPS_MM);
    WHILE_BLOCK(Ys->getState()>IDLE,20000);

    pos.X = X;
    pos.Y = Y;
    moving = false;
    return 1;
}

int moveZ(int Z){
    moving = true;
    Zs->runPos(Z*Z_STEPS_MM);
    WHILE_BLOCK(Zs->getState()>IDLE,20000);
    moving = false;
    return 0;
}

void getXYZ(int *X, int *Y, int *Z){
    *X = pos.X;
    *Y = pos.Y;
    if(pos.Z > 0)
        *Z = 1;
    else
        *Z = 0;
}

void home(void*p){
    //1st stop
    gpio_uninstall_isr_service();
    Xs->stop();
    Ys->stop();
    Zs->stop();
    vTaskDelay(1);
    homeZ();
    homeY();
    homeX();
    gpio_install_isr_service(0);
    disabled = false;
}