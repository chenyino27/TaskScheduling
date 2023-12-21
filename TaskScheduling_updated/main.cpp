//
//  main.cpp
//  TaskScheduling_updated
//
//  Created by Chenyi on 12/11/23.
//

#include<iostream>
#include<vector>
#include<memory>
#include<string>
#include<algorithm>
#include<stack>
using namespace std;

double p1=1;           //the power consumption of core 1
double p2=2;           //the power consumption of core 2
double p3=4;           //the power consumption of core 3
double pc=0.5;         //the power consumption of cloud


class Task{
public:
    int id;             //task id;
    int t_core1;        //executed time in the core 1
    int t_core2;        //executed time in the core 2
    int t_core3;        //executed time in the core 3
    int t_re;           //executed time in the cloud (t_wr+t_cloud+t_ws=3+1+1=5)
    
    vector<int> succ;  //immediate successors of the task
    vector<int> pred;  //immediate predecessors of the task

    Task(int id,int c1,int c2,int c3,const vector<int>& succ={},const vector<int>& pred={}) : id(id),t_core1(c1),t_core2(c2),t_core3(c3),t_re(5),succ(succ),pred(pred) {}
    
    void addSucc(int v){
        succ.emplace_back(v);
    }
    
    void addpred(int v){
        pred.emplace_back(v);
    }
};

struct ScheduledTask{
    Task task;
    int rt_local;       //ready time in local schedule
    int rt_ws;          //ready time in wireless send schedule
    int rt_cloud;       //ready time in cloud schedule
    int rt_wr;          //ready time in wireless receive schedule
    int ft_local;       //finish time in local schedule
    int ft_ws;          //finish time in wireless send schedule
    int ft_cloud;       //finish time in cloud schedule
    int ft_wr;          //finish time in wireless receive schedule
    
    double priority;            //priority of the task
    double e;                   //energy consumption of the task
    double w;                   //computation cost of the task
    vector<int> schedule_id;    //which schedule the task executed in. 0-2 means core 1-3, 3 means wireless sending, 4 means cloud, 5 means wireless receiving

    ScheduledTask(const Task& task) : task(task),rt_local(0),rt_ws(0),rt_cloud(0),rt_wr(0),ft_local(0),ft_ws(0),ft_cloud(0),ft_wr(0),priority(0),e(0),w(0) {}
    
    void addSchedule(int id) {
        schedule_id.emplace_back(id);
    }
    
    void getInitial(){
        rt_local=0;
        rt_ws=0;
        rt_cloud=0;
        rt_wr=0;
        ft_local=0;
        ft_ws=0;
        ft_cloud=0;
        ft_wr=0;
    }

    
};

class Schedule{
public:
    string id;
    vector<int> task_id;//store ids of task executed in this schedule
    
    void addTask(int num) {
        task_id.emplace_back(num);
    }
    
};

class ScheduleSystem{
public:
    vector<ScheduledTask> tasks;
    vector<Schedule> schedules;
    
    vector<int> entry;  //entry tasks
    vector<int> exit;   //exit tasks
    
    int t_total;        //total execution time
    double e_total;
    
    ScheduleSystem(const vector<ScheduledTask>& tasks={}):tasks(tasks),t_total(0),e_total(0),schedules(6){
        schedules[0].id="s_core1";
        schedules[1].id="s_core2";
        schedules[2].id="s_core3";
        schedules[3].id="s_ws";
        schedules[4].id="s_cloud";
        schedules[5].id="s_wr";
    }
    
    //return current finish time of schedule s
    int finishTime(int s){
        if (s<3){//return finish time of s_core
            if (schedules[s].task_id.empty()) return 0;
            return tasks[schedules[s].task_id.back()].ft_local;
        }else if (s==3){//return finish time of s_ws
            if (schedules[s].task_id.empty()) return 0;
            if (tasks[schedules[s].task_id.back()].rt_ws==0){
                return tasks[schedules[s].task_id[schedules[s].task_id.size()-2]].ft_ws;
            }
            return tasks[schedules[s].task_id.back()].ft_ws;
        }else if (s==4){//return finish time of s_cloud
            if (schedules[s].task_id.empty()) return 0;
            return tasks[schedules[s].task_id.back()].ft_cloud;
        }else{//return finish time of s_wr
            if (schedules[s].task_id.empty()) return 0;
            return tasks[schedules[s].task_id.back()].ft_wr;
        }
    }
    
