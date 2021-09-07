// copyright (c) 2021 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#ifndef XREGISTERSVIEW_H
#define XREGISTERSVIEW_H

#include <QObject>
#include <QWidget>
#include <QMap>
#include <QRect>
#include <QVariant>
#include "xabstracttableview.h"
#include "xformats.h"

class XRegistersView : public XShortcutstScrollArea // TODO no table !!!
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
        XBinary::MODE mode;
    };

public:
    XRegistersView(QWidget *pParent=nullptr);
    void setMode(XBinary::DM disasmMode);
    void setData(QMap<QString, QVariant> *pMapRegisters);
    void clear();

private:
    void addRegion(QString sTitle,qint32 nLeft,qint32 nTop,qint32 nTitleWidth,qint32 nValueWidth,qint32 nCommentWidth,XBinary::MODE mode);
    void addRegsList(QList<QString> *pRegsList,qint32 nLeft,qint32 nTop,qint32 nTitleWidth,qint32 nValueWidth,qint32 nCommentWidth,XBinary::MODE mode);

protected:
    virtual void paintEvent(QPaintEvent* pEvent) override;

protected:
    virtual void paintCell(QPainter *pPainter,qint32 nRow,qint32 nColumn,qint32 nLeft,qint32 nTop,qint32 nWidth,qint32 nHeight);
    virtual void registerShortcuts(bool bState);

private:
    QMap<QString, QVariant> g_mapRegisters;
    QList<REGION> g_listRegions;
    QSet<QString> g_stChanged;
    qint32 g_nCharWidth;
    qint32 g_nCharHeight;
};

#endif // XREGISTERSVIEW_H
