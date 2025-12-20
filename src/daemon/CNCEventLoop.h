#ifndef  __CNC_EVENT_LOOP_H__
#define __CNC_EVENT_LOOP_H__

#include <map>

#include <hnode2/HNEPLoop.h>

typedef enum EventLoopResultCodes
{
    CNCEVLP_RESULT_SUCCESS,
    CNCEVLP_RESULT_FAILURE
}CNCEVLP_RESULT_T;

class CNCEventCB
{
    public:
        virtual void eventFD( int fd ) = 0;
};

class CNCEventFD
{
    public:
        CNCEventFD();
       ~CNCEventFD();

        int getFD();

        CNCEVLP_RESULT_T init( CNCEventCB *callback );

        void signalEvent();
        void clearEvent();

        void makeCallback();
        
    private:

        int        m_fd;
        
        CNCEventCB *m_callback;
};

class CNCEventLoop
{
    public:
        CNCEventLoop();
       ~CNCEventLoop();

        CNCEVLP_RESULT_T init();

        CNCEVLP_RESULT_T registerFD( int fd, CNCEventCB *callback );

        CNCEventFD* createEventFD( CNCEventCB *callback );

        CNCEVLP_RESULT_T run();

        void signalQuit();
        void clearQuit();
        
    private:

        HNEPLoop  m_evtLoop;

        //int m_epollFD;
        //int m_quitFD;

        std::map< int, CNCEventCB* > m_fdList;

        std::map< int, CNCEventFD* > m_evtList;
};

#endif // __CNC_EVENT_LOOP_H__