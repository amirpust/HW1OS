//
// Created by jonat on 10/04/2020.
//

#ifndef HW1_JOBSLIST_H
#define HW1_JOBSLIST_H

#include "Commands.h"
#include <exception>
#include <ctime>
#include <cassert>

using std::cout;
using std::endl;



class JobsList {
    typedef enum{
        RUN, STOP, END
    }cmdStatus;
public:
    class JobEntry {
        Command* cmd;
        cmdStatus status;
        time_t startTime;
        time_t stopTime;
        int jobId;
        pid_t pid;
    public:
        JobEntry(Command* _cmd, int _jobId, pid_t p) :
            cmd(_cmd), status(RUN), jobId(_jobId),pid(p){
            startTime = time(nullptr);
        }
        ~JobEntry() {
            delete cmd;
        };

        void updateStatus(){
            if(status == END)
                return;

            int newStatus = 0;
            pid_t p = waitpid(pid, &newStatus, WNOHANG | WUNTRACED | WCONTINUED);

            cout << "Checking  " << p << endl;
            cout << "Checking status of " << pid << endl;
            cout << "newStatus: " << newStatus << endl;
            if( p != 0){
                if(WIFSTOPPED(newStatus)){
                    //TODO: debug
                    cout << "update status stop" << endl;
                    stopTime = time(nullptr);
                    status = STOP;
                }else if(WIFEXITED(newStatus) || WTERMSIG(newStatus)){
                    cout << "update status end" << endl;
                    status = END;
                }else if(WIFCONTINUED(newStatus)){
                    cout << "update status continued" << endl;
                    startTime = time(nullptr);
                    status = RUN;
                }else{
                    cout << "Error: else | Function: " << __FUNCTION__ << " | Line: " << __LINE__  << endl;
                    status = RUN;
                }
            }
        }

        Command *getCmd() const {
            return cmd;
        }

        time_t getStartTime() const {
            return startTime;
        }

        time_t getStopTime() const {
            return stopTime;
        }

        int getJobId() const {
            return jobId;
        }

        bool operator==(const JobEntry &rhs) const {
            return jobId == rhs.jobId;
        }

        bool operator!=(const JobEntry &rhs) const {
            return !(rhs == *this);
        };

        pid_t getJobPid(){
            return pid;
        }

        void stopCmd(){
            assert(status == RUN); // TODO: debug
            if(status != RUN)
                return;

            updateStatus();
            kill(pid, SIGSTOP);
        }
        void continueCmd(){
            assert(status == STOP); //TODO: debug
            if(status != STOP)
                return;

            startTime = time(nullptr);
            updateStatus();
            kill(pid, SIGCONT); //TODO:Check
        }
        void killCmd(){
            assert(status != END); //TODO: debug
            if(status == END)
                return;

            updateStatus();
            kill(pid, SIGKILL);
        }

        cmdStatus getStatus() const {
            return status;
        }
    };
    class notExist: public std::exception{
        int jobId;

        public:
        notExist(int _jobId = 0): jobId(_jobId){};

        const char *what() const noexcept override {
            return ("job-id " + std::to_string(jobId) + string(" does not exist")).c_str();
        }
    };
    class emptyList: public std::exception{};
    class inBG: public std::exception{};

private:
    int counter;
    int maxId;
    vector<JobEntry*> jobs;
    JobEntry* fg;

    // TODO: Add your data members
public:
    JobsList() : counter(0), maxId(0), jobs(), fg(NULL) {};
    ~JobsList();
    void addJob(Command* cmd,pid_t p);
    void printJobsList();
    void killAllJobs();
    int getSize();
    pid_t fgPid ();
    void removeJobById(int jobId);
    bool contains(int jobId);
    void sendSigById(int sig, int jobId = 0);
    void bringFG(int jobId);
    void resumeOnBG(int jobId);

    JobEntry * getJobById(int jobId);
    JobEntry * getLastJob(int* lastJobId = NULL);
    JobEntry *getLastStoppedJob(int *jobId = NULL);
    void killCommand(JobEntry* job, bool toPrint = true);
private:
    void removeFinishedJobs();
    void printKilledCommand(JobEntry* job);
    void runFG();
    void update();

    // TODO: Add extra methods or modify exisitng ones as needed
};


#endif //HW1_JOBSLIST_H
