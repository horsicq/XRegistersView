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
#include "xregistersview.h"

XRegistersView::XRegistersView(QWidget *pParent) : XShortcutstScrollArea(pParent)
{
    g_bActive=false;

    setFont(XAbstractTableView::getMonoFont(10)); // TODO options

    g_nCharWidth=XAbstractTableView::getCharWidth(this);
    g_nCharHeight=XAbstractTableView::getCharHeight(this);

//    addColumn("",300); // TODO Width
//    setVerticalLinesVisible(true);
////    setHorisontalLinesVisible(true);
//    setHeaderVisible(false);
//    setLineDelta(0);

}

void XRegistersView::setOptions(XAbstractDebugger::REG_OPTIONS regOptions)
{
    g_regOptions=regOptions;
}

void XRegistersView::setData(XAbstractDebugger::REGISTERS *pRegisters)
{
    g_pRegisters=pRegisters;

    QMap<QString,XBinary::XVARIANT> mapRegisters;
#ifdef Q_PROCESSOR_X86_32
    addMapValue(&mapRegisters,"EAX",pRegisters->EAX);
    addMapValue(&mapRegisters,"EBX",pRegisters->EBX);
    addMapValue(&mapRegisters,"ECX",pRegisters->ECX);
    addMapValue(&mapRegisters,"EDX",pRegisters->EDX);
    addMapValue(&mapRegisters,"EBP",pRegisters->EBP);
    addMapValue(&mapRegisters,"ESP",pRegisters->ESP);
    addMapValue(&mapRegisters,"ESI",pRegisters->ESI);
    addMapValue(&mapRegisters,"EDI",pRegisters->EDI);
    addMapValue(&mapRegisters,"EIP",pRegisters->EIP);
#endif
#ifdef Q_PROCESSOR_X86_64
    addMapValue(&mapRegisters,"RAX",pRegisters->RAX);
    addMapValue(&mapRegisters,"RBX",pRegisters->RBX);
    addMapValue(&mapRegisters,"RCX",pRegisters->RCX);
    addMapValue(&mapRegisters,"RDX",pRegisters->RDX);
    addMapValue(&mapRegisters,"RBP",pRegisters->RBP);
    addMapValue(&mapRegisters,"RSP",pRegisters->RSP);
    addMapValue(&mapRegisters,"RSI",pRegisters->RSI);
    addMapValue(&mapRegisters,"RDI",pRegisters->RDI);
    addMapValue(&mapRegisters,"R8",pRegisters->R8);
    addMapValue(&mapRegisters,"R9",pRegisters->R9);
    addMapValue(&mapRegisters,"R10",pRegisters->R10);
    addMapValue(&mapRegisters,"R11",pRegisters->R11);
    addMapValue(&mapRegisters,"R12",pRegisters->R12);
    addMapValue(&mapRegisters,"R13",pRegisters->R13);
    addMapValue(&mapRegisters,"R14",pRegisters->R14);
    addMapValue(&mapRegisters,"R15",pRegisters->R15);
    addMapValue(&mapRegisters,"RIP",pRegisters->RIP);
#endif
#ifdef Q_PROCESSOR_X86
    addMapValue(&mapRegisters,"EFLAGS",pRegisters->EFLAGS);
    addMapValue(&mapRegisters,"CF",(bool)((pRegisters->EFLAGS)&0x0001));
    addMapValue(&mapRegisters,"PF",(bool)((pRegisters->EFLAGS)&0x0004));
    addMapValue(&mapRegisters,"AF",(bool)((pRegisters->EFLAGS)&0x0010));
    addMapValue(&mapRegisters,"ZF",(bool)((pRegisters->EFLAGS)&0x0040));
    addMapValue(&mapRegisters,"SF",(bool)((pRegisters->EFLAGS)&0x0080));
    addMapValue(&mapRegisters,"TF",(bool)((pRegisters->EFLAGS)&0x0100));
    addMapValue(&mapRegisters,"IF",(bool)((pRegisters->EFLAGS)&0x0200));
    addMapValue(&mapRegisters,"DF",(bool)((pRegisters->EFLAGS)&0x0400));
    addMapValue(&mapRegisters,"OF",(bool)((pRegisters->EFLAGS)&0x0800));
    addMapValue(&mapRegisters,"GS",pRegisters->GS);
    addMapValue(&mapRegisters,"FS",pRegisters->FS);
    addMapValue(&mapRegisters,"ES",pRegisters->ES);
    addMapValue(&mapRegisters,"DS",pRegisters->DS);
    addMapValue(&mapRegisters,"CS",pRegisters->CS);
    addMapValue(&mapRegisters,"SS",pRegisters->SS);
    addMapValue(&mapRegisters,"DR0",pRegisters->DR[0]);
    addMapValue(&mapRegisters,"DR1",pRegisters->DR[1]);
    addMapValue(&mapRegisters,"DR2",pRegisters->DR[2]);
    addMapValue(&mapRegisters,"DR3",pRegisters->DR[3]);
    addMapValue(&mapRegisters,"DR6",pRegisters->DR[6]);
    addMapValue(&mapRegisters,"DR7",pRegisters->DR[7]);
#endif

    QMapIterator<QString,XBinary::XVARIANT> iter(mapRegisters);

    while(iter.hasNext())
    {
        iter.next();
        XBinary::XVARIANT varValue=iter.value();
        QString sKey=iter.key();

        if( (g_mapRegisters.value(sKey).var.v_uint128.low!=varValue.var.v_uint128.low)||
            (g_mapRegisters.value(sKey).var.v_uint128.high!=varValue.var.v_uint128.high))
        {
            g_stChanged.insert(sKey);
        }
        else
        {
            g_stChanged.remove(sKey);
        }
    }

    g_mapRegisters=mapRegisters;

    g_bActive=true;

    adjustView();

    viewport()->update();
}

