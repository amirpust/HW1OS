#ifndef defines_H
#define defines_H

static int debugScope = 0;

#define PRINT_START do{                             \
        for(int debugScopeIndex = 0; debugScopeIndex < debugScope; debugScopeIndex++) \
            cout << "\t" ;                          \
        cout <<"Start :"<< __FUNCTION__ << endl;    \
        debugScope++;                              \
        }while(0)


#define PRINT_END do{                             \
        for(int debugScopeIndex = 0; debugScopeIndex < debugScope; debugScopeIndex++) \
            cout << "\t" ;                          \
        cout <<"END :"<< __FUNCTION__ << endl;    \
        debugScope--;                               \
        }while(0)



#define PRINT_START_PARAM(param) do{    \
        for(int debugScopeIndex = 0; debugScopeIndex < debugScope; debugScopeIndex++) \
            cout << "\t" ;              \
        cout <<"Start :"<< __FUNCTION__ << " With " << #param << ": " << param << endl; \
        debugScope++;                               \
        }while(0)


#define PRINT_END_PARAM(param) do{    \
        for(int debugScopeIndex = 0; debugScopeIndex < debugScope; debugScopeIndex++) \
            cout << "\t" ;              \
        cout <<"END :"<< __FUNCTION__ << " With " << #param << ": " << param << endl; \
        debugScope--;                                \
        }while(0)


#define GRADE  100
#endif
