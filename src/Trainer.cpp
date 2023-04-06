
#include <iostream>
#include "../include/Trainer.h"
#include "string"

using namespace std;

Trainer::Trainer(int t_capacity) {
    capacity=t_capacity;
    salary=0;
    open=false;


}
Trainer::Trainer(const Trainer &other) {
    capacity=other.capacity;
    open=other.open;
    salary=other.salary;
    customersList=other.customersList;
    for(unsigned int i=0;i<other.orderList.size();i++)
        orderList.push_back(orderList[i]);
}

int Trainer::getCapacity() const {
    return capacity;
}

void Trainer::addCustomer(Customer *customer) {
    customersList.push_back(customer);

}

Customer *Trainer::getCustomer(int id) {
    Customer *ans= nullptr;
    for(unsigned int i=0;i<customersList.size();i++)
        if(customersList[i]->getId()==id)
            ans=customersList[i];
    return ans;
}

std::vector<Customer *> &Trainer::getCustomers() {
    return customersList ;
}

std::vector<OrderPair> &Trainer::getOrders() {
    return orderList;
}

void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const vector<Workout> &workout_options) {
    Customer *x=getCustomer(customer_id);
    if(x!= nullptr) {
        std::vector<int> workouts = x->order(workout_options);
        std::vector<Workout> ans;
        for (unsigned int i = 0; i < workouts.size(); i++)
            ans.push_back(workout_options[workouts[i]]);
        for (unsigned int i = 0; i < ans.size(); i++) {
            OrderPair temp(x->getId(), ans[i]);
            std::cout<< getCustomer(customer_id)->getName()+" Is Doing "+ans[i].getName()<<std::endl;
            orderList.push_back(temp);
            salary+=ans[i].getPrice();
        }

        workouts.clear();
        ans.clear();
        x= nullptr;
    }

}

void Trainer::openTrainer() {
    open=true;
}

void Trainer::closeTrainer() {
    open= false;
    orderList.clear();
    customersList.clear();


}

int Trainer::getSalary() {
    return salary;
}

bool Trainer::isOpen() {
    return open;
}

void Trainer::removeCustomer(int id) {
    std::vector<Customer*> ans;
    for(unsigned int i=0;i<customersList.size();i++)
        if(customersList[i]->getId()!=id)
            ans.push_back(customersList[i]);
    customersList.clear();
    for(unsigned int i=0;i<ans.size();i++)
        customersList.push_back(ans[i]);
    ans.clear();
    std::vector<OrderPair> temp;
    for(unsigned int i=0;i<orderList.size();i++)
        if(orderList[i].first!=id)
            temp.push_back(orderList[i]);
    orderList.clear();
    for(unsigned int i=0;i<temp.size();i++)
        orderList.push_back(temp[i]);
}
