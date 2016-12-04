#include <time.h>
#include "Random.h"
#include "OwnAEUtils.h"
#include "Point.h"
#include <math.h>

Point::Point(int x, int y, int radius, int action, bool stage){
    this->x = x;
    this->y = y;
    this->radius = radius;
    this->action = action;
    this->stage = stage;
}

Point::~Point(){}

int Point::getX() {
    return x;
}

void Point::setX(int x) {
    this->x = x;
}

int Point::getY() {
    return y;
}

void Point::setY(int y) {
    this->y = y;
}

int Point::getRadius() {
    return radius;
}

void Point::setRadius(int radius) {
    this->radius = radius;
}

int Point::getAction() {
    return action;
}

void Point::setAction(int action) {
    this->action = action;
}

bool Point::getStage() {
    return stage;
}

void Point::setStage(bool stage) {
    this->stage = stage;
}

void Point::mutate(Random* rng) {

	int val = rng->nextInt(6);

    if(val < 3){

        float rnd = rng->nextFloat();

        x += round(26.0*rnd*COEF);
        if(x < 0){
            x = 0;
        }
        else if(x > 255){
            x = 255;
        }
        rnd = rng->nextFloat();
        y += round(24.0*rnd*COEF);
        if(y < 0){
            y = 0;
        }
        else if(y > 224){
            y=224;
        }
        rnd = rng->nextFloat();
        radius += round(13.0*rnd*COEF);
        if(radius < 0){
            radius = 0;
        }
        else if(radius > 255){
            radius = 255;
        }
    }
    else if(val < 5){
        int rnd = rng->nextInt(2);
        if(action == 2){
            if(rnd == 0)
                action = 64; //Left
            else
                action = 66; //LeftRight
        }
        else if(action == 64){
            if(rnd == 0)
                action = 2; //Right
            else
                action = 66; //LeftRight
        }
        else{
            if(rnd == 0)
                action = 2; //Right
            else
                action = 64; //Left
        }
    }
    else{
        stage = !stage;
    }
}

std::ostream& Point::write(std::ostream& os){
    os << (int)x
       << " " << (int)y
       << " " << (int)radius
       << " " << (int)action
       << " " << (int)stage;
    return os;
}

std::ofstream& Point::write(std::ofstream& os){
    os << (int)x
       << " " << (int)y
       << " " << (int)radius
       << " " << (int)action
       << " " << (int)stage;
    return os;
}

int Point::serialize(char* buff){
    return sprintf(buff, "%d %d %d %d %d ", x, y, radius, action, stage);
}

Point* Point::clone() {
    return new Point(x, y, radius, action, stage);
}
