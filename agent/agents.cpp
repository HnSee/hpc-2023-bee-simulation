#include <iostream>
#include <cmath>
#include <vector>

#include "basic.cpp"
#include "var.cpp"

int scout = 0;
int worker = 0;

// place where the hive stores the food
class hive_bee_access{
    public:
        // how much food the hive has
        int foodstore;

        // random access foodsources (maybe change that into kde tree)
        std::vector< position<double> > foodsources;

        // all the bees with no tasks
        int activebees;
        
        void addfood(int x){
            foodstore = foodstore + x;
        }

        // adds foodsource
        void add_fs(position<double> p){
            for(int k = foodsources.size()-1; k > 0; k--){
                if( foodsources.at(k).x == p.x && foodsources.at(k).y == p.y ){
                    return;
                }
            }
            foodsources.push_back( p );    
        }

        // removes foodsource
        void rem_fs(position<double> p){
            for(int k = 0; k < foodsources.size(); k++){
                if( foodsources.at(k).x == p.x && foodsources.at(k).y == p.y ){
                    foodsources.at(k) = foodsources.at(foodsources.size()-1);  
                    foodsources.pop_back();
                    return;
                }
            }
        }

        // get random foodsource
        position<double> rand_fs(){
            if(foodsources.size() == 0){
                throw std::invalid_argument( "must be at least one argument" );
            }
            
            int r = std::rand()%foodsources.size();

            return foodsources.at(r);

        }
};

class bee: public agent{
    public:
        // hiveptr
        hive_bee_access* hstore;

        // wether the bee is flying to the destination or the hive
        bool searching;

        // whether or not the bee is a scout
        bool worker;

        // food the bee is carrying
        int food;
        
        // position of the hive
        position<double> hivepos;
        
        // position send to
        position<double> destination;

        void init(position<double> pos, position<double> hive, position<double> destination, bool searching, bool worker, hive_bee_access* hstore){
            this->pos = pos;
            this->hivepos = hivepos;
            this->destination = destination;
            this->searching = searching;
            this->worker = worker;
            this->hstore = hstore;
            hstore->foodstore = 0;
        }
        
        void move(){
            if( worker ){
                if( searching ){
                    pos = getmovementvector(pos, destination);
                    if( pos.x == destination.x && pos.y == destination.y){
                        food = 1;
                        searching = false;
                    }
                }
                else{
                    pos = getmovementvector(pos, hivepos);

                    if( pos.x == hivepos.x && pos.y == hivepos.y ){
                        hstore->addfood(food);
                        nuke();
                    }
                    
                }
            }
            else{
                if( searching ){
                    pos = getmovementvector(pos, destination);
                    // check if food is near
                    // if food is near, store position in destination,
                    // if scout reaches destination and doesnt find anything then the scout either generates next location or returns
                    if( pos.x == destination.x && pos.y == destination.y ){
                        if( scoutindurance > std::rand()%101 ){
                            destination.x += (std::rand()%2000) - 1000;
                            destination.y += (std::rand()%2000) - 1000;
                        }
                        else{
                            searching = false;
                        }
                    }
                }
                else{
                    pos = getmovementvector(pos, hivepos);
                    if( pos.x == hivepos.x && pos.y == hivepos.y ){
                        hstore->add_fs(destination);
                        nuke();
                    }
                }
            }
        }

        void update(){
            return;
        }

        std::string gettype(){
            std::string ret = "bee";
            return ret;
        }

        void nuke(){
            //std::cout << "removed bee\n";
            hstore->activebees -= 1;
            removeagent = true;
            delete this;
        }
};

class hive: public agent{
    public:
        // place where the food is stored
        hive_bee_access* hstore;    

        // access to the datastructure to spawn bees into  
        std::vector<agent*> *ds;
        
        // what time of the day it is
        int tickoftheday;
        
        // all the bees belonging to the hive
        int totalbees;

