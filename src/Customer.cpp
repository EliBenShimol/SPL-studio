
#include <../include/Customer.h>
#include "string"
#include <iostream>

using namespace std;


std::string Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

Customer::Customer(std::string c_name, int c_id) : id(c_id), name(c_name) {}

std::vector<Workout> Customer::sort(const std::vector<Workout> &workout_options) {
    std::vector<Workout> temp;
    int max = 0;
    int place = -1;
    int last = -1;
    bool used= true;
    for (unsigned int j = 0; j < workout_options.size(); j++) {
        max=0; place=-1;used= true;
        for (unsigned int i = 0; i < workout_options.size(); i++) {
            bool found= true;
            if(last!=i) {
                if(last==-1) {
                    if (workout_options[i].getPrice() > max) {
                        max = workout_options[i].getPrice();
                        place = i;
                    }
                }
                else
                if(temp[temp.size()-1].getPrice()>=workout_options[i].getPrice())
                    if(temp[temp.size()-1].getPrice()==workout_options[i].getPrice()) {
                        for(unsigned int k=0;k<temp.size();k++)
                            if(temp[k].getId()==workout_options[i].getId())
                                found= false;
                        if(found&used) {
                            max = workout_options[i].getPrice();
                            place = i;
                            used=false;
                        }
                    }
                    else if (workout_options[i].getPrice() > max) {
                        max = workout_options[i].getPrice();
                        place = i;

                    }
            }

        }
        last = place;
        temp.push_back(workout_options[place]);
    }
    return temp;
}
///////////////////////////////////////

SweatyCustomer::SweatyCustomer(std::string name, int id) : Customer(name, id) {

}
std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> ans;
    for(unsigned int i=0;i<workout_options.size();i++) {
        if(workout_options[i].getType()==CARDIO)
            ans.push_back(workout_options[i].getId());
    }
    return ans;
}

std::string SweatyCustomer::toString() const {
    return this->getName()+","+"swt";
}
///////////////


CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id) {

}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<Workout> temp=this->sort(workout_options);
    std::vector<int> ans;
    ans.push_back(temp[temp.size()-1].getId());
    temp.clear();
    return ans;

}

std::string CheapCustomer::toString() const {
    return this->getName()+","+"chp";
}

////////////////
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(name, id) {
}
std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<Workout> temp=this->sort(workout_options);
    std::vector<int> ans;
    for(unsigned int i=0;i<temp.size();i++) {
        Workout x(temp[i]);
        if(x.getType()==ANAEROBIC)
            ans.push_back(x.getId());
    }
    return ans;
}


std::string HeavyMuscleCustomer::toString() const {
    return this->getName()+","+"mcl";
}
///////////////////////////

FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name, id) {

}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<Workout> tempCrd;
    std::vector<Workout> tempMix;
    std::vector<Workout> tempAn;
    for(unsigned int i=0;i<workout_options.size();i++) {
        Workout x(workout_options[i]);
        if(x.getType()==ANAEROBIC)
            tempAn.push_back(x);
        else if(x.getType()==MIXED)
            tempMix.push_back(x);
        else
            tempCrd.push_back(x);
    }
    std::vector<Workout> temp;
    std::vector<int> ans;
    if(tempCrd.size()>0) {
        temp = sort(tempCrd);
        unsigned int check=temp.size()-1;
        while(check>0&&temp[check].getPrice()==temp[check-1].getPrice())
            check=check-1;
        ans.push_back(temp[check].getId());
        temp.clear();
    }
    if(tempMix.size()>0) {
        temp = sort(tempMix);
        ans.push_back(temp[0].getId());
        temp.clear();
    }


    if(tempAn.size()>0) {
        temp = sort(tempAn);
        unsigned int check=temp.size()-1;
        while(check>0&&temp[check].getPrice()==temp[check-1].getPrice())
            check=check-1;
        ans.push_back(temp[check].getId());
        temp.clear();
    }
    return ans;
}
std::string FullBodyCustomer::toString() const {
    return this->getName()+","+"fbd";
}