void XRegistersView::clear()
{
    g_bActive=false;
#ifdef QT_DEBUG
    qDebug("void XRegistersView::clear()");
#endif

    adjustView();

    viewport()->update();
}

void XRegistersView::adjustView()
{
    g_listRegions.clear();

    qint32 nMinWidth=0;
    qint32 nMinHeight=0;

    if(g_bActive)
    {
        qint32 nLeft=5;
        qint32 nTop=0;

        qint32 nValueWidthBit=g_nCharWidth*1;
        qint32 nValueWidth32=g_nCharWidth*8;
        qint32 nValueWidth64=g_nCharWidth*12;
        qint32 nValueWidth128=g_nCharWidth*20; // Check
        qint32 nCommentWidth=nValueWidth32;

        bool bFirst=false;

        nTop+=g_nCharHeight;

        if(g_regOptions.bGeneral)
        {
            bFirst=true;

        #ifdef Q_PROCESSOR_X86_32
            QList<QString> listGeneralRegs;
            listGeneralRegs.append("EAX");
            listGeneralRegs.append("EBX");
            listGeneralRegs.append("ECX");
            listGeneralRegs.append("EDX");
            listGeneralRegs.append("EBP");
            listGeneralRegs.append("ESP");
            listGeneralRegs.append("ESI");
            listGeneralRegs.append("EDI");

            addRegsList(&listGeneralRegs,nLeft,nTop,g_nCharWidth*3,nValueWidth32,nCommentWidth);

            nTop+=listGeneralRegs.count()*g_nCharHeight;
        #endif
        #ifdef Q_PROCESSOR_X86_64
            QList<QString> listGeneralRegs;
            listGeneralRegs.append("RAX");
            listGeneralRegs.append("RBX");
            listGeneralRegs.append("RCX");
            listGeneralRegs.append("RDX");
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

            addRegsList(&listGeneralRegs,nLeft,nTop,g_nCharWidth*3,nValueWidth64,nCommentWidth);

            nTop+=listGeneralRegs.count()*g_nCharHeight;
        #endif
        }

        if(g_regOptions.bIP)
        {
            if(bFirst)
            {
                nTop+=g_nCharHeight/2; // Empty
            }

            bFirst=true;

        #ifdef Q_PROCESSOR_X86_32
            addRegion("EIP",
                      nLeft,
                      nTop,
                      g_nCharWidth*3,
                      nValueWidth32,
                      nCommentWidth);
        #endif
        #ifdef Q_PROCESSOR_X86_64
            addRegion("RIP",
                      nLeft,
                      nTop,
                      g_nCharWidth*3,
                      nValueWidth64,
                      nCommentWidth);
        #endif

            nTop+=g_nCharHeight;
        }

        if(g_regOptions.bFlags)
        {
            if(bFirst)
            {
                nTop+=g_nCharHeight/2; // Empty
            }

            bFirst=true;

            addRegion("EFLAGS",
                      nLeft,
                      nTop,
                      g_nCharWidth*6,
                      nValueWidth32,
                      nCommentWidth);
            nTop+=g_nCharHeight;

            QList<QString> listRegs1;
            listRegs1.append("ZF");
            listRegs1.append("OF");
            listRegs1.append("CF");
            addRegsList(&listRegs1,nLeft,nTop,g_nCharWidth*2,nValueWidthBit,0);

            QList<QString> listRegs2;
            listRegs2.append("ZF");
            listRegs2.append("OF");
            listRegs2.append("CF");
            addRegsList(&listRegs2,nLeft+g_nCharWidth*4,nTop,g_nCharWidth*2,nValueWidthBit,0);

            QList<QString> listRegs3;
            listRegs3.append("AF");
            listRegs3.append("DF");
            listRegs3.append("IF");
            addRegsList(&listRegs3,nLeft+g_nCharWidth*8,nTop,g_nCharWidth*2,nValueWidthBit,0);

            nTop+=(3)*g_nCharHeight;
        }

        if(g_regOptions.bSegments)
        {
            if(bFirst)
            {
                nTop+=g_nCharHeight/2; // Empty
            }

            bFirst=true;

            QList<QString> listRegs1;
            listRegs1.append("GS");
            listRegs1.append("ES");
            listRegs1.append("CS");
            addRegsList(&listRegs1,nLeft,nTop,g_nCharWidth*2,nValueWidthBit,0);

            QList<QString> listRegs2;
            listRegs2.append("FS");
            listRegs2.append("DS");
            listRegs2.append("SS");
            addRegsList(&listRegs2,nLeft+g_nCharWidth*6,nTop,g_nCharWidth*2,nValueWidthBit,0);

            nTop+=(3)*g_nCharHeight;
        }

        if(g_regOptions.bFloat)
        {
            if(bFirst)
            {
                nTop+=g_nCharHeight/2; // Empty
            }

            bFirst=true;

            QList<QString> listXmmFloat;
            listXmmFloat.append("ST0");
            listXmmFloat.append("ST1");
            listXmmFloat.append("ST2");
            listXmmFloat.append("ST3");
            listXmmFloat.append("ST4");
            listXmmFloat.append("ST5");
            listXmmFloat.append("ST6");
            listXmmFloat.append("ST7");

            addRegsList(&listXmmFloat,nLeft,nTop,g_nCharWidth*3,nValueWidth128,nCommentWidth);

            nTop+=listXmmFloat.count()*g_nCharHeight;

            // TODO tagWord
            // TODO StatusWord
            // ControlWord
        }

        if(g_regOptions.bSegments)
        {
            if(bFirst)
            {
                nTop+=g_nCharHeight/2; // Empty
            }

            bFirst=true;

            QList<QString> listDebugRegs;
            listDebugRegs.append("DR0");
            listDebugRegs.append("DR1");
            listDebugRegs.append("DR2");
            listDebugRegs.append("DR3");
            listDebugRegs.append("DR6");
            listDebugRegs.append("DR7");

        #ifdef Q_PROCESSOR_X86_32
            addRegsList(&listDebugRegs,nLeft,nTop,g_nCharWidth*3,nValueWidth32,nCommentWidth);
        #endif
        #ifdef Q_PROCESSOR_X86_64
            addRegsList(&listDebugRegs,nLeft,nTop,g_nCharWidth*3,nValueWidth64,nCommentWidth);
        #endif
            nTop+=listDebugRegs.count()*g_nCharHeight;
        }

        if(g_regOptions.bXMM)
        {
            if(bFirst)
            {
                nTop+=g_nCharHeight/2; // Empty
            }

            bFirst=true;

            QList<QString> listXmmRegs;
            listXmmRegs.append("XMM0");
            listXmmRegs.append("XMM1");
            listXmmRegs.append("XMM2");
            listXmmRegs.append("XMM3");
            listXmmRegs.append("XMM4");
            listXmmRegs.append("XMM5");
            listXmmRegs.append("XMM6");
            listXmmRegs.append("XMM7");
            listXmmRegs.append("XMM8");
            listXmmRegs.append("XMM9");
            listXmmRegs.append("XMM10");
            listXmmRegs.append("XMM11");
            listXmmRegs.append("XMM12");
            listXmmRegs.append("XMM13");
            listXmmRegs.append("XMM14");
            listXmmRegs.append("XMM15");

            addRegsList(&listXmmRegs,nLeft,nTop,g_nCharWidth*5,nValueWidth128,nCommentWidth);

            nTop+=listXmmRegs.count()*g_nCharHeight;

            // TODO MxCsr
        }

        qint32 nNumberOfRegions=g_listRegions.count();

        for(qint32 i=0;i<nNumberOfRegions;i++)
        {
            nMinWidth=qMax(nMinWidth,g_listRegions.at(i).nLeft+g_listRegions.at(i).nTitleWidth+g_listRegions.at(i).nValueWidth+g_listRegions.at(i).nCommentWidth);
            nMinHeight=qMax(nMinHeight,g_listRegions.at(i).nTop+g_listRegions.at(i).nHeight);
        }
    }

    setMinimumWidth(nMinWidth);
    setMinimumHeight(nMinHeight);
}

