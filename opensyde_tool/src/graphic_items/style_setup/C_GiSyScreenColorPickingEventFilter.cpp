//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Event filter to be installed on the color-select-widget while in screen-color-picking mode.

   While the user is in screen-color-picking mode one of three event filters can be use. The user can move
   the mouse over the screen, can release the mouse button to select the current color on screen or can press
   a key on the keyboard.

   \copyright   Copyright 2019 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "precomp_headers.h"

#include "C_GiSyScreenColorPickingEventFilter.h"
#include "C_GiSyColorSelectWidget.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */
using namespace stw_types;
using namespace stw_opensyde_gui;

/* -- Module Global Constants --------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Module Global Variables --------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default constructor

   Set up the screen color picking event filters.

   \param[in,out]   opc_Parent               Pointer to parent
   \param[in,out]   opc_ColorSelectWidget    Pointer to widget
*/
//----------------------------------------------------------------------------------------------------------------------
C_GiSyScreenColorPickingEventFilter::C_GiSyScreenColorPickingEventFilter(
   C_GiSyColorSelectWidget * const opc_ColorSelectWidget, QObject * const opc_Parent) :
   QObject(opc_Parent),
   mpc_ColorSelectWidget(opc_ColorSelectWidget)
{
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default destructor
*/
//----------------------------------------------------------------------------------------------------------------------
C_GiSyScreenColorPickingEventFilter::~C_GiSyScreenColorPickingEventFilter(void)
{
   delete this->mpc_ColorSelectWidget;
}
//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Choose this event which is used

   \param[in] opc_Object   Pointer unused
   \param[in, out] opc_Event    Event identification and information

   \return
   bool   if one event was chosen
*/
//----------------------------------------------------------------------------------------------------------------------
bool C_GiSyScreenColorPickingEventFilter::eventFilter(QObject * const opc_Object, QEvent * const opc_Event)
{
   Q_UNUSED(opc_Object)
   //lint -save -e929  false positive in PC-Lint: allowed by MISRA 5-2-7
   switch (opc_Event->type())
   {
   case QEvent::MouseMove:
      return mpc_ColorSelectWidget->HandleColorPickingMouseMove(dynamic_cast<QMouseEvent *>(opc_Event));
      break;
   case QEvent::MouseButtonRelease:
      return mpc_ColorSelectWidget->HandleColorPickingMouseButtonRelease(dynamic_cast<QMouseEvent *>(opc_Event));
      break;
   default:
      break;
   }
   //lint -restore
   return false;
}
