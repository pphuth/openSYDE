//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Dialog for choosing the CAN communication interface (Socket CAN)

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_SYVSECANCONFIGURATIONDIALOG_H
#define C_SYVSECANCONFIGURATIONDIALOG_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include <QWidget>

#include "stwtypes.h"

#include "C_PuiSvPc.h"
#include "C_OgePopUpDialog.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */

namespace Ui
{
class C_SyvSeCanConfigurationDialog;
}

namespace stw_opensyde_gui
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_SyvSeCanConfigurationDialog :
   public QWidget
{
   Q_OBJECT

public:
   explicit C_SyvSeCanConfigurationDialog(stw_opensyde_gui_elements::C_OgePopUpDialog & orc_Parent);
   ~C_SyvSeCanConfigurationDialog();

   void InitText() const;
   void SetDllType(const stw_opensyde_gui_logic::C_PuiSvPc::E_CANDllType oe_Type) const;
   void SetCustomDllPath(const QString & orc_Path);
   void SetBitrate(const stw_types::uint64 ou64_Bitrate);
   stw_opensyde_gui_logic::C_PuiSvPc::E_CANDllType GetDllType(void) const;
   QString GetCustomDllPath(void) const;

protected:
   // The naming of the Qt parameters can't be changed and are not compliant with the naming conventions
   //lint -save -e1960
   virtual void keyPressEvent(QKeyEvent * const opc_KeyEvent) override;
   //lint -restore

private:
   //Avoid call
   C_SyvSeCanConfigurationDialog(const C_SyvSeCanConfigurationDialog &);
   C_SyvSeCanConfigurationDialog & operator =(const C_SyvSeCanConfigurationDialog &);

   void m_OkClicked(void);
   void m_CancelClicked(void) const;

   void m_GetInterfaceNames(QStringList & orc_Interfaces);

   Ui::C_SyvSeCanConfigurationDialog * mpc_Ui;
   //lint -e{1725} Only problematic if copy or assignment is allowed
   stw_opensyde_gui_elements::C_OgePopUpDialog & mrc_ParentDialog;

   stw_types::uint64 mu64_Bitrate;
   QString mc_InterfaceName;
};
}

#endif