void XRegistersView::addRegion(QString sTitle,qint32 nLeft,qint32 nTop,qint32 nTitleWidth,qint32 nValueWidth,qint32 nCommentWidth)
{
    REGION region={};

    region.sTitle=sTitle;
    region.nLeft=nLeft;
    region.nTop=nTop;
    region.nTitleWidth=nTitleWidth;
    region.nValueWidth=nValueWidth;
    region.nCommentWidth=nCommentWidth;
    region.nHeight=g_nCharHeight;

    g_listRegions.append(region);
}

void XRegistersView::addRegsList(QList<QString> *pRegsList,qint32 nLeft,qint32 nTop,qint32 nTitleWidth,qint32 nValueWidth,qint32 nCommentWidth)
{
    qint32 nNumberOfRegs=pRegsList->count();

    for(qint32 i=0;i<nNumberOfRegs;i++)
    {
        addRegion(pRegsList->at(i),
                  nLeft,
                  nTop+i*g_nCharHeight,
                  nTitleWidth,
                  nValueWidth,
                  nCommentWidth);
    }
}

void XRegistersView::addMapValue(QMap<QString,XBinary::XVARIANT> *pMap,QString sName,quint64 nValue)
{
    XBinary::XVARIANT xVariant={};
    xVariant.bIsBigEndian=false;
    xVariant.mode=XBinary::MODE_64;
    xVariant.var.v_uint64=nValue;
    pMap->insert(sName,xVariant);
}