    //return total time of whole system
    int getTotalTime(){
        t_total=max(finishTime(0),finishTime(1));
        t_total=max(t_total,finishTime(2));
        t_total=max(t_total,finishTime(5));
        return t_total;
    }
    
    //return total energy consumption of whole system
    double getTotalEnergyConsump(){
        e_total=0;
        for (int i=0;i<tasks.size();i++){
            e_total+=tasks[i].e;
        }
        return e_total;
    }
    
    void printTotalTime(){
        getTotalTime();
        cout<<"\n\nTotal time="<<t_total<<endl;
    }
    
    void printTotalEnergyConsump(){
        getTotalEnergyConsump();
        cout<<"Total energy consumption="<<e_total<<endl;
    }
    
    void printSchedule(){
        for (int s=0;s<schedules.size();s++){
            if (s<3){//print tasks executed in core 1-3
                if (s==0){
                    cout<<"Schedule core1: \n";
                }else{
                    cout<<"\n\nSchedule core"<<s+1<<": \n";
                }
                if (!schedules[s].task_id.empty()){
                    for (int i=0;i<schedules[s].task_id.size();i++){
                        int n=schedules[s].task_id[i];
                        cout<<"task["<<n<<"] in ["<<tasks[n].rt_local<<", "<<tasks[n].ft_local<<"]   ";
                    }
                }
            }else if (s==3){//print tasks executed in wireless send
                cout<<"\n\nSchedule wireless send: \n";
                if (!schedules[s].task_id.empty()){
                    for (int i=0;i<schedules[s].task_id.size();i++){
                        int n=schedules[s].task_id[i];
                        cout<<"task["<<n<<"] in ["<<tasks[n].rt_ws<<", "<<tasks[n].ft_ws<<"]   ";
                    }
                }
            }else if (s==4){//print tasks executed in the cloud
                cout<<"\n\nSchedule cloud: \n";
                if (!schedules[s].task_id.empty()){
                    for (int i=0;i<schedules[s].task_id.size();i++){
                        int n=schedules[s].task_id[i];
                        cout<<"task["<<n<<"] in ["<<tasks[n].rt_cloud<<", "<<tasks[n].ft_cloud<<"]   ";
                    }
                }
            }else{//print tasks executed in wireless receive
                cout<<"\n\nSchedule wireless receive: \n";
                if (!schedules[s].task_id.empty()){
                    for (int i=0;i<schedules[s].task_id.size();i++){
                        int n=schedules[s].task_id[i];
                        cout<<"task["<<n<<"] in ["<<tasks[n].rt_wr<<", "<<tasks[n].ft_wr<<"]   ";
                    }
                }
            }
        }

    }
};

double calPrio(ScheduleSystem& schedule,ScheduledTask& v){
    if (v.task.succ.empty()){
        v.priority=v.w;
    }else{
        double max=0;
        for (auto v_succ:v.task.succ){
            double prio=calPrio(schedule,schedule.tasks[v_succ]);//priority of successor
            if (prio>max) max=prio;
        }
        v.priority=v.w+max;
    }
    return v.priority;
}

