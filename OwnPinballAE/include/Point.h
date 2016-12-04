#ifndef POINT_H
#define POINT_H

#include <Genome.h>

class Point : public Genome
{
    public:
        Point(int x, int y, int radius, int action, bool stage);
        virtual ~Point();
        int getX();
        void setX(int x);
        int getY();
        void setY(int y);
        int getRadius();
        void setRadius(int radius);
        int getAction();
        void setAction(int action);
        bool getStage();
        void setStage(bool stage);
        void mutate(Random* rng);

        virtual std::ostream& write(std::ostream& os);
        virtual std::ofstream& write(std::ofstream& os);
        int serialize(char* buff);
        Point* clone();

    private:
        int x;
        int y;
        int radius;
        int action;
        bool stage;
};

#endif // POINT_H
