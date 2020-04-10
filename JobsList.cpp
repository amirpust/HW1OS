//
// Created by jonat on 10/04/2020.
//

#include "JobsList.h"

JobsList::~JobsList() {
    for (auto i : jobs)
        delete i;

    delete [] jobs;
}