void XRegistersView::addMapValue(QMap<QString,XBinary::XVARIANT> *pMap,QString sName,quint32 nValue)
{
    XBinary::XVARIANT xVariant={};
    xVariant.bIsBigEndian=false;
    xVariant.mode=XBinary::MODE_32;
    xVariant.var.v_uint32=nValue;
    pMap->insert(sName,xVariant);
}

void XRegistersView::addMapValue(QMap<QString,XBinary::XVARIANT> *pMap,QString sName,quint16 nValue)
{
    XBinary::XVARIANT xVariant={};
    xVariant.bIsBigEndian=false;
    xVariant.mode=XBinary::MODE_16;
    xVariant.var.v_uint16=nValue;
    pMap->insert(sName,xVariant);
}

void XRegistersView::addMapValue(QMap<QString, XBinary::XVARIANT> *pMap, QString sName, quint8 nValue)
{
    XBinary::XVARIANT xVariant={};
    xVariant.bIsBigEndian=false;
    xVariant.mode=XBinary::MODE_8;
    xVariant.var.v_uint8=nValue;
    pMap->insert(sName,xVariant);
}

void XRegistersView::addMapValue(QMap<QString, XBinary::XVARIANT> *pMap, QString sName, bool bValue)
{
    XBinary::XVARIANT xVariant={};
    xVariant.bIsBigEndian=false;
    xVariant.mode=XBinary::MODE_BIT;
    xVariant.var.v_bool=bValue;
    pMap->insert(sName,xVariant);
}

void XRegistersView::paintEvent(QPaintEvent *pEvent)
{
    QPainter *pPainter=new QPainter(this->viewport());
    pPainter->setPen(viewport()->palette().color(QPalette::WindowText));
    pPainter->setBackgroundMode(Qt::TransparentMode);

    qint32 nNumberOfRegions=g_listRegions.count();

    for(qint32 i=0;i<nNumberOfRegions;i++)
    {
        pPainter->save();
        QString sTitle=g_listRegions.at(i).sTitle;
        bool bChanged=g_stChanged.contains(sTitle);
        qint32 nTop=g_listRegions.at(i).nTop;
        qint32 nLeft=g_listRegions.at(i).nLeft;

        pPainter->setPen(QColor(Qt::gray));
        pPainter->drawText(nLeft,nTop,sTitle); // TODO Text Optional

        pPainter->restore();

        pPainter->save();

        if(bChanged)
        {
            pPainter->setPen(QColor(Qt::red));
        }

        // TODO MMX
        pPainter->drawText(nLeft+g_listRegions.at(i).nTitleWidth,nTop,XBinary::xVariantToHex(g_mapRegisters.value(sTitle))); // TODO Text Optional

        pPainter->restore();
        // TODO Comment
    }

    delete pPainter;
}

void XRegistersView::paintCell(QPainter *pPainter,qint32 nRow,qint32 nColumn,qint32 nLeft,qint32 nTop,qint32 nWidth,qint32 nHeight)
{
    // TODO remove
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

        pPainter->drawText(nLeft+g_listRegions.at(nIndex).nTitleWidth,nTop+nHeight,XBinary::xVariantToHex(g_mapRegisters.value(sTitle))); // TODO Text Optional

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
