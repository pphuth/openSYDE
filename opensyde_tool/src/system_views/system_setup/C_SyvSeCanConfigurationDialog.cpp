//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Dialog for choosing the CAN communication interface

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#include "precomp_headers.h"

#include <sys/types.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>

#include <QFile>
#include <QFileInfo>
#include <QFileDialog>

#include "stwtypes.h"
#include "stwerrors.h"

#include "C_SyvSeCanConfigurationDialog.h"
#include "ui_C_SyvSeCanConfigurationDialog.h"

#include "C_OSCUtils.h"
#include "C_Uti.h"
#include "C_GtGetText.h"
#include "CCAN.h"
#include "C_OgeWiCustomMessage.h"
#include "C_ImpUtil.h"
#include "C_PuiUtil.h"
#include "C_OgeWiUtil.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */
using namespace stw_types;
using namespace stw_errors;
using namespace stw_opensyde_gui;
using namespace stw_opensyde_gui_logic;
using namespace stw_opensyde_gui_elements;
using namespace stw_opensyde_core;
using namespace stw_can;

/* -- Module Global Constants --------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Module Global Variables --------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default constructor

   Set up GUI with all elements.

   \param[in,out] opc_parent Optional pointer to parent
*/
//----------------------------------------------------------------------------------------------------------------------
C_SyvSeCanConfigurationDialog::C_SyvSeCanConfigurationDialog(stw_opensyde_gui_elements::C_OgePopUpDialog & orc_Parent) :
   QWidget(&orc_Parent),
   mpc_Ui(new Ui::C_SyvSeCanConfigurationDialog),
   mrc_ParentDialog(orc_Parent),
   mu64_Bitrate(0U)
{
   QStringList c_Interfaces;

   mpc_Ui->setupUi(this);

   this->mrc_ParentDialog.SetWidget(this);

   this->InitText();

   this->m_GetInterfaceNames(c_Interfaces);
   this->mpc_Ui->pc_ComboBoxCanInterfaces->addItems(c_Interfaces);

   // connects
   connect(this->mpc_Ui->pc_BushButtonOk, &QPushButton::clicked,
           this, &C_SyvSeCanConfigurationDialog::m_OkClicked);
   connect(this->mpc_Ui->pc_BushButtonCancel, &QPushButton::clicked,
           this, &C_SyvSeCanConfigurationDialog::m_CancelClicked);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   default destructor

   Clean up.
*/
//----------------------------------------------------------------------------------------------------------------------
C_SyvSeCanConfigurationDialog::~C_SyvSeCanConfigurationDialog()
{
   delete mpc_Ui;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Initializes all visible strings on the widget
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvSeCanConfigurationDialog::InitText(void) const
{
   this->mrc_ParentDialog.SetTitle(C_GtGetText::h_GetText("PC CAN Interface"));
   this->mrc_ParentDialog.SetSubTitle(C_GtGetText::h_GetText("Configuration"));

   this->mpc_Ui->pc_LabelBusHeading->setText(C_GtGetText::h_GetText("Select Interface"));
   this->mpc_Ui->pc_BushButtonOk->setText(C_GtGetText::h_GetText("OK"));
   this->mpc_Ui->pc_BushButtonCancel->setText(C_GtGetText::h_GetText("Cancel"));
   this->mpc_Ui->pc_LabelBitrateInfo->setText(C_GtGetText::h_GetText(
                                                 "The openSYDE PC tool does not change the CAN bitrate since this is a system-wide setting."));
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Sets the CAN DLL type.

   \param[in]     oe_Type       CAN DLL type
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvSeCanConfigurationDialog::SetDllType(const C_PuiSvPc::E_CANDllType oe_Type) const
{
   // We ignore the dll type for the SocketCAN interface
   (void)oe_Type;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Sets the custom CAN DLL path.

   \param[in]     orc_Path       Path of the CAN DLL
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvSeCanConfigurationDialog::SetCustomDllPath(const QString & orc_Path)
{
   sintn sn_Index;

   // We missuse the dll path for the interface name
   this->mc_InterfaceName = orc_Path;

   // Select the preset interface
   sn_Index = this->mpc_Ui->pc_ComboBoxCanInterfaces->findText(this->mc_InterfaceName);
   if (sn_Index >= 0)
   {
      this->mpc_Ui->pc_ComboBoxCanInterfaces->setCurrentIndex(sn_Index);
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Sets the bitrate for the test connection in bit/s

   \param[in]     ou64_Bitrate     Bitrate for test connection
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvSeCanConfigurationDialog::SetBitrate(const uint64 ou64_Bitrate)
{
   this->mu64_Bitrate = ou64_Bitrate;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Get CAN DLL type.

   \return     CAN DLL type (PEAK/VECTOR/Other)
*/
//----------------------------------------------------------------------------------------------------------------------
C_PuiSvPc::E_CANDllType C_SyvSeCanConfigurationDialog::GetDllType(void) const
{
   return C_PuiSvPc::E_CANDllType::eOTHER;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Get custom CAN DLL path.

   \return     path of custom CAN DLL
*/
//----------------------------------------------------------------------------------------------------------------------
QString C_SyvSeCanConfigurationDialog::GetCustomDllPath(void) const
{
   return mc_InterfaceName;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Overwritten key press event slot

   Here: Handle specific enter key cases

   \param[in,out] opc_KeyEvent Event identification and information
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvSeCanConfigurationDialog::keyPressEvent(QKeyEvent * const opc_KeyEvent)
{
   bool q_CallOrg = true;

   //Handle all enter key cases manually
   if ((opc_KeyEvent->key() == static_cast<sintn>(Qt::Key_Enter)) ||
       (opc_KeyEvent->key() == static_cast<sintn>(Qt::Key_Return)))
   {
      if (((opc_KeyEvent->modifiers().testFlag(Qt::ControlModifier) == true) &&
           (opc_KeyEvent->modifiers().testFlag(Qt::AltModifier) == false)) &&
          (opc_KeyEvent->modifiers().testFlag(Qt::ShiftModifier) == false))
      {
         this->m_OkClicked();
      }
      else
      {
         q_CallOrg = false;
      }
   }
   if (q_CallOrg == true)
   {
      QWidget::keyPressEvent(opc_KeyEvent);
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Slot of OK button click
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvSeCanConfigurationDialog::m_OkClicked(void)
{
   this->mc_InterfaceName = this->mpc_Ui->pc_ComboBoxCanInterfaces->currentText();
   this->mrc_ParentDialog.accept();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Slot of Cancel button
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvSeCanConfigurationDialog::m_CancelClicked(void) const
{
   this->mrc_ParentDialog.reject();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Get a list of all CAN interfaces of the system

   \param[out] orc_Interfaces    List of CAN interfaces
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvSeCanConfigurationDialog::m_GetInterfaceNames(QStringList & orc_Interfaces)
{
   sintn sn_Ret;
   struct ifaddrs * pt_InterfaceList;
   struct ifaddrs * pt_If;

   orc_Interfaces.clear();

   sn_Ret = getifaddrs(&pt_InterfaceList);
   if (sn_Ret == 0)
   {
      pt_If = pt_InterfaceList;

      while (pt_If != NULL)
      {
         sintn sn_Socket = -1;
         struct sockaddr_can t_Addr;
         struct ifreq t_Ifr;

         // Try to bind a socket to the interface to see if it is a CAN inteface
         sn_Socket = socket(AF_CAN, SOCK_RAW, CAN_RAW);
         sn_Ret = (sn_Socket < 0) ? -1 : 0;

         if (sn_Ret == 0)
         {
            // Get interface index
            strcpy(t_Ifr.ifr_name, pt_If->ifa_name);
            sn_Ret = ioctl(sn_Socket, SIOCGIFINDEX, &t_Ifr);
         }

         if (sn_Ret == 0)
         {
            t_Addr.can_family = AF_CAN;
            t_Addr.can_ifindex = t_Ifr.ifr_ifindex;
            sn_Ret = bind(sn_Socket, (struct sockaddr*)&t_Addr, sizeof(t_Addr));
         }

         if (sn_Ret == 0)
         {
            orc_Interfaces << pt_If->ifa_name;
         }

         if (sn_Socket >= 0)
         {
            ::close(sn_Socket);
         }

         pt_If = pt_If->ifa_next;
      }

      freeifaddrs(pt_InterfaceList);
   }
}