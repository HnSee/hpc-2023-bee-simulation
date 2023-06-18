/* 
In this file are the basic structures and classes stored, everything depends on
*/

template<typename T>
struct position{
    T x;
    T y;
};

// function for comparison based sorting std::sort
struct sortby_xy {
    bool operator()(const position<double> a, const position<double> b) const {
        if( a.x == b.x){
            return a.y < b.y;
        }
        else{
            return a.x < b.x;
        }
    }
};

// agent class, that is the base class for all other classes
class agent{
    public:
        // int x, y; -> if grid based, also int x,y

        // coordinates
        position<double> pos;
        
        // initialization of the Agent, function has to initialize all necessary variables
        virtual void init(){
            std::cout << "This is an empty declaration! \n";
            return;
        };

        // movement of the agent
        virtual void move(){
            std::cout << "This is an empty declaration! \n";
            return;
        };

        // update of agent after move
        virtual void update(){
            std::cout << "This is an empty declaration! \n";
            return;
        };

        virtual void specialinteraction(){
            std::cout << "This is an empty declaration! \n";
            return;
        };

        // returns int of class (look enum declaration) 
        virtual std::string gettype(){
            std::cout << "This is an empty declaration! \n";
            return NULL;
        };

        // frees the allocated memory
        virtual void nuke(){
            std::cout << "This is an empty declaration! \n";
        }
};

// returns the position of the agent 1 meter in one direction -> speed of agent needs still be implemented
position<double> getmovementvector( position<double> pos, position<double> target ){
    double x,y,pyt; 
    x = target.x - pos.x;
    y = target.y - pos.y;
    pyt = sqrt(x*x+y*y);

    if( pyt < 1){
        return position<double> { pos.x + x, pos.y + y};
    }
    else{
        return position<double> { pos.x + x/pyt, pos.y + y/pyt};
    }
}
