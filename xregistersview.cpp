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
#include "xregistersview.h"

XRegistersView::XRegistersView(QWidget *pParent) : XShortcutstScrollArea(pParent)
{
    setFont(XAbstractTableView::getMonoFont(10));

    g_nCharWidth=XAbstractTableView::getCharWidth(this);
    g_nCharHeight=XAbstractTableView::getCharHeight(this);

//    addColumn("",300); // TODO Width
//    setVerticalLinesVisible(true);
////    setHorisontalLinesVisible(true);
//    setHeaderVisible(false);
//    setLineDelta(0);
}

void XRegistersView::setMode(XBinary::DM disasmMode)
{
    g_listRegions.clear();

    if(disasmMode==XBinary::DM_X86_32)
    {
        qint32 nLeft=0;
        qint32 nTop=0;

        qint32 nTitleWidth=g_nCharWidth*3;
        qint32 nValueWidth=g_nCharWidth*8;
        qint32 nCommentWidth=nValueWidth;

        QList<QString> listGeneralRegs;
        listGeneralRegs.append("EAX");
        listGeneralRegs.append("ECX");
        listGeneralRegs.append("EDX");
        listGeneralRegs.append("EBX");
        listGeneralRegs.append("EBP");
        listGeneralRegs.append("ESP");
        listGeneralRegs.append("ESI");
        listGeneralRegs.append("EDI");

        addRegsList(&listGeneralRegs,nLeft,nTop,nTitleWidth,nValueWidth,nCommentWidth,XBinary::MODE_32);

        nTop+=listGeneralRegs.count()*g_nCharHeight;
        nTop+=3; // TODO const

        addRegion("EIP",
                  nLeft,
                  nTop,
                  nTitleWidth,
                  nValueWidth,
                  nCommentWidth,
                  XBinary::MODE_32);

        nTop+=g_nCharHeight;

        addRegion("EFLAGS",
                  nLeft,
                  nTop,
                  g_nCharWidth*6,
                  nValueWidth,
                  nCommentWidth,
                  XBinary::MODE_32);
    }
    else if(disasmMode==XBinary::DM_X86_64)
    {
        qint32 nLeft=0;
        qint32 nTop=0;

        qint32 nTitleWidth=g_nCharWidth*3;
        qint32 nValueWidth=g_nCharWidth*12;
        qint32 nCommentWidth=nValueWidth;

        QList<QString> listGeneralRegs;
        listGeneralRegs.append("RAX");
        listGeneralRegs.append("RCX");
        listGeneralRegs.append("RDX");
        listGeneralRegs.append("RBX");
        listGeneralRegs.append("RBP");
        listGeneralRegs.append("RSP");
        listGeneralRegs.append("RSI");
        listGeneralRegs.append("RDI");
        listGeneralRegs.append("R8");
        listGeneralRegs.append("R9");
        listGeneralRegs.append("R10");
        listGeneralRegs.append("R11");
        listGeneralRegs.append("R12");
        listGeneralRegs.append("R13");
        listGeneralRegs.append("R14");
        listGeneralRegs.append("R15");

        addRegsList(&listGeneralRegs,nLeft,nTop,nTitleWidth,nValueWidth,nCommentWidth,XBinary::MODE_64);

        nTop+=listGeneralRegs.count()*g_nCharHeight;
        nTop+=3; // TODO const

        addRegion("RIP",
                  nLeft,
                  nTop,
                  nTitleWidth,
                  nValueWidth,
                  nCommentWidth,
                  XBinary::MODE_64);

        nTop+=g_nCharHeight;

        addRegion("EFLAGS",
                  nLeft,
                  nTop,
                  g_nCharWidth*6,
                  nValueWidth,
                  nCommentWidth,
                  XBinary::MODE_32);
    }
}

