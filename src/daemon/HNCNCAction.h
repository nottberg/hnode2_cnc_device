#ifndef __HNCNC_ACTION_H__
#define __HNCNC_ACTION_H__

#include <stdint.h>

#include <string>
#include <mutex>
#include <thread>
#include <sstream>

#include <hnode2/HNReqWaitQueue.h>

#include "CNCMachine.h"

typedef enum HNCNCActionType 
{
    HNCNC_ATYPE_NOTSET                = 0,
    HNCNC_ATYPE_GET_STATUS            = 1,
    HNCNC_ATYPE_GET_MACHINE_DESC      = 2,
    HNCNC_ATYPE_GET_SEQ_DEF_LIST      = 3,
    HNCNC_ATYPE_GET_SEQ_DEF_INFO      = 4,
    HNCNC_ATYPE_ENQUEUE_SEQUENCE      = 5,
    HNCNC_ATYPE_GET_ACTIVE_SEQUENCES  = 6,
    HNCNC_ATYPE_GET_SEQUENCE_INFO     = 7,
    HNCNC_ATYPE_CANCEL_SEQUENCE       = 8
}HNCNC_ATYPE_T;

#if 0
typedef enum HNSDActionZoneUpdateMaskEnum
{
    HNSD_ZU_FLDMASK_CLEAR  = 0x00000000,
    HNSD_ZU_FLDMASK_NAME   = 0x00000001,
    HNSD_ZU_FLDMASK_DESC   = 0x00000002,
    HNSD_ZU_FLDMASK_SPW    = 0x00000004,
    HNSD_ZU_FLDMASK_MAXSPC = 0x00000008,
    HNSD_ZU_FLDMASK_MINSPC = 0x00000010,
    HNSD_ZU_FLDMASK_SWLST  = 0x00000020
}HNSD_ZU_FLDMASK_T;
#endif

typedef enum HNCNCActionResult
{
    HNCNC_ARESULT_SUCCESS,
    HNCNC_ARESULT_FAILURE
}HNCNC_ARESULT_T;

class HNCNCAction : public HNReqWaitAction
{
    public:
        HNCNCAction();
       ~HNCNCAction();

        void setType( HNCNC_ATYPE_T type );

        void setNewID( std::string id );
        
        bool decodeStartCapture( std::istream& bodyStream );

        void setRequestCaptureID( std::string id );
        std::string getRequestCaptureID();

        void setSeqDefID( std::string id );
        std::string getSeqDefID();

        HNCNC_ATYPE_T getType();

        bool hasNewObject( std::string &newID );
        bool hasRspContent( std::string &contentType );
        bool generateRspContent( std::ostream &ostr );

        void setResponseJSON( std::string jsonStr );

    private:
        HNCNC_ATYPE_T  m_type;

        std::string m_newID;

        std::string m_reqCaptureID;

        std::string m_seqDefID;

        std::string m_rspStr;

        std::stringstream m_rspStream;
};

#endif // __HNCNC_ACTION_H__