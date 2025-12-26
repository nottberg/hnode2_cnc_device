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
        bool isMatch( int sfd );
        
        CNCEVLP_RESULT_T init( CNCEventCB *callback );

        void signalEvent();
        void clearEvent();

        void makeCallback();
        
    private:

        HNEPTrigger m_trigger;
        
        CNCEventCB *m_callback;
};

class CNCEventLoop : public HNEPLoopCallbacks
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
        
        virtual void loopIteration();
        virtual void timeoutEvent();
        virtual void fdEvent( int sfd );
        virtual void fdError( int sfd ); 

    private:

        HNEPLoop  m_eventLoop;

        HNEPTrigger m_quitTrigger; 

        std::map< int, CNCEventCB* > m_fdList;

        std::map< int, CNCEventFD* > m_evtList;
};

#endif // __CNC_EVENT_LOOP_H__