//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Widget for all displaying and interacting with manually configured messages (header)

   See cpp file for detailed description

   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_CAMGENMESSAGESWIDGET_H
#define C_CAMGENMESSAGESWIDGET_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "C_OgeWiOnlyBackground.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace Ui
{
class C_CamGenMessagesWidget;
}

namespace stw_opensyde_gui
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_CamGenMessagesWidget :
   public stw_opensyde_gui_elements::C_OgeWiOnlyBackground
{
   Q_OBJECT

public:
   explicit C_CamGenMessagesWidget(QWidget * const opc_Parent = NULL);
   ~C_CamGenMessagesWidget(void);

   void InitStaticNames(void) const;
   void SaveUserSettings(void) const;
   void LoadUserSettings(void) const;
   void RemoveMessagesForFile(const QString & orc_File) const;
   void SetCommunicationStarted(const bool oq_Online);
   bool CheckAndHandleKey(const QString & orc_Input) const;
   void UpdateMessageData(const stw_types::uint32 ou32_MessageIndex) const;
   void TriggerModelUpdateCyclicMessage(const stw_types::uint32 ou32_MessageIndex, const bool oq_Active) const;

   //The signals keyword is necessary for Qt signal slot functionality
   //lint -save -e1736

Q_SIGNALS:
   //lint -restore
   void SigRegisterCyclicMessage(const stw_types::uint32 ou32_MessageIndex, const bool oq_Active);
   void SigRemoveAllCyclicMessages (void);
   void SigSendMessage(const stw_types::uint32 ou32_MessageIndex, const stw_types::uint32 ou32_TimeToSend);
   void SigUpdateMessageDLC(const stw_types::uint32 ou32_MessageIndex);
   void SigSelected(const stw_types::uint32 ou32_NumSelectedItems, const stw_types::uint32 ou32_Row);

protected:
   // The naming of the Qt parameters can't be changed and are not compliant with the naming conventions
   //lint -save -e1960
   virtual void resizeEvent(QResizeEvent * const opc_Event) override;
   //lint -restore

private:
   Ui::C_CamGenMessagesWidget * mpc_Ui;

   void m_InitButtons(void) const;
   void m_UpdateHeading(void) const;
   void m_LoadConfig(void) const;
   void m_OnItemCountChanged(const stw_types::uint32 ou32_NewItemCount) const;
   void m_OnTransmissionToggled(const bool oq_Active);

   //Avoid call
   C_CamGenMessagesWidget(const C_CamGenMessagesWidget &);
   C_CamGenMessagesWidget & operator =(const C_CamGenMessagesWidget &);
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