        void init(int totalbees, std::vector<agent*> *ds, hive_bee_access* hstore){
            tickoftheday = 0;
            this->totalbees = totalbees;
            this->ds = ds;
            this->hstore = hstore;
            this->hstore->activebees = 0;
            this->hstore->foodstore = 0;
        }
        
        void move(){
            tickoftheday += 1;
            return;
        }

        int getsize(){
            return ds->size();
        }

        void update(){
            double x = (double)tickoftheday/daylength;
            int release = totalbees * (-3 * (x-0.5) * (x-0.5) + 1);

            for(int k = 0; release > hstore->activebees; k++){
                //release bees
                // calc the percentage the bee is a scout
                int beespersource = 100;
                double scoutpercentage = ( (double) totalbees - hstore->foodsources.size() * beespersource ) / totalbees;
                double t = (double)( std::rand() %10000 ) /10000;

                if( ( totalbees - hstore->foodsources.size() > 0 && t < scoutpercentage ) || hstore->foodsources.size() == 0){
                    //std::cout << "Scout: " << scoutpercentage << "  " << t << "\n";
                    scout += 1;
                    bee* b = new bee;
                    b->init(position<double>{pos.x,pos.y}, position<double>{pos.x,pos.y}, position<double>{pos.x,pos.y}, true, false, hstore);
                    ds->push_back(b);
                }
                else{
                    //std::cout << "Worker: " << scoutpercentage << "  " << t << "\n";
                    worker += 1;
                    bee* b = new bee;
                    b->init(position<double>{pos.x,pos.y}, position<double>{pos.x,pos.y}, hstore->rand_fs(), true, true, hstore);
                    ds->push_back(b);

                }

                hstore->activebees += 1;
            }
            return;
        }

        std::string gettype(){
            std::string ret = "hive";
            return ret;
        }

        void nuke(){
            removeagent = true;
            delete hstore;
            delete this;
        }
};

class flower: public agent{
    public:
        // how long a flower produces food closer to one -> longer blooming ( 1= blooming forever )
        double bloomlength;

        // how much a flower produces at maximum -> larger = more production
        double maxproduction;

        // day of the max production
        int peak;
        
        // regenerates with ((maxproduction)/(bloomlength^(abs(x-peak))))
        double size;


        void init(double bloomlength, double maxproduction, int peak){
            this->bloomlength = bloomlength;
            this->maxproduction = maxproduction;
            this->peak = peak;
            this->size = 0;
        }

        void move(){
            return;
        }

        void update(){
            int storesize = size;
            size += maxproduction / ( pow(bloomlength, abs(day-peak)) );

            //std::cout << day << "\n";
            //std::cout << (int) (maxproduction / ( pow(bloomlength, abs(day-peak))) ) << "\n";

            if( (int) (maxproduction / ( pow(bloomlength, abs(day-peak))) ) == 0 && day > peak){
                nuke();
                return;
            }
            
        }

        std::string gettype(){
            std::string ret = "flower";
            return ret;
        }

        void nuke(){
            //std::cout << "removed flower\n";
            removeagent = true;
            delete this;
        }

};

// removes agent
bool check_agent_removal(int j){
    if( removeagent == true){
        agents[j] = agents[agents.size()-1];
        agents.pop_back();
        removeagent = false;
        return true;
    }
    return false;
}

// one move
void tick(int j){
    agents[j]->move();

    if (check_agent_removal(j)){ return; }

    agents[j]->update();

    if (check_agent_removal(j)){ return; }
}

// remove selfpointer
int main(){
    std::srand(time(NULL));
    
    hive* h = new hive;
    
    h->init(40000, &agents, new hive_bee_access);

    agents.push_back(h);

    for(int x = 0; x < 5; x++){
        for(int k = 0; k < daylength; k++){
           auto t = agents.size();
           h->tickoftheday = 0;
            for(int j = 0; j < t ; j++ ){
                tick(j);
                t = agents.size();
            }
            // baum aufgebaut werden
        }
    }

    std::cout << "Scout: " << scout << "\n";
    std::cout << "Worker: " << worker << "\n";
}