ScheduleSystem initialScheduling(const vector<Task>& tasklist){
    ScheduleSystem s_initial;
    vector<ScheduledTask> s_tasks;
    for (int i=0;i<tasklist.size();i++){
        s_initial.tasks.emplace_back(tasklist[i]);
        if (tasklist[i].succ.empty()){//check if it is an entry task
            s_initial.exit.emplace_back(i);
        }
        if (tasklist[i].pred.empty()){//check if it is an exit task
            s_initial.entry.emplace_back(i);
        }
    }
    
    //compare t_core3 and t_re of each task to ensure if the task execute in the cloud or not
    for (int i=0;i<s_initial.tasks.size();i++){
        int core1=s_initial.tasks[i].task.t_core1;
        int core2=s_initial.tasks[i].task.t_core2;
        int core3=s_initial.tasks[i].task.t_core3;
        int re=s_initial.tasks[i].task.t_re;
        if (core3>re){//task execute in the cloud
            s_initial.schedules[3].addTask(i);//s_ws
            s_initial.schedules[4].addTask(i);//s_cloud
            s_initial.schedules[5].addTask(i);//s_wr
            s_initial.tasks[i].addSchedule(3);
            s_initial.tasks[i].addSchedule(4);
            s_initial.tasks[i].addSchedule(5);
            s_initial.tasks[i].w=re;
        }else{
            s_initial.tasks[i].w=(core1+core2+core3)/3;
        }
    }
    
    //calculate the priority of each task
    for (int i=0;i<s_initial.entry.size();i++){
        int v=s_initial.entry[i];
        s_initial.tasks[v].priority=calPrio(s_initial,s_initial.tasks[v]);
    }
    
    vector<pair<int, double>> order;    //order: <task id,priority>
    for (int i=0;i<s_initial.tasks.size();i++){
        order.emplace_back(i,s_initial.tasks[i].priority);
    }
    
    //descending order of their priorities
    sort(order.begin(),order.end(),[](const pair<int, double>& a, const pair<int, double>& b) {
        return a.second>b.second;
    });
    
    cout<<"\nPriority of initial schedule (<task id, priority>): ";
    for (int i=0;i<s_initial.tasks.size();i++){
        cout<<"<"<<order[i].first<<", "<<order[i].second<<">   ";
    }
    
    //schedule
    for (auto val:order){
        int v=val.first;
        if (s_initial.tasks[v].schedule_id.empty()){//not decide where this task execute in
            int ft1=s_initial.finishTime(0)+s_initial.tasks[v].task.t_core1;//ft_core1
            int ft2=s_initial.finishTime(1)+s_initial.tasks[v].task.t_core2;//ft_core2
            int ft3=s_initial.finishTime(2)+s_initial.tasks[v].task.t_core3;//ft_core3
            int ftc=s_initial.finishTime(3)+s_initial.tasks[v].task.t_re;//ft_wr
            vector<int> ft_list={ft1,ft2,ft3,ftc};
            int ft_min=*min_element(ft_list.begin(), ft_list.end());
            if (ft_min!=ftc){
                int max_pred_ft;
                if (ft_min==ft1){
                    max_pred_ft=ft_min-s_initial.tasks[v].task.t_core1;
                }else if (ft_min==ft1){
                    max_pred_ft=ft_min-s_initial.tasks[v].task.t_core2;
                }else{
                    max_pred_ft=ft_min-s_initial.tasks[v].task.t_core3;
                }
                vector<int> v_pred=s_initial.tasks[v].task.pred;//predeccors of task v
                if (!v_pred.empty()){
                    for (int i=0;i<v_pred.size();i++){
                        max_pred_ft=max(max_pred_ft,s_initial.tasks[v_pred[i]].ft_local);
                        max_pred_ft=max(max_pred_ft,s_initial.tasks[v_pred[i]].ft_wr);
                    }
                }
                
                s_initial.tasks[v].rt_local=max_pred_ft;
                
                if (ft_min==ft1){//task executed in core 1
                    s_initial.schedules[0].addTask(v);
                    s_initial.tasks[v].addSchedule(0);
                    s_initial.tasks[v].ft_local=max_pred_ft+s_initial.tasks[v].task.t_core1;
                    s_initial.tasks[v].e=p1*s_initial.tasks[v].task.t_core1;
                }else if (ft_min==ft2){//task executed in core 2
                    s_initial.schedules[1].addTask(v);
                    s_initial.tasks[v].addSchedule(1);
                    s_initial.tasks[v].ft_local=max_pred_ft+s_initial.tasks[v].task.t_core2;
                    s_initial.tasks[v].e=p2*s_initial.tasks[v].task.t_core2;
                }else{//task executed in core 3
                    s_initial.schedules[2].addTask(v);
                    s_initial.tasks[v].addSchedule(2);
                    s_initial.tasks[v].ft_local=max_pred_ft+s_initial.tasks[v].task.t_core3;
                    s_initial.tasks[v].e=p3*s_initial.tasks[v].task.t_core3;
                }
                continue;
            }else{
                s_initial.schedules[3].addTask(v);//s_ws
                s_initial.schedules[4].addTask(v);//s_cloud
                s_initial.schedules[5].addTask(v);//s_wr
                s_initial.tasks[v].addSchedule(3);
                s_initial.tasks[v].addSchedule(4);
                s_initial.tasks[v].addSchedule(5);

            }
        }//task executed in the cloud
        int max_pred_ft_1=s_initial.finishTime(3);
        int max_pred_ft_2=0;
        vector<int> v_pred=s_initial.tasks[v].task.pred;
        if (!v_pred.empty()){
            for (int i=0;i<v_pred.size();i++){
                max_pred_ft_1=max(max_pred_ft_1,s_initial.tasks[v_pred[i]].ft_local);
                max_pred_ft_1=max(max_pred_ft_1,s_initial.tasks[v_pred[i]].ft_ws);
                max_pred_ft_2=max(max_pred_ft_2,s_initial.tasks[v_pred[i]].ft_cloud);
            }
        }
        s_initial.tasks[v].rt_ws=max_pred_ft_1;
        s_initial.tasks[v].ft_ws=s_initial.tasks[v].rt_ws+3;
        s_initial.tasks[v].rt_cloud=max(max_pred_ft_2,s_initial.tasks[v].ft_ws);
        s_initial.tasks[v].ft_cloud=s_initial.tasks[v].rt_cloud+1;
        s_initial.tasks[v].rt_wr=s_initial.tasks[v].ft_cloud;
        s_initial.tasks[v].ft_wr=s_initial.tasks[v].rt_wr+1;
        s_initial.tasks[v].e=pc*3;
    }
    return s_initial;
}


