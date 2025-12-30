#include <unistd.h>
#include <sys/eventfd.h>

#include <Poco/JSON/Object.h>
#include <Poco/StreamCopier.h>

#include "CNCAxisComponent.h"

namespace pjs = Poco::JSON;
namespace pdy = Poco::Dynamic;

CNCAxisComponent::CNCAxisComponent()
{

}

CNCAxisComponent::~CNCAxisComponent()
{

}

void
CNCAxisComponent::setID( std::string id )
{
    m_id = id;
}

std::string
CNCAxisComponent::getID()
{
    return m_id;
}

void
CNCAxisComponent::setFunction( std::string function )
{
    m_function = function;
}

std::string
CNCAxisComponent::getFunction()
{
    return m_function;
}

void
CNCAxisComponent::populateJsonObject( void *obj )
{
    pjs::Object *compObj = (pjs::Object *) obj;

    if( compObj == NULL )
        return;

    compObj->set( "id", m_id );
    compObj->set( "function", m_function );

    populateAxisComponentSpecificJson( obj );
}

void
CNCAxisComponent::eventFD( int fd )
{
    printf( "CNCAxisComponent::eventFD - default\n" );
}

CNCACOMP_RESULT_T
CNCAxisComponent::registerWithEventLoop( CNCEventLoop *loop )
{
    printf( "CNCAxisComponent::registerWithEventLoop - default\n" );

    return CNCACOMP_RESULT_SUCCESS;
}
