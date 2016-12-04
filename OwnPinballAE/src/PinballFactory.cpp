#include "PinballFactory.h"
#include <math.h>
#include "Point.h"
#include "AIParameter.h"

PinballFactory::PinballFactory(){}

PinballFactory::~PinballFactory(){}

Solution* PinballFactory::generateRandomCandidate(Random* rng){
        std::vector<Genome*>* sol = new std::vector<Genome*>();
        int x, y;
        int boundX, boundXlow, boundXhigh;
        bool stage;
        for(int i=0; i<20;i++){
            //3 puntos arriba izquierda
            if(i < 3){
                stage = false;
                y = 157 + rng->nextInt(56);
                if(y < 191){
                    boundX = ceil(sqrt(1156 - pow(y-191, 2)));
                    x = 110 + ((boundX>0)? rng->nextInt(boundX + 1) : 0);
                }
                else{
                    boundXlow = ceil(4/3*(y-108.5));
                    boundXhigh = 110+ceil(sqrt(1156 - pow(y-191, 2)));
                    x = boundXlow + ((boundXlow<boundXhigh)? rng->nextInt(boundXhigh - boundXlow + 1) : 0);
                }
            }
            //3 puntos arriba derecha
            else if(i < 6){
                stage = false;
                y = 157 + rng->nextInt(56);
                if(y < 191){
                    boundXlow = ceil(176 - sqrt(1156 - pow(y-191, 2)));
                    x = boundXlow + ((boundXlow<176)? rng->nextInt(176 - boundXlow + 1) : 0);
                }
                else{
                    boundXlow = ceil(176 - sqrt(1156 - pow(y-191, 2)));
                    boundXhigh = ceil((2951-9*y)/7);
                    x = boundXlow + ((boundXlow<boundXhigh)? rng->nextInt(boundXhigh - boundXlow + 1) : 0);
                }
            }
            //3 puntos abajo izquierda
            else if(i < 9){
                stage = true;
                y = 153 + rng->nextInt(53);
                if(y < 186){
                    boundX = ceil(sqrt(1089 - pow(y-186, 2)));
                    x = 112 + ((boundX>0)? rng->nextInt(boundX + 1) : 0);
                }
                else{
                    boundXlow = ceil((27*y-2894)/19);
                    boundXhigh = 112 + ceil(sqrt(1089 - pow(y-186, 2)));
                    x = boundXlow + ((boundXlow<boundXhigh)? rng->nextInt(boundXhigh - boundXlow + 1) : 0);
                }
            }
            //3 puntos abajo derecha
            else if(i < 12){
                stage = true;
                y = 154 + rng->nextInt(52);
                if(y < 186){
                    boundXlow = 175 - ceil(sqrt(1024 - pow(y-186, 2)));
                    x = boundXlow + ((boundXlow<175)? rng->nextInt(175 - boundXlow + 1) : 0);
                }
                else{
                    boundXlow = 175 - ceil(sqrt(1024 - pow(y-186, 2)));
                    boundXhigh = ceil((8161-26*y)/19);
                    x = boundXlow + ((boundXlow<boundXhigh)? rng->nextInt(boundXhigh - boundXlow + 1) : 0);
                }
            }
            else{
                x = 70 + rng->nextInt(160);
                y = 170 + rng->nextInt(55);
                stage = rng->nextInt(2);
            }
            int act = 0;
            switch(rng->nextInt(3)){
                case 0:
                        act = 64;
                        break;
                case 1:
                        act = 2;
                        break;
                case 2:
                        act = 66;
                        break;
            }

            sol->push_back(new Point(x, //X
                                    y, //Y
                                    10 + rng->nextInt(11), //Radius
                                    act, //Action
                                    stage)); //Stage
        }
        for(int i = 20; i < 23; i++){
            sol->push_back(new AIParameter(rng->nextInt(150)));
        }

        return new Solution(sol);
}
