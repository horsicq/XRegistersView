/* Copyright (c) 2021-2022 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef XREGISTERSVIEW_H
#define XREGISTERSVIEW_H

#include <QMap>
#include <QObject>
#include <QRect>
#include <QVariant>
#include <QWidget>
#include "xabstracttableview.h"
#include "xformats.h"
#include "xabstractdebugger.h"

class XRegistersView : public XShortcutstScrollArea
{
    Q_OBJECT

    struct REGION
    {
        QString sTitle;
        qint32 nLeft;
        qint32 nTop;
        qint32 nTitleWidth;
        qint32 nValueWidth;
        qint32 nCommentWidth;
        qint32 nHeight;
    };

public:

    struct OPTIONS
    {
        XBinary::DM disasmMode;
        XProcess::REG_OPTIONS regOptions;
    };

    XRegistersView(QWidget *pParent=nullptr);
    void setOptions(XProcess::REG_OPTIONS regOptions);
    XProcess::REG_OPTIONS getOptions();
    void setData(XProcess::REGISTERS *pRegisters);
    void clear();
    virtual void adjustView() override;

private:
    void addRegion(QString sTitle,qint32 nLeft,qint32 nTop,qint32 nTitleWidth,qint32 nValueWidth,qint32 nCommentWidth);
    void addRegsList(QList<QString> *pRegsList,qint32 nLeft,qint32 nTop,qint32 nTitleWidth,qint32 nValueWidth,qint32 nCommentWidth);
    void addMapValue(QMap<QString,XBinary::XVARIANT> *pMap,QString sName,quint64 nValue);
    void addMapValue(QMap<QString,XBinary::XVARIANT> *pMap,QString sName,quint32 nValue);
    void addMapValue(QMap<QString,XBinary::XVARIANT> *pMap,QString sName,quint16 nValue);
    void addMapValue(QMap<QString,XBinary::XVARIANT> *pMap,QString sName,quint8 nValue);
    void addMapValue(QMap<QString,XBinary::XVARIANT> *pMap,QString sName,bool bValue);

protected:
    virtual void paintEvent(QPaintEvent* pEvent) override;

private slots:
    void _customContextMenu(const QPoint &pos);

public slots:
    void actionViewGeneral();
    void actionViewIP();
    void actionViewFlags();
    void actionViewSegments();
    void actionViewDebug();
    void actionViewFloat();
    void actionViewXMM();

protected:
    virtual void paintCell(QPainter *pPainter,qint32 nRow,qint32 nColumn,qint32 nLeft,qint32 nTop,qint32 nWidth,qint32 nHeight);
    virtual void registerShortcuts(bool bState) override;
    virtual void contextMenu(const QPoint &pos);

private:
    bool g_bActive;
    XProcess::REGISTERS *g_pRegisters;
    QMap<QString,XBinary::XVARIANT> g_mapRegisters;
    QList<REGION> g_listRegions;
    QSet<QString> g_stChanged;
    qint32 g_nCharWidth;
    qint32 g_nCharHeight;
    XProcess::REG_OPTIONS g_regOptions;
};

#endif // XREGISTERSVIEW_H