ScheduleSystem kernalAlgo(ScheduleSystem s_initial,int ori,int tar,int v){
    ScheduleSystem s_new=s_initial;
    int v_rt;
    stack<int> stack;
    
    vector<int> k_ori={};//ids of task in each schedule
    for (int val:s_initial.schedules[ori].task_id){
        if (val==v) continue;
        k_ori.emplace_back(val);
    }
    s_new.schedules[ori].task_id=k_ori;
   
    
    vector<int> k_tar={};
    bool is_insert=false;//check if task v is inserted into target schedule
    //insert task v into target schedule
    if (tar!=3){//target schedule is one of cores
        s_new.tasks[v].schedule_id={tar};
        int max_pred_ft=0;
        vector<int> v_pred=s_new.tasks[v].task.pred;
        if (!v_pred.empty()){
            for (int i=0;i<v_pred.size();i++){
                max_pred_ft=max(max_pred_ft,s_new.tasks[v_pred[i]].ft_local);
                max_pred_ft=max(max_pred_ft,s_new.tasks[v_pred[i]].ft_wr);
            }
        }
        s_new.tasks[v].rt_local=max_pred_ft;
        v_rt=max_pred_ft;
        if (s_new.schedules[tar].task_id.empty()){
            k_tar={v};
        }else{
            for (int i=0;i<s_new.schedules[tar].task_id.size();i++){
                int t_num=s_new.schedules[tar].task_id[i];//original task in the schedule
                if (!is_insert and v_rt<=s_new.tasks[t_num].rt_local){
                    k_tar.emplace_back(v);
                    is_insert=true;
                }
                k_tar.emplace_back(t_num);
            }
            if (find(k_tar.begin(),k_tar.end(),v)==k_tar.end()){
                k_tar.emplace_back(v);
            }
        }
        s_new.schedules[tar].task_id=k_tar;
    }else{//target schedule is cloud
        s_new.tasks[v].schedule_id={3,4,5};
        int max_pred_ft=0;
        vector<int> v_pred=s_initial.tasks[v].task.pred;
        if (!v_pred.empty()){
            for (int i=0;i<v_pred.size();i++){
                max_pred_ft=max(max_pred_ft,s_initial.tasks[v_pred[i]].ft_local);
                max_pred_ft=max(max_pred_ft,s_initial.tasks[v_pred[i]].ft_ws);
            }
        }
        v_rt=max_pred_ft;
        for (int i=0;i<s_new.schedules[3].task_id.size();i++){
            int t_num=s_new.schedules[3].task_id[i];
            if (!is_insert and v_rt<s_new.tasks[t_num].rt_ws){
                k_tar.emplace_back(v);
                is_insert=true;
            }
            k_tar.emplace_back(t_num);
        }
        if (find(k_tar.begin(),k_tar.end(),v)==k_tar.end()){
            k_tar.emplace_back(v);
        }
        s_new.schedules[3].task_id=k_tar;
        s_new.schedules[4].task_id=k_tar;
        s_new.schedules[5].task_id=k_tar;
    }
    
    
    vector<int> ready1(s_new.tasks.size());//number of task index i's pred
    vector<int> ready2(s_new.tasks.size(),-1);//number of task before task insex i in the same schedule already done

    
    for (int i=0;i<ready1.size();i++){
        ready1[i]=(int)s_new.tasks[i].task.pred.size();
        int s_id=s_new.tasks[i].schedule_id[0];//schedule id of task i in
        for (int id:s_new.schedules[s_id].task_id){
            ready2[i]+=1;
            if (id==i){
                break;
            }
        }
//        cout<<"i="<<i<<", ready 1="<<ready1[i]<<", ready2="<<ready2[i]<<endl;
        if (ready1[i]==0 and ready2[i]==0){
            stack.push(i);
            ready1[i]=-1;
            ready2[i]=-1;
        }
    }
    
    ScheduleSystem s_done=s_new;
    
    for (int i=0;i<s_new.tasks.size();i++){
        s_done.tasks[i].getInitial();
    }
    
    for (int i=0;i<s_new.schedules.size();i++){
        s_done.schedules[i].task_id={};
    }
    
    while (!stack.empty()){
        int v=stack.top();
        stack.pop();
        int s_id=s_done.tasks[v].schedule_id[0];//schedule which task v in
        
        vector<int> v_succ=s_done.tasks[v].task.succ;//successors of task v
        vector<int> v_pred=s_done.tasks[v].task.pred;//predecessors of task v
        vector<int> v_sched=s_new.schedules[s_id].task_id;//tasks in the schedule which task v in
        

        if (s_id<3){
            int max_pred_ft=s_done.finishTime(s_id);
            s_done.schedules[s_id].task_id.emplace_back(v);
            if (!v_pred.empty()){
                for (int i=0;i<v_pred.size();i++){
                    max_pred_ft=max(max_pred_ft,s_done.tasks[v_pred[i]].ft_local);
                    max_pred_ft=max(max_pred_ft,s_done.tasks[v_pred[i]].ft_wr);
                }
            }
            s_done.tasks[v].rt_local=max_pred_ft;
            if(s_id==0){
                s_done.tasks[v].ft_local=max_pred_ft+s_done.tasks[v].task.t_core1;
                s_done.tasks[v].e=p1*s_done.tasks[v].task.t_core1;
            }else if(s_id==1){
                s_done.tasks[v].ft_local=max_pred_ft+s_done.tasks[v].task.t_core2;
                s_done.tasks[v].e=p2*s_done.tasks[v].task.t_core2;
            }else{
                s_done.tasks[v].ft_local=max_pred_ft+s_done.tasks[v].task.t_core3;
                s_done.tasks[v].e=p3*s_done.tasks[v].task.t_core3;
            }
        }else{//task executed in the cloud
            s_done.schedules[3].task_id.emplace_back(v);
            s_done.schedules[4].task_id.emplace_back(v);
            s_done.schedules[5].task_id.emplace_back(v);
            int max_pred_ft_1=s_done.finishTime(3);
            int max_pred_ft_2=0;
            if (!v_pred.empty()){
                for (int i=0;i<v_pred.size();i++){
                    max_pred_ft_1=max(max_pred_ft_1,s_done.tasks[v_pred[i]].ft_local);
                    max_pred_ft_1=max(max_pred_ft_1,s_done.tasks[v_pred[i]].ft_ws);
                    max_pred_ft_2=max(max_pred_ft_2,s_done.tasks[v_pred[i]].ft_cloud);
                }
            }
            s_done.tasks[v].rt_ws=max_pred_ft_1;
            s_done.tasks[v].ft_ws=s_done.tasks[v].rt_ws+3;
            s_done.tasks[v].rt_cloud=max(max_pred_ft_2,s_done.tasks[v].ft_ws);
            s_done.tasks[v].ft_cloud=s_done.tasks[v].rt_cloud+1;
            s_done.tasks[v].rt_wr=s_done.tasks[v].ft_cloud;
            s_done.tasks[v].ft_wr=s_done.tasks[v].rt_wr+1;
            s_done.tasks[v].e=pc*3;
        }

        
        
        for (int i=0;i<v_succ.size();i++){
            ready1[v_succ[i]]-=1;
        }
        for (int i=0;i<v_sched.size();i++){
            if (ready2[v_sched[i]]!=0){
                ready2[v_sched[i]]-=1;
            }
        }
        
        for (int i=0;i<ready1.size();i++){
            if (ready1[i]==0 and ready2[i]==0){
                stack.push(i);
                ready1[i]=-1;
                ready2[i]=-1;
            }
        }
        
    }
    
    return s_done;
}

