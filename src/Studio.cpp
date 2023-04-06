#include "string"
#include <iostream>
#include <../include/Studio.h>
#include <fstream>


using namespace std;

Studio::Studio() {
}

Studio::Studio(const string &configFilePath) {
    open=true;
    canBackUp=false;
    ifstream file(configFilePath);
    char line[256];
    vector<string> lines;
    string chars="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz 0123456789";
    string charsNoBlank="ABCDEFGHIJKLMNOPQESTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    string numbers="0123456789";
    string abc="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    while(file){
        file.getline(line,256);
        if(chars.find(line[0])!=18446744073709551615) {
            lines.push_back(line);
        }
    }
    int lplace=0;
    string thisl="";
    int place=0;
    int numt=-1;
    while(numt==-1){
        thisl=lines[lplace];
        place=0;
        while(place<thisl.length()&&charsNoBlank.find(thisl[place])==18446744073709551615)
            place++;
        if(place<thisl.length()){
            string scapt="";
            while(place<thisl.length()&&thisl[place!=' ']) {
                scapt = scapt + thisl[place];
                place++;
            }
            numt=stoi(scapt);
        }
        else
            lplace++;
    }
    ///end of finding the num of trainers
    lplace++;
    place=0;
    int finish=0;
    int capt=-1;
    thisl="";
    while(capt==-1){
        thisl=lines[lplace];
        place=0;
        while(place<thisl.length()&&charsNoBlank.find(thisl[place])==18446744073709551615)
            place++;
        if(place<thisl.length()){
            for(int id=1;place<thisl.length()&&id<numt;id++){
                while(thisl[place]!=',')
                    place++;
                finish=place-1;
                place++;
                while(thisl[finish]==' ')
                    finish--;
                int start=finish;
                while(numbers.find(thisl[start])!=18446744073709551615)
                    start--;
                start++;
                string scapt="";
                for(;start<=finish;start++)
                    scapt=scapt+thisl[start];
                capt=stoi(scapt);
                Trainer *add=  new Trainer(capt);
                trainers.push_back(add);
            }
        }
        else
            lplace++;
    }
    while(thisl[place]==' ')
        place++;
    string scapt="";
    for(;place<thisl.length();place++)
        scapt=scapt+thisl[place];
    capt=stoi(scapt);
    Trainer *add=new Trainer(capt);
    trainers.push_back(add);
    /////end of creating trainers
    lplace++;
    place=0;
    finish=0;
    string name="";
    WorkoutType wt;
    thisl="";
    int id=0;
    while(lplace<lines.size()){
        name="";
        thisl=lines[lplace];
        place=0;
        while(place<thisl.length()&&abc.find(thisl[place])==18446744073709551615)
            place++;
        if(place<thisl.length()){
            while(thisl[place]!=',')
                place++;
            finish=place-1;
            place++;
            while(thisl[finish]==' ')
                finish--;
            int start=finish;
            while(abc.find(thisl[start])!=18446744073709551615)
                start--;
            if(abc.find(thisl[start-1])!=18446744073709551615){
                start--;
                while(abc.find(thisl[start])!=18446744073709551615)
                    start--;
            }

            start++;
            for(;start<=finish;start++)
                name=name+thisl[start];
            while(thisl[place]!=',')
                place++;
            finish=place-1;
            place++;
            while(thisl[finish]==' ')
                finish--;
            start=finish;
            while(abc.find(thisl[start])!=18446744073709551615)
                start--;
            start++;
            string type="";
            for(;start<=finish;start++)
                type=type+thisl[start];
            if(type=="Anaerobic")
                wt=ANAEROBIC;
            else if(type=="Mixed")
                wt=MIXED;
            else
                wt=CARDIO;
            while(thisl[place]==' ')
                place++;
            scapt="";
            for(;place<thisl.length();place++)
                scapt=scapt+thisl[place];
            capt=stoi(scapt);
            Workout b(id,name,capt,wt);
            id++;
            workout_options.push_back(b);
            lplace++;

        }
        else
            lplace++;
    }

    ///end of config file
}