void XRegistersView::setData(QMap<QString, QVariant> *pMapRegisters)
{
//    stChanged.clear();

    QMapIterator<QString,QVariant> iter(*pMapRegisters);
    while(iter.hasNext())
    {
        iter.next();
        QVariant varValue=iter.value();
        QString sKey=iter.key();

        if(g_mapRegisters.value(sKey)!=varValue)
        {
            g_stChanged.insert(sKey);
        }
        else
        {
            g_stChanged.remove(sKey);
        }
    }

    g_mapRegisters=*pMapRegisters;

    viewport()->update();
}

void XRegistersView::clear()
{
#ifdef QT_DEBUG
    qDebug("void XRegistersView::clear()");
#endif
}

void XRegistersView::addRegion(QString sTitle, qint32 nLeft, qint32 nTop, qint32 nTitleWidth, qint32 nValueWidth, qint32 nCommentWidth, XBinary::MODE mode)
{
    REGION region={};

    region.sTitle=sTitle;
    region.nLeft=nLeft;
    region.nTop=nTop;
    region.nTitleWidth=nTitleWidth;
    region.nValueWidth=nValueWidth;
    region.nCommentWidth=nCommentWidth;
    region.mode=mode;

    g_listRegions.append(region);
}

void XRegistersView::addRegsList(QList<QString> *pRegsList, qint32 nLeft, qint32 nTop, qint32 nTitleWidth, qint32 nValueWidth, qint32 nCommentWidth, XBinary::MODE mode)
{
    int nNumberOfRegs=pRegsList->count();

    for(int i=0;i<nNumberOfRegs;i++)
    {
        addRegion(pRegsList->at(i),
                  nLeft,
                  nTop+i*g_nCharHeight,
                  nTitleWidth,
                  nValueWidth,
                  nCommentWidth,
                  mode);
    }
}

void XRegistersView::paintEvent(QPaintEvent *pEvent)
{
    QPainter *pPainter=new QPainter(this->viewport());
    pPainter->setPen(viewport()->palette().color(QPalette::WindowText));
    pPainter->setBackgroundMode(Qt::TransparentMode);

    int nNumberOfRegions=g_listRegions.count();

    for(int i=0;i<nNumberOfRegions;i++)
    {
        // TODO grey titles
        QString sTitle=g_listRegions.at(i).sTitle;
        bool bChanged=g_stChanged.contains(sTitle);
        qint32 nTop=g_listRegions.at(i).nTop;
        qint32 nLeft=g_listRegions.at(i).nLeft;

        pPainter->drawText(nLeft,nTop,sTitle); // TODO Text Optional

        if(bChanged)
        {
            pPainter->save();
            pPainter->setPen(QColor(Qt::red));
        }

        pPainter->drawText(nLeft+g_listRegions.at(i).nTitleWidth,nTop,XBinary::valueToHex(g_listRegions.at(i).mode,g_mapRegisters.value(sTitle).toULongLong())); // TODO Text Optional

        if(bChanged)
        {
            pPainter->restore();
        }
        // TODO Comment
    }

    delete pPainter;
}

void XRegistersView::paintCell(QPainter *pPainter, qint32 nRow, qint32 nColumn, qint32 nLeft, qint32 nTop, qint32 nWidth, qint32 nHeight)
{
    qint32 nIndex=0;

    if(nIndex<g_listRegions.count())
    {
        // TODO grey titles
        QString sTitle=g_listRegions.at(nIndex).sTitle;
        bool bChanged=g_stChanged.contains(sTitle);

        pPainter->drawText(nLeft,nTop+nHeight,sTitle); // TODO Text Optional

        if(bChanged)
        {
            pPainter->save();
            pPainter->setPen(QColor(Qt::red));
        }

        pPainter->drawText(nLeft+g_listRegions.at(nIndex).nTitleWidth,nTop+nHeight,XBinary::valueToHex(g_listRegions.at(nIndex).mode,g_mapRegisters.value(sTitle).toULongLong())); // TODO Text Optional

        if(bChanged)
        {
            pPainter->restore();
        }
        // TODO Comment
    }
}

void XRegistersView::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}
