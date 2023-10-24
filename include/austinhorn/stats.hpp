#ifndef stats_hpp
#define stats_hpp

// Object counter class
template <typename T>
struct objCount {
    objCount () {
        //cout << "object created" << endl;
        this->objects_created++;
        this->objects_alive++;
    }
    objCount (const objCount& origObjCount) {
        //cout << "object created" << endl;
        this->objects_created++;
        this->objects_alive++;
    }
    static int getObjectsCreated () { return objects_created; }
    static int getObjectsAlive () { return objects_alive; }
protected:
    virtual ~objCount() {
        //cout << "object destroyed" << endl;
        --objects_alive;
    }
    static int objects_created;
    static int objects_alive;
};

// Initialize static data members
template <typename T> int objCount<T>::objects_created( 0 );
template <typename T> int objCount<T>::objects_alive( 0 );

#endif /* stats_hpp */