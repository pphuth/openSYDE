//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Help engine handler (header)

   See cpp file for detailed description

   \copyright   Copyright 2016 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_HEHANDLER_H
#define C_HEHANDLER_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include <QWidget>
#include <QString>
#include <QMap>
#include "stwtypes.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace stw_opensyde_gui_logic
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */
class C_HeHandler
{
public:
   static C_HeHandler & GetInstance(void);
   void CallSpecificHelpPage(const QString & orc_ClassName);
   static bool CheckHelpKey(const QKeyEvent * const opc_KeyEvent);
   void SetHelpFileRelPath(const QString & orc_RelPath);

private:
   C_HeHandler();
   static C_HeHandler mhc_Instance;
   virtual ~C_HeHandler();
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
