#ifndef __HN_CNC_DEVICE_PRIVATE_H__
#define __HN_CNC_DEVICE_PRIVATE_H__

#include <string>
#include <vector>

#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/OptionSet.h"

#include <hnode2/HNodeDevice.h>
#include <hnode2/HNodeConfig.h>
#include <hnode2/HNEPLoop.h>
#include <hnode2/HNReqWaitQueue.h>

#include "CNCEventLoop.h"
#include "HNCNCAction.h"
#include "CmdSequence.h"
#include "CNCMachine.h"
#include "UIM342Cmd.h"
#include "UIM342Machines.h"

#define HNODE_CNC_DEVTYPE   "hnode2-cnc-device"

typedef enum HNCNCDeviceResultEnum
{
  HNCNC_RESULT_SUCCESS,
  HNCNC_RESULT_FAILURE,
  HNCNC_RESULT_BAD_REQUEST,
  HNCNC_RESULT_SERVER_ERROR
}HNCNC_RESULT_T;

typedef enum HNIDStartActionBitsEnum
{
    HNID_ACTBIT_CLEAR     = 0x0000,
    HNID_ACTBIT_COMPLETE  = 0x0001,
    HNID_ACTBIT_UPDATE    = 0x0002,
//    HNID_ACTBIT_RECALCSCH = 0x0004,
    HNID_ACTBIT_ERROR     = 0x0008,
//    HNID_ACTBIT_SENDREQ   = 0x0010
} HNID_ACTBIT_T;

class HNCNCDevice : public Poco::Util::ServerApplication, public HNDEPDispatchInf, public HNDEventNotifyInf, public CNCEventCB, public CNCMachineEventsCB
{
    private:
        bool _helpRequested   = false;
        bool _debugLogging    = false;
        bool _instancePresent = false;

        std::string _instance; 
        std::string m_instanceName;

        HNodeDevice m_hnodeDev;

        CNCEventFD *m_configUpdateTrigger;

        CNCEventLoop m_eventLoop;

        // Format string codes
        uint m_errStrCode;
        uint m_noteStrCode;

        // Health component ids
        std::string m_hc1ID;
        std::string m_hc2ID;
        std::string m_hc3ID;

        // Keep track of health state change simulation
        uint m_healthStateSeq;

        HNCNCAction    *m_curUserAction;
        HNReqWaitQueue  m_userActionQueue;

        CmdSeqParameters  m_cmdParams;
        CNCMachine       *m_curMachine;

        void displayHelp();

        bool configExists();
        HNCNC_RESULT_T initConfig();
        HNCNC_RESULT_T readConfig();
        HNCNC_RESULT_T updateConfig();

        void generateNewHealthState();

        void startAction();

    protected:
        // HNDevice REST callback
        virtual void dispatchEP( HNodeDevice *parent, HNOperationData *opData );

        // Handle trigger events
        virtual void eventFD( int fd );

        // Notification for hnode device config changes.
        virtual void hndnConfigChange( HNodeDevice *parent );

        // Event loop functions
        //virtual void loopIteration();
        //virtual void timeoutEvent();
        //virtual void fdEvent( int sfd );
        //virtual void fdError( int sfd );

        // Poco funcions
        void defineOptions( Poco::Util::OptionSet& options );
        void handleOption( const std::string& name, const std::string& value );
        int main( const std::vector<std::string>& args );

        virtual void sequenceComplete();
};

#endif // __HN_CNC_DEVICE_PRIVATE_H__
