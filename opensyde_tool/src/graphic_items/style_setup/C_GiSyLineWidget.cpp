//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Widget for bus style setup to customize bus optics

   \copyright   Copyright 2016 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "precomp_headers.h"

#include <QGraphicsView>

#include "C_GiSyLineWidget.h"
#include "ui_C_GiSyLineWidget.h"

#include "C_HeHandler.h"
#include "C_GtGetText.h"
#include "C_OgePubColorOnly.h"
#include "C_UtiStyleSheets.h"
#include "C_GiLiLine.h"
#include "C_SdTopologyScene.h"
#include "C_GiSyColorSelectWidget.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */

using namespace stw_opensyde_gui;
using namespace stw_opensyde_gui_logic;
using namespace stw_opensyde_gui_elements;
using namespace stw_types;

/* -- Module Global Constants --------------------------------------------------------------------------------------- */
const QString mc_PATH_BACKGROUND_IMG = ":/images/graphic_items/TransparentBtnBackground.png";

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Module Global Variables --------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default constructor

   Set up GUI with all elements.

   \param[in]     oe_Mode     CAN-Bus / Ethernet-Bus or line
   \param[in,out] orc_Parent  Reference to parent
*/
//----------------------------------------------------------------------------------------------------------------------
C_GiSyLineWidget::C_GiSyLineWidget(const E_Type oe_Mode, C_GiSyBaseWidget & orc_Parent) :
   QWidget(&orc_Parent),
   mpc_Ui(new Ui::C_GiSyLineWidget),
   mpc_ParentDialog(&orc_Parent),
   me_Mode(oe_Mode)
{
   QPixmap c_Pic;

   c_Pic.load(mc_PATH_BACKGROUND_IMG);

   mpc_Ui->setupUi(this);

   //color
   //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
   this->mpc_LabelColor = new QLabel(dynamic_cast<QWidget *>(this->mpc_Ui->pc_BushButtonColor->parent()));
   this->mpc_LabelColor->setPixmap(c_Pic);
   this->mpc_LabelColor->lower();

   //color innerline
   //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
   this->mpc_LabelColorInner = new QLabel(dynamic_cast<QWidget *>(this->mpc_Ui->pc_BushButtonColorInner->parent()));
   this->mpc_LabelColorInner->setPixmap(c_Pic);
   this->mpc_LabelColorInner->lower();

   //Ui restriction
   this->mpc_Ui->pc_SpinBoxWidth->SetMinimumCustom(6);
   this->mpc_Ui->pc_SpinBoxWidth->SetMaximumCustom(25);

   // register the widget for showing
   this->mpc_ParentDialog->SetWidget(this);

   switch (this->me_Mode)
   {
   case eETHERNET_BUS:
      this->mpc_Ui->pc_LabelLineStyle->setVisible(false);
      this->mpc_Ui->pc_LabelLineStartArrow->setVisible(false);
      this->mpc_Ui->pc_LabelLineEndArrow->setVisible(false);
      this->mpc_Ui->pc_ComboBoxLineStyle->setVisible(false);
      this->mpc_Ui->pc_ComboBoxStartArrow->setVisible(false);
      this->mpc_Ui->pc_ComboBoxEndArrow->setVisible(false);
      break;
   case eCAN_BUS:
      this->mpc_Ui->pc_LabelLineStyle->setVisible(false);
      this->mpc_Ui->pc_LabelLineStartArrow->setVisible(false);
      this->mpc_Ui->pc_LabelLineEndArrow->setVisible(false);
      this->mpc_Ui->pc_ComboBoxLineStyle->setVisible(false);
      this->mpc_Ui->pc_ComboBoxStartArrow->setVisible(false);
      this->mpc_Ui->pc_ComboBoxEndArrow->setVisible(false);

      this->mpc_Ui->pc_BushButtonColorInner->setVisible(false);
      this->mpc_LabelColorInner->setVisible(false);
      this->mpc_Ui->pc_LabelColorInner->setVisible(false);
      break;
   case eLINE:
      this->mpc_Ui->pc_BushButtonColorInner->setVisible(false);
      this->mpc_LabelColorInner->setVisible(false);
      this->mpc_Ui->pc_LabelColorInner->setVisible(false);

      // remove the inner line to avoid a gap between the items
      this->mpc_Ui->pc_VerticalLayout->removeItem(this->mpc_Ui->pc_HorizontalLayoutColorInner);
      // add it again for memory management
      this->mpc_Ui->pc_VerticalLayout->addItem(this->mpc_Ui->pc_HorizontalLayoutColorInner);

      // a line can be thinner
      this->mpc_Ui->pc_SpinBoxWidth->SetMinimumCustom(1);
      break;
   default:
      break;
   }

   this->mc_Color = C_UtiStyleSheets::h_GetStyleSheetColor(this->mpc_Ui->pc_BushButtonColor->styleSheet());
   this->mc_InnerLineColor = C_UtiStyleSheets::h_GetStyleSheetColor(
      this->mpc_Ui->pc_BushButtonColorInner->styleSheet());

   this->m_UpdatePreview();

   InitStaticNames();

   connect(this->mpc_Ui->pc_BushButtonColor, &C_OgePubColorOnly::clicked, this,
           &C_GiSyLineWidget::m_ColorClicked);
   connect(this->mpc_Ui->pc_BushButtonColorInner, &C_OgePubColorOnly::clicked,
           this, &C_GiSyLineWidget::m_ColorInnerClicked);
   //lint -e{929} Cast required to avoid ambiguous signal of qt interface
   connect(this->mpc_Ui->pc_SpinBoxWidth, static_cast<void (QSpinBox::*)(sintn)>(&QSpinBox::valueChanged),
           this, &C_GiSyLineWidget::m_ValueChanged);
   //lint -e{929} Cast required to avoid ambiguous signal of qt interface
   connect(this->mpc_Ui->pc_ComboBoxLineStyle, static_cast<void (QComboBox::*)(sintn)>(&QComboBox::currentIndexChanged),
           this, &C_GiSyLineWidget::m_ValueChanged);
   //lint -e{929} Cast required to avoid ambiguous signal of qt interface
   connect(this->mpc_Ui->pc_ComboBoxStartArrow,
           static_cast<void (QComboBox::*)(sintn)>(&QComboBox::currentIndexChanged),
           this, &C_GiSyLineWidget::m_ValueChanged);
   //lint -e{929} Cast required to avoid ambiguous signal of qt interface
   connect(this->mpc_Ui->pc_ComboBoxEndArrow, static_cast<void (QComboBox::*)(sintn)>(&QComboBox::currentIndexChanged),
           this, &C_GiSyLineWidget::m_ValueChanged);

   mh_InitArrow(this->mpc_Ui->pc_ComboBoxStartArrow, true);
   mh_InitArrow(this->mpc_Ui->pc_ComboBoxEndArrow);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   default destructor

   Clean up.
*/
//----------------------------------------------------------------------------------------------------------------------
C_GiSyLineWidget::~C_GiSyLineWidget(void)
{
   delete mpc_Ui;
   //lint -e{1740}  no memory leak because the ownership of these objects was never transfered to this class
   // and Qt management takes care of the rest
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Initialize all displayed static names
*/
//----------------------------------------------------------------------------------------------------------------------
void C_GiSyLineWidget::InitStaticNames(void) const
{
   this->mpc_Ui->pc_LabelWidth->setText(C_GtGetText::h_GetText("Width"));
   this->mpc_Ui->pc_LabelColor->setText(C_GtGetText::h_GetText("Color"));
   this->mpc_Ui->pc_LabelColorInner->setText(C_GtGetText::h_GetText("Inner Line Color"));
   this->mpc_Ui->pc_LabelLineStyle->setText(C_GtGetText::h_GetText("Dash Type"));
   this->mpc_Ui->pc_LabelLineStartArrow->setText(C_GtGetText::h_GetText("Begin Type"));
   this->mpc_Ui->pc_LabelLineEndArrow->setText(C_GtGetText::h_GetText("End Type"));
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   On Show Event

   \param[in,out]    opc_Event   Event identification and information
*/
//----------------------------------------------------------------------------------------------------------------------
void C_GiSyLineWidget::showEvent(QShowEvent * const opc_Event)
{
   //color
   this->mpc_LabelColor->resize(this->mpc_Ui->pc_BushButtonColor->size());
   this->mpc_LabelColor->move(this->mpc_Ui->pc_BushButtonColor->pos());

   //color innerline
   this->mpc_LabelColorInner->resize(this->mpc_Ui->pc_BushButtonColorInner->size());
   this->mpc_LabelColorInner->move(this->mpc_Ui->pc_BushButtonColorInner->pos());

   QWidget::showEvent(opc_Event);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Get actual line width

   \return  Actual line width
*/
//----------------------------------------------------------------------------------------------------------------------
stw_types::sintn C_GiSyLineWidget::GetLineWidth(void) const
{
   return this->mpc_Ui->pc_SpinBoxWidth->value();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Set line width

   \param[in]  osn_Value   New line width
*/
//----------------------------------------------------------------------------------------------------------------------
void C_GiSyLineWidget::SetLineWidth(const stw_types::sintn osn_Value)
{
   this->mpc_Ui->pc_SpinBoxWidth->setValue(osn_Value);
   m_UpdatePreview();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Get actual color of line

   \return  Actual line color
*/
//----------------------------------------------------------------------------------------------------------------------
QColor C_GiSyLineWidget::GetLineColor(void) const
{
   return this->mc_Color;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Set line color

   \param[in]  orc_Value   New line color
*/
//----------------------------------------------------------------------------------------------------------------------
void C_GiSyLineWidget::SetLineColor(const QColor & orc_Value)
{
   QString c_Style = this->mpc_Ui->pc_BushButtonColor->styleSheet();

   this->mc_Color = orc_Value;

   // set the color of the button
   C_UtiStyleSheets::h_SetStyleSheetBackgroundColor(c_Style, orc_Value);
   this->mpc_Ui->pc_BushButtonColor->setStyleSheet(c_Style);
   this->m_UpdatePreview();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   GetInnerlineColor

   \return  Actual color of inner line
*/
//----------------------------------------------------------------------------------------------------------------------
QColor C_GiSyLineWidget::GetInnerLineColor(void) const
{
   return this->mc_InnerLineColor;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Set inner line color

   \param[in]  orc_Value   New inner line color
*/
//----------------------------------------------------------------------------------------------------------------------
void C_GiSyLineWidget::SetInnerLineColor(const QColor & orc_Value)
{
   QString c_Style = this->mpc_Ui->pc_BushButtonColorInner->styleSheet();

   this->mc_InnerLineColor = orc_Value;

   // set the color of the button
   C_UtiStyleSheets::h_SetStyleSheetBackgroundColor(c_Style, orc_Value);
   this->mpc_Ui->pc_BushButtonColorInner->setStyleSheet(c_Style);
   m_UpdatePreview();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Get line style

   \return  Actual style of line
*/
//----------------------------------------------------------------------------------------------------------------------
stw_opensyde_gui_logic::C_PuiBsLineArrow::E_LineType C_GiSyLineWidget::GetLineStyle(void) const
{
   return static_cast<stw_opensyde_gui_logic::C_PuiBsLineArrow::E_LineType>(
      this->mpc_Ui->pc_ComboBoxLineStyle->currentIndex());
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Set line style

   \param[in]  oe_Value   New style of line
*/
//----------------------------------------------------------------------------------------------------------------------
void C_GiSyLineWidget::SetLineStyle(const stw_opensyde_gui_logic::C_PuiBsLineArrow::E_LineType oe_Value)
{
   // set the state of the combo box
   this->mpc_Ui->pc_ComboBoxLineStyle->setCurrentIndex(static_cast<sintn>(oe_Value));

   m_UpdatePreview();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Get the arrow type on the start of the line

   \return  Actual start arrow of line
*/
//----------------------------------------------------------------------------------------------------------------------
stw_opensyde_gui_logic::C_PuiBsLineArrow::E_ArrowHeadType C_GiSyLineWidget::GetStartArrow(void) const
{
   return static_cast<stw_opensyde_gui_logic::C_PuiBsLineArrow::E_ArrowHeadType>(
      this->mpc_Ui->pc_ComboBoxStartArrow->currentIndex());
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Set the arrow type on the start of the line

   \param[in]  oe_Value   New start arrow of line
*/
//----------------------------------------------------------------------------------------------------------------------
void C_GiSyLineWidget::SetStartArrow(const stw_opensyde_gui_logic::C_PuiBsLineArrow::E_ArrowHeadType oe_Value)
{
   // set the state of the combo box
   this->mpc_Ui->pc_ComboBoxStartArrow->setCurrentIndex(static_cast<sintn>(oe_Value));

   m_UpdatePreview();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Get the arrow type on the end of the line

   \return  Actual end arrow of line
*/
//----------------------------------------------------------------------------------------------------------------------
stw_opensyde_gui_logic::C_PuiBsLineArrow::E_ArrowHeadType C_GiSyLineWidget::GetEndArrow(void) const
{
   return static_cast<stw_opensyde_gui_logic::C_PuiBsLineArrow::E_ArrowHeadType>(
      this->mpc_Ui->pc_ComboBoxEndArrow->currentIndex());
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Set the arrow type on the end of the line

   \param[in]  oe_Value   New end arrow of line
*/
//----------------------------------------------------------------------------------------------------------------------
void C_GiSyLineWidget::SetEndArrow(const stw_opensyde_gui_logic::C_PuiBsLineArrow::E_ArrowHeadType oe_Value)
{
   // set the state of the combo box
   this->mpc_Ui->pc_ComboBoxEndArrow->setCurrentIndex(static_cast<sintn>(oe_Value));

   m_UpdatePreview();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Update of preview click
*/
//----------------------------------------------------------------------------------------------------------------------
void C_GiSyLineWidget::m_UpdatePreview(void)
{
   const QSize c_ViewSize = C_GiSyBaseWidget::h_GetSceneViewSize();
   const float64 f64_PlacementOffset = 25.0;

   std::vector<QPointF> c_Points;

   // clear old scene
   this->mpc_ParentDialog->GetPreviewScene()->clear();

   if ((this->me_Mode == eCAN_BUS) ||
       (this->me_Mode == eETHERNET_BUS))
   {
      C_GiLiLine * pc_Item;

      // Main line
      c_Points.push_back(QPointF(f64_PlacementOffset, 70.0));
      c_Points.push_back(QPointF(c_ViewSize.width() - f64_PlacementOffset, 70.0));

      if (me_Mode == eETHERNET_BUS)
      {
         pc_Item = new C_GiLiLine(&c_Points, true);
      }
      else
      {
         pc_Item = new C_GiLiLine(&c_Points);
      }

      pc_Item->SetColor(this->mc_Color);
      pc_Item->SetWidth(this->mpc_Ui->pc_SpinBoxWidth->value());
      pc_Item->SetMiddleLineColor(this->mc_InnerLineColor);

      this->mpc_ParentDialog->GetPreviewScene()->addItem(pc_Item);

      // Connection line 1
      c_Points.clear();
      c_Points.push_back(QPointF(142.0, 71.0));
      c_Points.push_back(QPointF(142.0, 110.0));
      if (me_Mode == eETHERNET_BUS)
      {
         //lint -e{423}  no memory leak because of adding the item to the scene and the Qt memory management
         pc_Item = new C_GiLiLine(&c_Points, true);
      }
      else
      {
         //lint -e{423}  no memory leak because of adding the item to the scene and the Qt memory management
         pc_Item = new C_GiLiLine(&c_Points);
      }
      pc_Item->SetColor(this->mc_Color);
      pc_Item->SetWidth(static_cast<sint32>(this->mpc_Ui->pc_SpinBoxWidth->value() / 2));
      pc_Item->SetMiddleLineColor(this->mc_InnerLineColor);
      pc_Item->setFlag(QGraphicsItem::ItemStacksBehindParent);
      this->mpc_ParentDialog->GetPreviewScene()->addItem(pc_Item);
      //lint -e{429}  no memory leak because of adding the item to the scene and the Qt memory management
   }
   else
   {
      C_GiBiArrow * pc_Arrow;
      C_PuiBsLineArrow::E_ArrowHeadType e_ArrowHeadType = this->GetStartArrow();
      float64 f64_XOffset = 0.0;
      if (C_GiBiArrow::h_HasOffsetInteractionPoint(e_ArrowHeadType) == true)
      {
         f64_XOffset = this->mpc_Ui->pc_SpinBoxWidth->value() * 2.0;
      }

      c_Points.push_back(QPointF(f64_PlacementOffset + f64_XOffset, c_ViewSize.height() / 2.0));
      e_ArrowHeadType = this->GetEndArrow();
      if (C_GiBiArrow::h_HasOffsetInteractionPoint(e_ArrowHeadType) == true)
      {
         f64_XOffset = this->mpc_Ui->pc_SpinBoxWidth->value() * 2.0;
      }
      c_Points.push_back(QPointF((static_cast<float64>(c_ViewSize.width()) - f64_PlacementOffset) - f64_XOffset,
                                 c_ViewSize.height() / 2.0));

      pc_Arrow = new C_GiBiArrow(0ULL, &c_Points);

      pc_Arrow->SetColor(this->mc_Color);
      pc_Arrow->SetWidth(this->mpc_Ui->pc_SpinBoxWidth->value());

      pc_Arrow->SetLineType(this->GetLineStyle());
      pc_Arrow->SetStartArrowHeadType(this->GetStartArrow());
      pc_Arrow->SetEndArrowHeadType(this->GetEndArrow());

      this->mpc_ParentDialog->GetPreviewScene()->addItem(pc_Arrow);
      //lint -e{429}  no memory leak because of adding the item to the scene and the Qt memory management
   }

   //refresh the graphicsview
   if (this->mpc_ParentDialog->GetPreviewScene()->views().size() > 0)
   {
      //lint -e{1793} Access necessary for view update
      this->mpc_ParentDialog->GetPreviewScene()->views()[0] -> update();
   }

   this->mpc_ParentDialog->GetPreviewScene()->clearSelection();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Slot of Color button

   Offers the possibility to choose a color for the line
*/
//----------------------------------------------------------------------------------------------------------------------
void C_GiSyLineWidget::m_ColorClicked(void)
{
   QString c_Style = this->mpc_Ui->pc_BushButtonColor->styleSheet();
   // get the old color as initial color
   QColor c_Color = C_UtiStyleSheets::h_GetStyleSheetColor(c_Style);

   QPointer<C_OgePopUpDialog> const c_Popup = new C_OgePopUpDialog(this, this);
   C_GiSyColorSelectWidget * const pc_ColorWidget = new C_GiSyColorSelectWidget(* c_Popup, c_Color);

   //Resize
   c_Popup->SetSize(QSize(412, 620));

   if (c_Popup->exec() == static_cast<sintn>(QDialog::Accepted))
   {
      c_Color = pc_ColorWidget->ChooseSelectedColor();

      if (c_Color.isValid() == true)
      {
         // save the color
         this->mc_Color = c_Color;

         // update the button
         C_UtiStyleSheets::h_SetStyleSheetBackgroundColor(c_Style, c_Color);
         this->mpc_Ui->pc_BushButtonColor->setStyleSheet(c_Style);

         this->m_UpdatePreview();
      }
   }

   if (c_Popup != NULL)
   {
      c_Popup->HideOverlay();
   }
   //lint -e{429}  no memory leak because of the parent of pc_Dialog and the Qt memory management
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Slot of inner color button

   Offers the possibility to choose a color for the inner line
*/
//----------------------------------------------------------------------------------------------------------------------
void C_GiSyLineWidget::m_ColorInnerClicked(void)
{
   QString c_Style = this->mpc_Ui->pc_BushButtonColorInner->styleSheet();
   // get the old color as initial color
   QColor c_Color = C_UtiStyleSheets::h_GetStyleSheetColor(c_Style);

   QPointer<C_OgePopUpDialog> const c_Popup = new C_OgePopUpDialog(this, this);
   C_GiSyColorSelectWidget * const pc_ColorWidget = new C_GiSyColorSelectWidget(*c_Popup, c_Color);

   //Resize
   c_Popup->SetSize(QSize(412, 620));

   if (c_Popup->exec() == static_cast<sintn>(QDialog::Accepted))
   {
      c_Color = pc_ColorWidget->ChooseSelectedColor();

      if (c_Color.isValid() == true)
      {
         // save the color
         this->mc_InnerLineColor = c_Color;

         // update the button
         C_UtiStyleSheets::h_SetStyleSheetBackgroundColor(c_Style, c_Color);
         this->mpc_Ui->pc_BushButtonColorInner->setStyleSheet(c_Style);

         this->m_UpdatePreview();
      }
   }

   if (c_Popup != NULL)
   {
      c_Popup->HideOverlay();
   }
   //lint -e{429}  no memory leak because of the parent of pc_Dialog and the Qt memory management
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Slot of spin box changed
*/
//----------------------------------------------------------------------------------------------------------------------
void C_GiSyLineWidget::m_ValueChanged(void)
{
   this->m_UpdatePreview();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Init any arrow selection box

   \param[in,out] opc_ComboBox ComboBox to initialize with arrows
   \param[in]     orq_Rotate   Flag if content should be rotated 180 degrees
*/
//----------------------------------------------------------------------------------------------------------------------
void C_GiSyLineWidget::mh_InitArrow(C_OgeCbxIconOnly * const opc_ComboBox, const bool & orq_Rotate)
{
   if (opc_ComboBox != NULL)
   {
      QPixmap c_Img1(":/images/graphic_items/LineArrowHeadType1.png");
      QPixmap c_Img3(":/images/graphic_items/LineArrowHeadType2.png");
      QPixmap c_Img5(":/images/graphic_items/LineArrowHeadType3.png");
      QPixmap c_Img7(":/images/graphic_items/LineArrowHeadType4.png");
      QPixmap c_Img9(":/images/graphic_items/LineArrowHeadType5.png");
      QPixmap c_Img11(":/images/graphic_items/LineStyleSolid.png");

      QIcon c_Icon0;
      QIcon c_Icon1;
      QIcon c_Icon2;
      QIcon c_Icon3;
      QIcon c_Icon4;
      QIcon c_Icon5;

      if (orq_Rotate == true)
      {
         //Rotate
         QTransform c_Transform;

         c_Transform = c_Transform.rotate(180.0);
         c_Img1 = c_Img1.transformed(c_Transform);
         c_Img3 = c_Img3.transformed(c_Transform);
         c_Img5 = c_Img5.transformed(c_Transform);
         c_Img7 = c_Img7.transformed(c_Transform);
         c_Img9 = c_Img9.transformed(c_Transform);
      }

      c_Icon0.addPixmap(c_Img11, QIcon::Normal);
      c_Icon1.addPixmap(c_Img1, QIcon::Normal);
      c_Icon2.addPixmap(c_Img3, QIcon::Normal);
      c_Icon3.addPixmap(c_Img5, QIcon::Normal);
      c_Icon4.addPixmap(c_Img7, QIcon::Normal);
      c_Icon5.addPixmap(c_Img9, QIcon::Normal);

      opc_ComboBox->clear();
      opc_ComboBox->addItem(c_Icon0, "");
      opc_ComboBox->addItem(c_Icon1, "");
      opc_ComboBox->addItem(c_Icon2, "");
      opc_ComboBox->addItem(c_Icon3, "");
      opc_ComboBox->addItem(c_Icon4, "");
      opc_ComboBox->addItem(c_Icon5, "");
   }
}