ScheduleSystem taskMigration(ScheduleSystem s_initial){
    double t_max=s_initial.getTotalTime()*1.5;//50% higher than t_total of initial algo.
    ScheduleSystem s_final=s_initial;
    vector<ScheduleSystem> s_new(3);
    
    for (auto v:s_initial.tasks){
        int best_s=-1;
        double best_ratio=0;
        int best_ratio_s=-1;
        
        int t_id=v.task.id;//task id

        if (v.schedule_id[0]>2){//task v executes in the cloud
            continue;
        }
        
        for (int i=0;i<3;i++){
            
            if (i==0){
                if(v.schedule_id[0]==0){
                    s_new[i]=kernalAlgo(s_final,0,1,t_id);
                }else{
                    s_new[i]=kernalAlgo(s_final,v.schedule_id[0],0,t_id);
                }
            }else if (i==1){
                if(v.schedule_id[0]==2){
                    s_new[i]=kernalAlgo(s_final,2,1,t_id);
                }else{
                    s_new[i]=kernalAlgo(s_final,v.schedule_id[0],2,t_id);
                }
            }else{
                s_new[i]=kernalAlgo(s_final,v.schedule_id[0],3,t_id);
            }
            
        }
        
        for (int j=0;j<3;j++){
            if (s_new[j].getTotalTime()>t_max) continue;
            if (s_new[j].getTotalTime()<=s_final.getTotalTime() and s_new[j].getTotalEnergyConsump()<s_final.getTotalEnergyConsump()){
                if (best_s==-1 or s_new[j].getTotalEnergyConsump()<s_new[best_s].getTotalEnergyConsump()){
                    best_s=j;
                }
            }else if(s_new[j].getTotalTime()>s_final.getTotalTime()){
                const double ratio=(s_final.getTotalEnergyConsump()-s_new[j].getTotalEnergyConsump())/(s_new[j].getTotalTime()-s_final.getTotalTime());
                if (best_ratio_s==-1 or ratio>best_ratio){
                    best_ratio=ratio;
                    best_ratio_s=j;
                }
            }
        }
        
        if (best_s!=-1){
            s_final=s_new[best_s];
        }else if (best_ratio_s!=-1){
            s_final=s_new[best_ratio_s];
        }
    }

    return s_final;
}

 
 
