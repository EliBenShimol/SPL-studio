#include "string"
#include <../include/Action.h>
#include <../include/Trainer.h>
#include <../include/Studio.h>

using namespace std;
extern Studio* backup;

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status=COMPLETED;
    this->errorMsg="";
}

void BaseAction::error(std::string errorMsg) {
    status=ERROR;
    this->errorMsg= errorMsg;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

BaseAction::BaseAction() {}

OpenTrainer::OpenTrainer() : trainerId() {}
OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList) : trainerId(id) {
    for(unsigned int i=0;i<customersList.size();i++)
        customers.push_back(customersList[i]);


}

void OpenTrainer::act(Studio &studio) {
    Trainer *t=studio.getTrainer(trainerId);
    if(!t->isOpen()) {
        for (unsigned int i = 0; i < customers.size(); i++)
            t->addCustomer(customers[i]);
        t->openTrainer();
        complete();
    }
    else{
        string er="Workout session does not exist or is already open";
        error(er);
    }


}

std::string OpenTrainer::toString() const {
    string ans;
    ans="open "+ to_string(trainerId);
    for(unsigned int i=0;i<customers.size();i++)
        ans = ans + " " + customers[i]->toString();
    if(getStatus()==COMPLETED)
         ans=ans+" completed";
    else
        ans="ERROR: "+getErrorMsg();


    return ans;
}

Order::Order(int id) : trainerId(id) {
}

void Order::act(Studio &studio) {
    Trainer *t=studio.getTrainer(trainerId);
    std::vector<int> ids;
    for (unsigned int i = 0; i < studio.getWorkoutOptions().size(); i++)
        ids.push_back(studio.getWorkoutOptions()[i].getId());
    for (unsigned int i = 0; i < t->getCustomers().size(); i++)
        t->order(t->getCustomers()[i]->getId(), ids, studio.getWorkoutOptions());

}


std::string Order::toString() const {
    string ans;
    ans=ans+"order ";
    ans=ans+ to_string(trainerId);
    if(getStatus()==COMPLETED)
        ans=ans+" "+"completed";
    else
        ans=ans+" ERROR:"+getErrorMsg();
    return ans;
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId) : dstTrainer(dst), id(customerId), srcTrainer(src) {}

void MoveCustomer::act(Studio &studio) {
    if(srcTrainer<studio.getNumOfTrainers()&&dstTrainer<studio.getNumOfTrainers()) {
        Trainer *less = studio.getTrainer(srcTrainer);
        Trainer *add = studio.getTrainer(dstTrainer);
        Customer *c=less->getCustomer(id);
        if(c!= nullptr) {
            add->addCustomer(c);
            for (unsigned int i = 0; i < less->getOrders().size(); i++)
                if (less->getOrders()[i].first == id)
                    add->getOrders().push_back(less->getOrders()[i]);
            less->removeCustomer(id);
        }
        else{
            string er="Cannot move customer";
            error(er);
        }
    }
    else{
        string er="Cannot move customer";
        error(er);
    }


}

std::string MoveCustomer::toString() const {
    string ans;
    ans=ans+"move ";
    ans=ans+ to_string(srcTrainer);
    ans=ans+to_string(dstTrainer);
    ans=ans+to_string(id);
    if(getStatus()==COMPLETED)
        ans=ans+" "+"completed";
    else
        ans=ans+" ERROR:"+getErrorMsg();
    return ans;
}

Close::Close(int id) : trainerId(id) {}

void Close::act(Studio &studio) {
    Trainer *t=studio.getTrainer(trainerId);
    if(t->isOpen()) {
        t->closeTrainer();
        std::cout<<"Trainer "+ to_string(trainerId)+" closed. Salary " + to_string(t->getSalary())+"NIS"<<endl;
        for(unsigned int i=0;i<t->getCustomers().size();i++)
            cout<<i<<endl;
        for(unsigned int i=0;i<t->getOrders().size();i++)
            cout<<i<<endl;
        complete();
    }
    else{
        string er="Workout session does not exist or is not open";
        error(er);
    }


}

std::string Close::toString() const {
    string ans;
    ans="close "+ to_string(trainerId);
    if(getStatus()==COMPLETED)
        ans=ans+" completed";
    else
        ans="ERROR: "+getErrorMsg();
    return ans;
}

CloseAll::CloseAll() {

}

void CloseAll::act(Studio &studio) {
    cout<<toString()<<endl;
}

std::string CloseAll::toString() const {
    return "close all";
}

PrintWorkoutOptions::PrintWorkoutOptions() {

}

void PrintWorkoutOptions::act(Studio &studio) {
    for(unsigned int i=0;i<studio.getWorkoutOptions().size();i++)
        cout<<studio.getWorkoutOptions()[i].toString()<<endl;
}

std::string PrintWorkoutOptions::toString() const {
    return "workout_options.";
}

PrintTrainerStatus::PrintTrainerStatus(int id) : trainerId(id) {}

void PrintTrainerStatus::act(Studio &studio) {
    Trainer *t = studio.getTrainer(trainerId);
    string open = "open";
    if (!t->isOpen())
        open = "closed";
    cout << "trainer " + to_string(trainerId) + " status: " + open << endl;
    cout << "Customers:" << endl;
    for (unsigned int i = 0; i < t->getCustomers().size(); i++)
        cout << to_string(t->getCustomers()[i]->getId()) + " " + t->getCustomers()[i]->getName() << endl;
    cout << "Orders:" << endl;
    for (unsigned int i = 0; i < t->getOrders().size(); i++)
        cout << t->getOrders()[i].second.getName() + " " + to_string(t->getOrders()[i].second.getPrice()) +" "+
                to_string(t->getOrders()[i].first)<<endl;
    cout<<"Current Trainer's Salary: "+ to_string(t->getSalary())+"NIS"<<endl;
}

std::string PrintTrainerStatus::toString() const {
    return "status trainer "+ to_string(trainerId);
}

PrintActionsLog::PrintActionsLog() {

}

void PrintActionsLog::act(Studio &studio) {
    for(unsigned int i=0;i<studio.getActionsLog().size();i++)
        cout<<studio.getActionsLog()[i]->toString()<<endl;
}

std::string PrintActionsLog::toString() const {
    return "log";
}

BackupStudio::BackupStudio() {

}

void BackupStudio::act(Studio &studio) {
    if(backup!= nullptr)
        backup->~Studio();
    backup=new Studio(studio);

}

std::string BackupStudio::toString() const {
    return "backup";
}

RestoreStudio::RestoreStudio() {

}

void RestoreStudio::act(Studio &studio) {
    studio.~Studio();
    studio=backup;
    backup->~Studio();
}

std::string RestoreStudio::toString() const {
    return "restore";
}