void Studio::start() {
    cout<<"Studio is now open"<<endl;
    string numbers="0123456789";
    int cid=0;
    while (open){
        char temp[256];
        cin.getline (temp, 256);
        string act="";
        string full=temp;
        int place=0;
        while(place<full.length()&&full[place]==' ')
            place++;
        while(place<full.length()&&full[place]!=' ')
            place++;
        for(int i=0;i<place;i++)
            act = act + full[i];
        ////finding the action
        if(!act.compare("open")){
            std::vector<Customer *> customers;
            int tid;
            while(place<full.length()&&full[place]==' ')
                place++;
            string scapt="";
            while(full[place]!=' ') {
                scapt = scapt + full[place];
                place++;
            }
            tid=stoi(scapt);
            while(place<full.length()){
                while(place<full.length()&&full[place]==' ')
                    place++;
                string name;
                while(full[place]!=' '&&full[place]!=','){
                    name=name+full[place];
                    place++;
                }
                while(place<full.length()&&(full[place]==' '||full[place]==','))
                    place++;
                string type;
                while(place<full.length()&&full[place]!=' '&&full[place]!=','){
                    type=type+full[place];
                    place++;
                }
                Customer *c;
                if(!type.compare("swt"))
                    c=new SweatyCustomer(name,cid);
                else if(!type.compare("chp"))
                    c=new CheapCustomer(name,cid);
                else if(!type.compare("mcl"))
                    c=new HeavyMuscleCustomer(name,cid);
                else
                    c=new FullBodyCustomer(name,cid);
                customers.push_back(c);
                cid++;

            }

            BaseAction *ba=new OpenTrainer(tid,customers);
            ba->act(*this);
            if(ba->getStatus()==ERROR)
                cout<<ba->toString()<<endl;
            actionsLog.push_back(ba);

        }
        else if(!act.compare("order")){
            int tid;
            while(place<full.length()&&full[place]==' ')
                place++;
            string scapt="";
            while(place<full.size()&&full[place]!=' ') {
                scapt = scapt + full[place];
                place++;
            }
            tid=stoi(scapt);
            BaseAction *ba=new Order(tid);
            ba->act(*this);
            if(ba->getStatus()==ERROR)
                cout<<ba->toString()<<endl;
            actionsLog.push_back(ba);
        }
        else if(!act.compare("move")){
            int tid1;
            int tid2;
            int cid;
            while(place<full.length()&&full[place]==' ')
                place++;
            string scapt="";
            while(place<full.size()&&full[place]!=' ') {
                scapt = scapt + full[place];
                place++;
            }
            tid1=stoi(scapt);
            while(place<full.length()&&full[place]==' ')
                place++;
            scapt="";
            while(place<full.size()&&full[place]!=' ') {
                scapt = scapt + full[place];
                place++;
            }
            tid2=stoi(scapt);
            while(place<full.length()&&full[place]==' ')
                place++;
            scapt="";
            while(place<full.size()&&full[place]!=' ') {
                scapt = scapt + full[place];
                place++;
            }
            cid=stoi(scapt);
            BaseAction *ba=new MoveCustomer(tid1,tid2,cid);
            ba->act(*this);
            if(ba->getStatus()==ERROR)
                cout<<ba->toString()<<endl;
            actionsLog.push_back(ba);
            if(getTrainer(tid1)->getCustomers().size()==0){
                BaseAction *ba=new Close(tid1);
                ba->act(*this);
                if(ba->getStatus()==ERROR)
                    cout<<ba->toString()<<endl;
                actionsLog.push_back(ba);
            }
        }
        else if(!act.compare("close")){
            int tid;
            while(place<full.length()&&full[place]==' ')
                place++;
            string scapt="";
            while(place<full.size()&&full[place]!=' ') {
                scapt = scapt + full[place];
                place++;
            }
            tid=stoi(scapt);
            BaseAction *ba=new Close(tid);
            ba->act(*this);
            if(ba->getStatus()==ERROR)
                cout<<ba->toString()<<endl;
            actionsLog.push_back(ba);

        }
        else if(!act.compare("closeall")) {
            BaseAction *ba=new CloseAll();
            actionsLog.push_back(ba);
            ba->act(*this);
            for(unsigned int i=0;i<trainers.size();i++)
                if(trainers[i]->isOpen()) {
                    BaseAction *cl = new Close(i);
                    cl->act(*this);
                    actionsLog.push_back(cl);
                }
            open = false;
            }
        else if(!act.compare("workout_options")){
            BaseAction *ba=new PrintWorkoutOptions();
            ba->act(*this);
            actionsLog.push_back(ba);
        }
        else if(!act.compare("status")){
            int tid;
            while(place<full.length()&&full[place]==' ')
                place++;
            string scapt="";
            while(place<full.size()&&full[place]!=' ') {
                scapt = scapt + full[place];
                place++;
            }
            tid=stoi(scapt);
            BaseAction *ba=new PrintTrainerStatus(tid);
            ba->act(*this);
            actionsLog.push_back(ba);
        }
        else if(!act.compare("log")){
            BaseAction *ba=new PrintActionsLog();
            ba->act(*this);
            actionsLog.push_back(ba);
        }
        else if(!act.compare("backup")) {
            BaseAction *ba = new BackupStudio();
            ba->act(*this);
            canBackUp= true;
            actionsLog.push_back(ba);
        }
        else if(!act.compare("restore")){
            if(canBackUp== true){
                canBackUp=false;
                BaseAction *ba = new RestoreStudio();
                ba->act(*this);
                actionsLog.push_back(ba);
            }
            else{
                cout<<"No backup available"<<endl;
            }

        }

        }
    }

int Studio::getNumOfTrainers() const {
    return trainers.size();
}

Trainer *Studio::getTrainer(int tid) {
    return trainers[tid];
}

const std::vector<BaseAction *> &Studio::getActionsLog() const {
    return actionsLog;
}

std::vector<Workout> &Studio::getWorkoutOptions() {
    return workout_options;
}

void Studio::clean() {
    trainers.clear();
    workout_options.clear();
    actionsLog.clear();
}

Studio::Studio(Studio &other) {//copy constructor
    for(unsigned int i=0;i<other.trainers.size();i++)
        this->trainers.push_back(other.trainers[i]);
    for(unsigned int i=0;i<other.workout_options.size();i++)
        this->workout_options.push_back(other.workout_options[i]);
    for(unsigned int i=0;i<other.actionsLog.size();i++)
        this->actionsLog.push_back(other.actionsLog[i]);
    this->open=other.open;
    this->canBackUp=other.canBackUp;
}

Studio::~Studio() {//destructor
    clean();

}

// Copy Assignment
Studio& Studio::operator=(Studio *s) {
    if (this != s) {
        clean();
        Studio(s);
    }

    return *this;
}
