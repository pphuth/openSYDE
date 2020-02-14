//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Base class for all graphic items which are rectangle based (header)

   \copyright   Copyright 2016 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_GIBIRECTBASEGROUP_H
#define C_GIBIRECTBASEGROUP_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include <QGraphicsItemGroup>
#include <QSizeF>

#include "stwtypes.h"

#include "C_PuiBsBox.h"
#include "C_GiUnique.h"
#include "C_GiBiBase.h"
#include "C_GiBiSizeableItem.h"
#include "C_GiPointInteraction.h"
#include "C_GiBiCustomMouseItem.h"
#include "C_GiBiConnectableItem.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace stw_opensyde_gui
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_GiBiRectBaseGroup :
   public C_GiBiConnectableItem,
   public stw_opensyde_gui_logic::C_GiUnique,
   public C_GiBiCustomMouseItem,
   public C_GiBiBase,
   public QGraphicsItemGroup
{
   Q_OBJECT

public:
   C_GiBiRectBaseGroup(const stw_types::uint64 & oru64_ID, const stw_types::float64 of64_MinWidth,
                       const stw_types::float64 of64_MinHeight, const stw_types::float64 of64_ActionPointOffset,
                       const bool oq_KeepAspectRatio, QGraphicsItem * const opc_Parent = NULL,
                       const QPointF & orc_PosOffset = QPointF(-1.0, -1.0));
   virtual ~C_GiBiRectBaseGroup();

   virtual void RestoreDefaultCursor(void) override;
   virtual void SetTemporaryCursor(const QCursor & orc_TemporaryCursor) override;
   virtual void SetDefaultCursor(const QCursor & orc_Value) override;

   // The naming of the Qt parameters can't be changed and are not compliant with the naming conventions,
   // and default parameters are identical.
   //lint -save -e1960 -e1735
   virtual void paint(QPainter * const opc_Painter, const QStyleOptionGraphicsItem * const opc_Option,
                      QWidget * const opc_Widget = NULL) override;
   //lint -restore
   virtual QRectF boundingRect() const;
   QRectF GetVisibleBoundingRect() const;
   virtual void FindClosestPoint(const QPointF & orc_ScenePoint, QPointF & orc_Closest) const override;

   //GI base
   virtual void SetZValueCustom(const stw_types::float64 of64_ZValue) override;

   void SetResizing(const bool oq_Active);
   QSizeF GetSize(void) const;

   void LoadBasicData(const stw_opensyde_gui_logic::C_PuiBsBox & orc_Data);
   void UpdateBasicData(stw_opensyde_gui_logic::C_PuiBsBox & orc_Data) const;
   void ApplySizeChange(const QPointF & orc_NewPos, const QSizeF & orc_NewSize);
   virtual void CopyStyle(const QGraphicsItem * const opc_GuidelineItem);
   virtual void UpdateTransform(const QTransform & orc_Transform);
   //The signals keyword is necessary for Qt signal slot functionality
   //lint -save -e1736

Q_SIGNALS:
   //lint -restore
   void SigSelectionChange(const bool & orq_State);
   void SigItemWasMoved(const QPointF & orc_PositionDifference);
   void SigItemWasResized(const QPointF & orc_OldPos, const QSizeF & orc_OldSize, const QPointF & orc_NewPos,
                          const QSizeF & orc_NewSize);

protected:
   // The naming of the Qt parameters can't be changed and are not compliant with the naming conventions
   //lint -save -e1960
   virtual QVariant itemChange(const GraphicsItemChange oe_Change, const QVariant & orc_Value) override;
   virtual void mousePressEvent(QGraphicsSceneMouseEvent * const opc_Event) override;
   virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * const opc_Event) override;
   virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * const opc_Event) override;

   virtual bool sceneEventFilter(QGraphicsItem * const opc_Watched, QEvent * const opc_Event) override;
   //lint -restore

   void m_SetBiggestItem(C_GiBiSizeableItem & orc_Item);
   virtual void m_ResizeUpdateItems(const stw_types::float64 of64_DiffWidth,
                                    const stw_types::float64 of64_DiffHeight) = 0;

   void m_BiggestItemChanged(void);
   void m_BlockMoveAndResize(void);

   stw_types::float64 m_GetInteractionPointSceneWidth(void) const;

private:
   //Avoid call
   C_GiBiRectBaseGroup(const C_GiBiRectBaseGroup &);
   C_GiBiRectBaseGroup & operator =(const C_GiBiRectBaseGroup &);

   void m_InitActionPoints(void);
   void m_UpdateActionPoints(void);
   QRectF m_GetBiggestSubItemBoundingRect(void) const;
   void m_SetAdaptedPos(const QPointF & orc_Pos);

   QVector<C_GiPointInteraction *> mc_ActionPoints;
   bool mq_ResizingActive;
   stw_types::sintn msn_ActiveResizeMode;
   QRectF mc_ShowBoundingRect;
   C_GiBiSizeableItem * mpc_BiggestSubItem;
   stw_types::float64 mf64_AspectRatio;
   QPointF mc_LastKnownPosition;
   QSizeF mc_LastKnownSize;
   bool mq_BlockMoveAndResize;

   const bool mq_KeepAspectRatio;
   const stw_types::float64 mf64_ActionPointOffset;
   const stw_types::float64 mf64_MinWidth;
   const stw_types::float64 mf64_MinHeight;
   const QPointF mc_PosOffset;
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
