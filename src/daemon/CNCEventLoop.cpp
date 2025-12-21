#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <getopt.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>

#include "CNCEventLoop.h"

//#define MAX_EVENTS 10

CNCEventFD::CNCEventFD()
{
    m_callback = NULL;
}

CNCEventFD::~CNCEventFD()
{

}

int
CNCEventFD::getFD()
{
    return m_trigger.getFD();
}

CNCEVLP_RESULT_T
CNCEventFD::init( CNCEventCB *callback )
{
    m_callback = callback;

    if( m_trigger.setup() != HNEP_RESULT_SUCCESS )
        return CNCEVLP_RESULT_FAILURE;

    return CNCEVLP_RESULT_SUCCESS;
}

void
CNCEventFD::signalEvent()
{
    m_trigger.trigger();
}

void
CNCEventFD::clearEvent()
{
    m_trigger.reset();
}

void
CNCEventFD::makeCallback()
{
    if( m_callback )
        m_callback->eventFD( m_trigger.getFD() );
}

CNCEventLoop::CNCEventLoop()
{
}

CNCEventLoop::~CNCEventLoop()
{
}

CNCEVLP_RESULT_T
CNCEventLoop::init()
{
    m_quitTrigger.setup();

    m_eventLoop.setup( this );

    return CNCEVLP_RESULT_SUCCESS;
}

CNCEVLP_RESULT_T
CNCEventLoop::registerFD( int fd,  CNCEventCB *callback )
{
    if( m_eventLoop.addFDToEPoll( fd ) != HNEP_RESULT_SUCCESS )
        return CNCEVLP_RESULT_FAILURE;

    m_fdList.insert( std::pair< int, CNCEventCB* >( fd, callback ) );

    return CNCEVLP_RESULT_SUCCESS;
}

CNCEventFD*
CNCEventLoop::createEventFD( CNCEventCB *callback )
{
    CNCEventFD *newEvt = new CNCEventFD;

    newEvt->init( callback );

    m_evtList.insert( std::pair< int, CNCEventFD* >( newEvt->getFD(), newEvt ) );

    return newEvt;
}

void
CNCEventLoop::signalQuit()
{
    m_quitTrigger.trigger();
}

void
CNCEventLoop::clearQuit()
{
    m_quitTrigger.reset();
}

void
CNCEventLoop::loopIteration()
{

}

void
CNCEventLoop::timeoutEvent()
{

}

void
CNCEventLoop::fdEvent( int sfd )
{
    if( sfd == m_quitTrigger.getFD() )
    {
        clearQuit();
        printf("EventLoop quit signal detected\n");
        return;
    }

    std::map< int, CNCEventCB* >::iterator it = m_fdList.find( sfd );

    if( it != m_fdList.end() )
    {
        it->second->eventFD( sfd );
        return;
    }

    std::map< int, CNCEventFD* >::iterator et = m_evtList.find( sfd );

    if( et != m_evtList.end() )
    {
        et->second->clearEvent();
        et->second->makeCallback();
    }
}

void
CNCEventLoop::fdError( int sfd )
{

}

CNCEVLP_RESULT_T
CNCEventLoop::run()
{
    m_eventLoop.run();

/*
    struct epoll_event events[MAX_EVENTS];
    uint nfds = 0;

    printf( "EventLoop::run - loop start\n" );
    
    for( ;; )
    {
        nfds = epoll_wait( m_epollFD, events, MAX_EVENTS, -1 );
        if( nfds == -1 )
        {
            perror( "epoll_wait" );
            return EVLP_RESULT_FAILURE;
        }

        for( int n = 0; n < nfds; ++n )
        {
            if( events[n].data.fd == m_quitFD )
            {
                clearQuit();
                printf("EventLoop quit signal detected\n");
                return EVLP_RESULT_SUCCESS;
            }

            std::map< int, ELEventCB* >::iterator it = m_fdList.find( events[n].data.fd );

            if( it != m_fdList.end() )
            {
                it->second->eventFD( events[n].data.fd );
                continue;
            }

            std::map< int, ELEventFD* >::iterator et = m_evtList.find( events[n].data.fd );

            if( et != m_evtList.end() )
            {
                et->second->clearEvent();
                et->second->makeCallback();
                continue;
            }

        }
    }
*/
    return CNCEVLP_RESULT_SUCCESS;
}
