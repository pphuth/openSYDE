//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Device definition data container

   See .cpp file for full description

   \copyright   Copyright 2016 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_OSCDEVICEDEFINITIONH
#define C_OSCDEVICEDEFINITIONH

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include <vector>

#include "stwtypes.h"
#include "CSCLString.h"
#include "C_OSCSystemBus.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace stw_opensyde_core
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

///container for definition of one openSYDE device
class C_OSCDeviceDefinition
{
public:
   C_OSCDeviceDefinition(void);
   //lint -sem(stw_opensyde_core::C_OSCDeviceDefinition::Clear,initializer)
   void Clear(void);

   bool IsUpdateAvailable(const C_OSCSystemBus::E_Type oe_Type) const;
   bool IsRoutingAvailable(const C_OSCSystemBus::E_Type oe_Type) const;
   bool IsDiagnosisAvailable(const C_OSCSystemBus::E_Type oe_Type) const;
   stw_scl::C_SCLString GetDisplayName(void) const;

   stw_scl::C_SCLString c_DeviceName;                      ///< full device name used for checks
   stw_scl::C_SCLString c_DeviceNameAlias;                 ///< full displayed name of device
   std::vector<stw_scl::C_SCLString> c_OtherAcceptedNames; ///< Other compatible names for this device
   stw_scl::C_SCLString c_DeviceDescription;               ///< text describing device
   stw_scl::C_SCLString c_ImagePath;                       ///< path to file with image of device (e.g. JPG)
   stw_scl::C_SCLString c_FilePath;                        ///< used for service update package (#24474)
   stw_types::uint8 u8_NumCanBusses;                       ///< number of CAN busses present on device
   stw_types::uint8 u8_NumEthernetBusses;                  ///< number of ethernet interfaces present on device

   ///is programming supported enabled?
   bool q_ProgrammingSupport;

   std::vector<stw_types::uint16> c_SupportedBitrates; ///< supported CAN bitrates in kbit/s

   ///is the KEFEX protocol supported (on CAN bus) ?
   bool q_DiagnosticProtocolKefex;
   ///is the openSYDE protocol supported on CAN bus ?
   bool q_DiagnosticProtocolOpenSydeCan;
   ///is the openSYDE protocol supported on ethernet ?
   bool q_DiagnosticProtocolOpenSydeEthernet;

   ///is the STW flashloader available (on CAN bus) ?
   bool q_FlashloaderStwCan;
   ///is the openSYDE flashloader available on CAN bus ?
   bool q_FlashloaderOpenSydeCan;
   ///is the openSYDE flashloader available on ethernet ?
   bool q_FlashloaderOpenSydeEthernet;
   ///is the device file based or address based?
   bool q_FlashloaderOpenSydeIsFileBased;

   ///minimal times in ms to reset from the application to the Flashloader or reset the Flashloader itself.
   ///possible scenario: Reset from Application to Flashloader or from Flashloader to Flashloader
   ///no communication relevant configuration was changed.
   stw_types::uint32 u32_FlashloaderResetWaitTimeNoChangesCan;      ///<CAN is used. default time is 1000ms
   stw_types::uint32 u32_FlashloaderResetWaitTimeNoChangesEthernet; ///<Eth is used. default time is 5500ms

   ///Single scenario: From Flashloader to Flashloader
   ///no fundamental communication relevant configuration was changed (Bus-Id or Node Id).
   stw_types::uint32 u32_FlashloaderResetWaitTimeNoFundamentalChangesCan;      ///<CAN is used. default time is 1000ms
   stw_types::uint32 u32_FlashloaderResetWaitTimeNoFundamentalChangesEthernet; ///<Eth is used. default time is 5500ms

   /// Single scenario: From Flashloader to Flashloader
   ///fundamental communication relevant configuration was changed (CAN bitrate or IP address).
   stw_types::uint32 u32_FlashloaderResetWaitTimeFundamentalChangesCan;      ///<CAN is used. default time is 1000ms
   stw_types::uint32 u32_FlashloaderResetWaitTimeFundamentalChangesEthernet; ///<Eth is used. default time is 5500ms

   ///the maximum time in ms it can take to erase one continuous area in flash
   stw_types::uint32 u32_FlashloaderOpenSydeRequestDownloadTimeout;
   ///the maximum time in ms it can take to write up to 4kB of data to flash
   stw_types::uint32 u32_FlashloaderOpenSydeTransferDataTimeout;

   ///size of EEPROM memory available to the user application in bytes
   stw_types::uint32 u32_UserEepromSizeBytes;
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
}

#endif
