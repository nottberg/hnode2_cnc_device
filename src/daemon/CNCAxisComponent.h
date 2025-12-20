#ifndef __CNC_AXIS_COMPONENT_H__
#define __CNC_AXIS_COMPONENT_H__

#include <string>
#include <map>

#include "CNCEventLoop.h"

typedef enum CNCAxisComponentResultType
{
    CNCACOMP_RESULT_SUCCESS,
    CNCACOMP_RESULT_FAILURE
}CNCACOMP_RESULT_T;

#define CNCACOMP_FUNC_CANBUS_CONTROLLER  "CANController"
#define CNCACOMP_FUNC_DRIVER             "Driver"

class CNCAxisComponent : public CNCEventCB 
{
    public:
        CNCAxisComponent();
       ~CNCAxisComponent();

        void setID( std::string id );
        std::string getID();

        void setFunction( std::string function );
        std::string getFunction();

        virtual void eventFD( int fd );

        virtual CNCACOMP_RESULT_T registerWithEventLoop( CNCEventLoop *loop );

    private:

        std::string  m_id;
        std::string  m_function;
};

#endif // __CNC_AXIS_COMPONENT_H__