int main(int argc, const char * argv[]) {
    
    /**
     input example#1
     **/
    vector<vector<int>> graph1={{1,2,3,4,5},{7,8},{6},{7,8},{8},{7},{9},{9},{9},{}};
    vector<vector<int>> t_core1={{9,7,5},
                                {8,6,5},
                                {6,5,4},
                                {7,5,3},
                                {5,4,2},
                                {7,6,4},
                                {8,5,3},
                                {6,4,2},
                                {5,3,2},
                                {7,4,2}};

    /**
     input example#2
     **/
    vector<vector<int>> graph2={{1,2,3,4},{6,7},{6},{6,8},{5,8},{7},{9},{9},{9},{}};
    vector<vector<int>> t_core2={{9,7,5},
                                {8,6,5},
                                {6,5,4},
                                {7,5,3},
                                {5,4,2},
                                {7,6,4},
                                {8,5,3},
                                {6,4,2},
                                {5,3,2},
                                {7,4,2}};
    
    
    /**
     input example#3
     **/
    vector<vector<int>> graph3={{1,2,3,4,5},{7,8},{6},{7,8},{8},{7},{9},{9,11},{9,12},{13},{14},{15,16},{17},{18},{18},{18},{18},{18},{19},{}};
    vector<vector<int>> t_core3={{9,7,5},
                                {8,6,5},
                                {6,5,4},
                                {7,5,3},
                                {5,4,2},
                                {7,6,4},
                                {8,5,3},
                                {6,4,2},
                                {5,3,2},
                                {7,4,2},
                                {5,4,3},
                                {5,4,3},
                                {9,7,5},
                                {4,3,2},
                                {6,5,4},
                                {8,7,5},
                                {6,5,3},
                                {4,3,2},
                                {7,5,4},
                                {6,5,4}};
    
    
    /**
     input example#4
     **/
    vector<vector<int>> graph4={{3,4,5},{7,8},{6},{7,8},{8},{7},{9},{9,10,11},{9,12},{13},{14},{15,16},{17},{18},{18},{18},{18},{18},{19},{}};
    vector<vector<int>> t_core4={{9,7,5},
                                {8,6,5},
                                {6,5,4},
                                {7,5,3},
                                {5,4,2},
                                {7,6,4},
                                {8,5,3},
                                {6,4,2},
                                {5,3,2},
                                {7,4,2},
                                {5,4,3},
                                {5,4,3},
                                {9,7,5},
                                {4,3,2},
                                {6,5,4},
                                {8,7,5},
                                {6,5,3},
                                {4,3,2},
                                {7,5,4},
                                {6,5,4}};
    
    /**
     input example#5
     **/
    vector<vector<int>> graph={{3,4,5},{7,8},{6},{7,8},{8},{7},{9},{9,10,11},{9,12},{13},{14},{15,16},{17},{},{18},{18},{},{},{19},{}};
    vector<vector<int>> t_core={{9,7,5},
                                {8,6,5},
                                {6,5,4},
                                {7,5,3},
                                {5,4,2},
                                {7,6,4},
                                {8,5,3},
                                {6,4,2},
                                {5,3,2},
                                {7,4,2},
                                {5,4,3},
                                {5,4,3},
                                {9,7,5},
                                {4,3,2},
                                {6,5,4},
                                {8,7,5},
                                {6,5,3},
                                {4,3,2},
                                {7,5,4},
                                {6,5,4}};
    
    //convert the input graph into the Task we required
    vector<Task> tasklist={};
    for (int i=0;i<t_core.size();i++){
        tasklist.emplace_back(i,t_core[i][0],t_core[i][1],t_core[i][2]);//
     }
    
    for (int i=0;i<graph.size();i++){
        if (graph[i].empty()){//no child node
            continue;
        }
        for (int j=0;j<graph[i].size();j++){
            tasklist[i].addSucc(graph[i][j]);//node j is a successor of node i
            tasklist[graph[i][j]].addpred(i);//node i is a predecessor of node j
        }
    }
    
    ScheduleSystem s_initial=initialScheduling(tasklist);//initial schedule
        
    cout<<"\n-----------------Initial Scheduling-----------------"<<endl;
    s_initial.printSchedule();
    s_initial.printTotalTime();
    s_initial.printTotalEnergyConsump();
    
    ScheduleSystem s_final=taskMigration(s_initial);
    
    cout<<"\n\n-----------------Final Scheduling-----------------"<<endl;
    s_final.printSchedule();
    s_final.printTotalTime();
    s_final.printTotalEnergyConsump();
    
    
    return 0;
}
