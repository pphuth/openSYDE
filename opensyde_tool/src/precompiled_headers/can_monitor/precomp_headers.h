//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Precompiled header (header)

   To use this precompiled-header in a Qt project add the following to your .pro file:

   CONFIG   += precompile_header
   PRECOMPILED_HEADER = ../src/precomp_headers.h

   Contains a list of header files to be pre-compiled.
   see http://doc.qt.io/qt-5/qmake-precompiledheaders.html for details

   \copyright   Copyright 2016 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef  PRECOMP_HEADERS_GUI_H
#define  PRECOMP_HEADERS_GUI_H

//lint -esym(766,"precomp_headers.h")   effectively not used in lint "builds"; but that's exactly what we want
#ifndef _lint //speed up linting: don't include all of the headers for each linted .cpp file

/* -- Includes ------------------------------------------------------------------------------------------------------ */

/* Add C includes here */

#if defined __cplusplus

/* Add C++ includes here */
#include <vector>
#include <cmath>
#include <cstddef>
#include <cstring>
#include <iostream>
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif

// Qt includes
#include <QAction>
#include <QApplication>
#include <QCheckBox>
#include <QColor>
#include <QComboBox>
#include <QCursor>
#include <QDialog>
#include <QEvent>
#include <QFile>
#include <QFont>
#include <QFrame>
#include <QGraphicsItem>
#include <QGroupBox>
#include <QLabel>
#include <QList>
#include <QListWidget>
#include <QMenu>
#include <QMouseEvent>
#include <QObject>
#include <QPainter>
#include <QPoint>
#include <QPointF>
#include <QPushButton>
#include <QSize>
#include <QString>
#include <QTimer>
#include <QUndoCommand>
#include <QUndoStack>
#include <QVariant>
#include <QVector>
#include <QWidget>
/*
//Qt modules
#include <QtGui>
#include <QtCore>
#include <QtWidgets>

// STW includes
#include "stwtypes.h"
#include "C_PuiProject.h"
#include "C_PuiSdHandler.h"
#include "C_SdTopologyScene.h"
#include "C_GtGetText.h"
#include "CSCLChecksums.h"
*/
#endif

#endif

#endif
