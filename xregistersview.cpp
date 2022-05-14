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
    XOptions::setMonoFont(this,10); // TODO options

    g_fontText=font();

    g_nCharWidth=XOptions::getCharWidth(this);
    g_nCharHeight=XOptions::getCharHeight(this);

    g_pXInfoDB=nullptr;

    g_regOptions={};
    g_regOptions.bGeneral=true;
    g_regOptions.bIP=true;
    g_regOptions.bFlags=true;
    g_regOptions.bSegments=true;
    g_regOptions.bDebug=true;
    g_regOptions.bFloat=true;
    g_regOptions.bXMM=true;

//    addColumn("",300); // TODO Width
//    setVerticalLinesVisible(true);
////    setHorisontalLinesVisible(true);
//    setHeaderVisible(false);
//    setLineDelta(0);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(_customContextMenu(QPoint)));
}

void XRegistersView::setOptions(XInfoDB::XREG_OPTIONS regOptions)
{
    g_regOptions=regOptions;
}

XInfoDB::XREG_OPTIONS XRegistersView::getOptions()
{
    return g_regOptions;
}

void XRegistersView::setXInfoDB(XInfoDB *pXInfoDB)
{
    g_pXInfoDB=pXInfoDB;
}

void XRegistersView::reload()
{
    g_listRegions.clear();

    const QFontMetricsF fm(g_fontText);

    qint32 nMinWidth=0;
    qint32 nMinHeight=0;

    qint32 nLeft=5;
    qint32 nTop=0;

    qint32 nValueWidthBit=fm.boundingRect("0  ").width();
    qint32 nValueWidth32=fm.boundingRect("00000000  ").width();
    qint32 nValueWidth64=fm.boundingRect("0000000000000000  ").width();
    qint32 nValueWidth128=g_nCharWidth*20; // Check
    qint32 nCommentWidth=g_nCharWidth*20; // TODO Check

    bool bFirst=false;

    if(g_regOptions.bGeneral)
    {
        bFirst=true;

        // TODO Check EAX EBX or EAX ECX

    #ifdef Q_PROCESSOR_X86_32
        QList<XInfoDB::XREG> listGeneralRegs;
        listGeneralRegs.append(XInfoDB::XREG_EAX);
        listGeneralRegs.append(XInfoDB::XREG_EBX);
        listGeneralRegs.append(XInfoDB::XREG_ECX);
        listGeneralRegs.append(XInfoDB::XREG_EDX);
        listGeneralRegs.append(XInfoDB::XREG_EBP);
        listGeneralRegs.append(XInfoDB::XREG_ESP);
        listGeneralRegs.append(XInfoDB::XREG_ESI);
        listGeneralRegs.append(XInfoDB::XREG_EDI);

        addRegsList(&listGeneralRegs,nLeft,nTop,g_nCharWidth*3,nValueWidth32,nCommentWidth,true);

        nTop+=listGeneralRegs.count()*g_nCharHeight;
    #endif
    #ifdef Q_PROCESSOR_X86_64
        QList<XInfoDB::XREG> listGeneralRegs;
        listGeneralRegs.append(XInfoDB::XREG_RAX);
        listGeneralRegs.append(XInfoDB::XREG_RBX);
        listGeneralRegs.append(XInfoDB::XREG_RCX);
        listGeneralRegs.append(XInfoDB::XREG_RDX);
        listGeneralRegs.append(XInfoDB::XREG_RBP);
        listGeneralRegs.append(XInfoDB::XREG_RSP);
        listGeneralRegs.append(XInfoDB::XREG_RSI);
        listGeneralRegs.append(XInfoDB::XREG_RDI);
        listGeneralRegs.append(XInfoDB::XREG_R8);
        listGeneralRegs.append(XInfoDB::XREG_R9);
        listGeneralRegs.append(XInfoDB::XREG_R10);
        listGeneralRegs.append(XInfoDB::XREG_R11);
        listGeneralRegs.append(XInfoDB::XREG_R12);
        listGeneralRegs.append(XInfoDB::XREG_R13);
        listGeneralRegs.append(XInfoDB::XREG_R14);
        listGeneralRegs.append(XInfoDB::XREG_R15);

        addRegsList(&listGeneralRegs,nLeft,nTop,g_nCharWidth*3,nValueWidth64,nCommentWidth,true);

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
        addRegion(XInfoDB::XREG_EIP,
                  nLeft,
                  nTop,
                  g_nCharWidth*3,
                  nValueWidth32,
                  nCommentWidth,true);
    #endif
    #ifdef Q_PROCESSOR_X86_64
        addRegion(XInfoDB::XREG_RIP,
                  nLeft,
                  nTop,
                  g_nCharWidth*3,
                  nValueWidth64,
                  nCommentWidth,true);
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

        addRegion(XInfoDB::XREG_EFLAGS,
                  nLeft,
                  nTop,
                  g_nCharWidth*6,
                  nValueWidth32,
                  nCommentWidth,false);
        nTop+=g_nCharHeight;

        QList<XInfoDB::XREG> listRegs1;
        listRegs1.append(XInfoDB::XREG_ZF);
        listRegs1.append(XInfoDB::XREG_OF);
        listRegs1.append(XInfoDB::XREG_CF);
        addRegsList(&listRegs1,nLeft,nTop,g_nCharWidth*2,nValueWidthBit,0,false);

        QList<XInfoDB::XREG> listRegs2;
        listRegs2.append(XInfoDB::XREG_ZF);
        listRegs2.append(XInfoDB::XREG_OF);
        listRegs2.append(XInfoDB::XREG_CF);
        addRegsList(&listRegs2,nLeft+g_nCharWidth*4,nTop,g_nCharWidth*2,nValueWidthBit,0,false);

        QList<XInfoDB::XREG> listRegs3;
        listRegs3.append(XInfoDB::XREG_AF);
        listRegs3.append(XInfoDB::XREG_DF);
        listRegs3.append(XInfoDB::XREG_IF);
        addRegsList(&listRegs3,nLeft+g_nCharWidth*8,nTop,g_nCharWidth*2,nValueWidthBit,0,false);

        nTop+=(3)*g_nCharHeight;
    }

    if(g_regOptions.bSegments)
    {
        if(bFirst)
        {
            nTop+=g_nCharHeight/2; // Empty
        }

        bFirst=true;

        QList<XInfoDB::XREG> listRegs1;
        listRegs1.append(XInfoDB::XREG_GS);
        listRegs1.append(XInfoDB::XREG_ES);
        listRegs1.append(XInfoDB::XREG_CS);
        addRegsList(&listRegs1,nLeft,nTop,g_nCharWidth*2,nValueWidthBit,0,false);

        QList<XInfoDB::XREG> listRegs2;
        listRegs2.append(XInfoDB::XREG_FS);
        listRegs2.append(XInfoDB::XREG_DS);
        listRegs2.append(XInfoDB::XREG_SS);
        addRegsList(&listRegs2,nLeft+g_nCharWidth*6,nTop,g_nCharWidth*2,nValueWidthBit,0,false);

        nTop+=(3)*g_nCharHeight;
    }

    if(g_regOptions.bFloat)
    {
        if(bFirst)
        {
            nTop+=g_nCharHeight/2; // Empty
        }

        bFirst=true;

        QList<XInfoDB::XREG> listXmmFloat;
        listXmmFloat.append(XInfoDB::XREG_ST0);
        listXmmFloat.append(XInfoDB::XREG_ST1);
        listXmmFloat.append(XInfoDB::XREG_ST2);
        listXmmFloat.append(XInfoDB::XREG_ST3);
        listXmmFloat.append(XInfoDB::XREG_ST4);
        listXmmFloat.append(XInfoDB::XREG_ST5);
        listXmmFloat.append(XInfoDB::XREG_ST6);
        listXmmFloat.append(XInfoDB::XREG_ST7);

        addRegsList(&listXmmFloat,nLeft,nTop,g_nCharWidth*3,nValueWidth128,nCommentWidth,false);

        nTop+=listXmmFloat.count()*g_nCharHeight;

        // TODO tagWord
        // TODO StatusWord
        // ControlWord
    }

    if(g_regOptions.bDebug)
    {
        if(bFirst)
        {
            nTop+=g_nCharHeight/2; // Empty
        }

        bFirst=true;

        QList<XInfoDB::XREG> listDebugRegs;
        listDebugRegs.append(XInfoDB::XREG_DR0);
        listDebugRegs.append(XInfoDB::XREG_DR1);
        listDebugRegs.append(XInfoDB::XREG_DR2);
        listDebugRegs.append(XInfoDB::XREG_DR3);
        listDebugRegs.append(XInfoDB::XREG_DR6);
        listDebugRegs.append(XInfoDB::XREG_DR7);

    #ifdef Q_PROCESSOR_X86_32
        addRegsList(&listDebugRegs,nLeft,nTop,g_nCharWidth*3,nValueWidth32,nCommentWidth,false);
    #endif
    #ifdef Q_PROCESSOR_X86_64
        addRegsList(&listDebugRegs,nLeft,nTop,g_nCharWidth*3,nValueWidth64,nCommentWidth,false);
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

        QList<XInfoDB::XREG> listXmmRegs;
        listXmmRegs.append(XInfoDB::XREG_XMM0);
        listXmmRegs.append(XInfoDB::XREG_XMM1);
        listXmmRegs.append(XInfoDB::XREG_XMM2);
        listXmmRegs.append(XInfoDB::XREG_XMM3);
        listXmmRegs.append(XInfoDB::XREG_XMM4);
        listXmmRegs.append(XInfoDB::XREG_XMM5);
        listXmmRegs.append(XInfoDB::XREG_XMM6);
        listXmmRegs.append(XInfoDB::XREG_XMM7);
        listXmmRegs.append(XInfoDB::XREG_XMM8);
        listXmmRegs.append(XInfoDB::XREG_XMM9);
        listXmmRegs.append(XInfoDB::XREG_XMM10);
        listXmmRegs.append(XInfoDB::XREG_XMM11);
        listXmmRegs.append(XInfoDB::XREG_XMM12);
        listXmmRegs.append(XInfoDB::XREG_XMM13);
        listXmmRegs.append(XInfoDB::XREG_XMM14);
        listXmmRegs.append(XInfoDB::XREG_XMM15);

        addRegsList(&listXmmRegs,nLeft,nTop,g_nCharWidth*5,nValueWidth128,nCommentWidth,false);

        nTop+=listXmmRegs.count()*g_nCharHeight;

        // TODO MxCsr
    }

    qint32 nNumberOfRegions=g_listRegions.count();

    for(qint32 i=0;i<nNumberOfRegions;i++)
    {
        nMinWidth=qMax(nMinWidth,g_listRegions.at(i).nLeft+g_listRegions.at(i).nTitleWidth+g_listRegions.at(i).nValueWidth+g_listRegions.at(i).nCommentWidth);
        nMinHeight=qMax(nMinHeight,g_listRegions.at(i).nTop+g_listRegions.at(i).nHeight);
    }

    setMinimumWidth(nMinWidth);
    setMinimumHeight(nMinHeight);

    viewport()->update();
}

void XRegistersView::clear()
{
    g_listRegions.clear();
#ifdef QT_DEBUG
    qDebug("void XRegistersView::clear()");
#endif

    viewport()->update();
}

void XRegistersView::adjustView()
{
    QFont _font;
    QString sFont=getGlobalOptions()->getValue(XOptions::ID_REGISTERS_FONT).toString();

    if((sFont!="")&&_font.fromString(sFont))
    {
        g_fontText=_font;
    }

    reload();
}

void XRegistersView::addRegion(XInfoDB::XREG reg, qint32 nLeft, qint32 nTop, qint32 nTitleWidth, qint32 nValueWidth, qint32 nCommentWidth, bool bSymbol)
{
    REGION region={};

    region.reg=reg;
    region.nLeft=nLeft;
    region.nTop=nTop;
    region.nTitleWidth=nTitleWidth;
    region.nValueWidth=nValueWidth;
    region.nCommentWidth=nCommentWidth;
    region.nHeight=g_nCharHeight;
    region.bSymbol=bSymbol;

    g_listRegions.append(region);
}

void XRegistersView::addRegsList(QList<XInfoDB::XREG> *pRegsList,qint32 nLeft,qint32 nTop,qint32 nTitleWidth,qint32 nValueWidth,qint32 nCommentWidth,bool bSymbol)
{
    qint32 nNumberOfRegs=pRegsList->count();

    for(qint32 i=0;i<nNumberOfRegs;i++)
    {
        addRegion(pRegsList->at(i),
                  nLeft,
                  nTop+i*g_nCharHeight,
                  nTitleWidth,
                  nValueWidth,
                  nCommentWidth,
                  bSymbol);
    }
}

XInfoDB::XREG XRegistersView::pointToReg(QPoint pos)
{
    XInfoDB::XREG result=XInfoDB::XREG_UNKNOWN;

    qint32 nNumberOfRegions=g_listRegions.count();

    for(qint32 i=0;i<nNumberOfRegions;i++)
    {
        REGION region=g_listRegions.at(i);

        if( (pos.x()>region.nLeft)&&
            (pos.x()<region.nLeft+region.nTitleWidth+region.nValueWidth+region.nCommentWidth)&&
            (pos.y()>region.nTop)&&
            (pos.y()<region.nTop+region.nHeight)
            )
        {
            result=region.reg;

            break;
        }
    }

    return result;
}

void XRegistersView::showRegister(XInfoDB::XREG reg)
{
#ifdef Q_PROCESSOR_X86_32

#endif
#ifdef Q_PROCESSOR_X86_64
    if( (reg==XInfoDB::XREG_RAX)||
        (reg==XInfoDB::XREG_RBX)||
        (reg==XInfoDB::XREG_RCX)||
        (reg==XInfoDB::XREG_RDX)||
        (reg==XInfoDB::XREG_RSI)||
        (reg==XInfoDB::XREG_RDI)||
        (reg==XInfoDB::XREG_RBP)||
        (reg==XInfoDB::XREG_RSP)||
        (reg==XInfoDB::XREG_R8)||
        (reg==XInfoDB::XREG_R9)||
        (reg==XInfoDB::XREG_R10)||
        (reg==XInfoDB::XREG_R11)||
        (reg==XInfoDB::XREG_R12)||
        (reg==XInfoDB::XREG_R13)||
        (reg==XInfoDB::XREG_R14)||
        (reg==XInfoDB::XREG_R15)
        )
    {
        DialogRegisterGeneral dialogReg(this);

        dialogReg.setData(g_pXInfoDB,reg);

        if(dialogReg.exec()==QDialog::Accepted)
        {
            reload();
        }
    }
#endif
}

void XRegistersView::paintEvent(QPaintEvent *pEvent)
{
    Q_UNUSED(pEvent)

    if(g_pXInfoDB)
    {
        QPainter *pPainter=new QPainter(this->viewport());
        pPainter->setFont(g_fontText);
        pPainter->setPen(viewport()->palette().color(QPalette::WindowText));
        pPainter->setBackgroundMode(Qt::TransparentMode);

        qint32 nNumberOfRegions=g_listRegions.count();

        for(qint32 i=0;i<nNumberOfRegions;i++)
        {
            pPainter->save();
            QString sTitle=XInfoDB::regIdToString(g_listRegions.at(i).reg);
            bool bChanged=g_pXInfoDB->isRegChanged(g_listRegions.at(i).reg);
            qint32 nTop=g_listRegions.at(i).nTop;
            qint32 nLeft=g_listRegions.at(i).nLeft;

            pPainter->setPen(QColor(Qt::gray));
            pPainter->drawText(nLeft,nTop+g_listRegions.at(i).nHeight,sTitle); // TODO Text Optional

            pPainter->restore();

            if(bChanged)
            {
                pPainter->save();
                pPainter->setPen(QColor(Qt::red));
            }

            XBinary::XVARIANT xvariant=g_pXInfoDB->getCurrentReg(g_listRegions.at(i).reg);

            // TODO MMX
            pPainter->drawText(nLeft+g_listRegions.at(i).nTitleWidth,nTop+g_listRegions.at(i).nHeight,XBinary::xVariantToHex(xvariant)); // TODO Text Optional

            if(bChanged)
            {
                pPainter->restore();
            }

            // TODO Comment
            QString sComment;

            if(g_listRegions.at(i).bSymbol)
            {
                sComment=XInfoDB::recordInfoToString(g_pXInfoDB->getRecordInfoCache(XBinary::xVariantToQword(xvariant)),XInfoDB::RI_TYPE_GENERAL);
            }

            if(sComment!="")
            {
                pPainter->drawText(nLeft+g_listRegions.at(i).nTitleWidth+g_listRegions.at(i).nValueWidth,nTop+g_listRegions.at(i).nHeight,sComment); // TODO Text Optional
            }
        }

        delete pPainter;
    }
}

void XRegistersView::mousePressEvent(QMouseEvent *pEvent)
{
    XInfoDB::XREG reg=pointToReg(pEvent->pos());

    if(reg!=XInfoDB::XREG_UNKNOWN)
    {
        showRegister(reg);
    }
    // TODO Dialog
//    QString sReg=XInfoDB::regIdToString();

//    qDebug("%s",sReg.toLatin1().data());

    XShortcutstScrollArea::mousePressEvent(pEvent);
}

void XRegistersView::mouseReleaseEvent(QMouseEvent *pEvent)
{
    // TODO
//    QString sReg=XInfoDB::regIdToString(pointToReg(pEvent->pos()));

//    qDebug("%s",sReg.toLatin1().data());

    XShortcutstScrollArea::mouseReleaseEvent(pEvent);
}

void XRegistersView::_customContextMenu(const QPoint &pos)
{
    contextMenu(pos);
}

void XRegistersView::actionViewGeneral()
{
    g_regOptions.bGeneral=!(g_regOptions.bGeneral);

    adjustView();
}

void XRegistersView::actionViewIP()
{
    g_regOptions.bIP=!(g_regOptions.bIP);

    adjustView();
}

void XRegistersView::actionViewFlags()
{
    g_regOptions.bFlags=!(g_regOptions.bFlags);

    adjustView();
}

void XRegistersView::actionViewSegments()
{
    g_regOptions.bSegments=!(g_regOptions.bSegments);

    adjustView();
}

void XRegistersView::actionViewDebug()
{
    g_regOptions.bDebug=!(g_regOptions.bDebug);

    adjustView();
}

void XRegistersView::actionViewFloat()
{
    g_regOptions.bFloat=!(g_regOptions.bFloat);

    adjustView();
}

void XRegistersView::actionViewXMM()
{
    g_regOptions.bXMM=!(g_regOptions.bXMM);

    adjustView();
}

void XRegistersView::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}

void XRegistersView::contextMenu(const QPoint &pos)
{
    QMenu contextMenu(this);

    QMenu menuView(tr("View"),this);

    QAction actionGeneral(QString("General"),this);
    actionGeneral.setCheckable(true);
    actionGeneral.setChecked(g_regOptions.bGeneral);
    connect(&actionGeneral,SIGNAL(triggered()),this,SLOT(actionViewGeneral()));
    menuView.addAction(&actionGeneral);

    QAction actionIP(QString("IP"),this);
    actionIP.setCheckable(true);
    actionIP.setChecked(g_regOptions.bIP);
    connect(&actionIP,SIGNAL(triggered()),this,SLOT(actionViewIP()));
    menuView.addAction(&actionIP);

    QAction actionFlags(QString("Flags"),this);
    actionFlags.setCheckable(true);
    actionFlags.setChecked(g_regOptions.bFlags);
    connect(&actionFlags,SIGNAL(triggered()),this,SLOT(actionViewFlags()));
    menuView.addAction(&actionFlags);

    QAction actionSegments(QString("Segments"),this);
    actionSegments.setCheckable(true);
    actionSegments.setChecked(g_regOptions.bSegments);
    connect(&actionSegments,SIGNAL(triggered()),this,SLOT(actionViewSegments()));
    menuView.addAction(&actionSegments);

    QAction actionDebug(QString("Debug"),this);
    actionDebug.setCheckable(true);
    actionDebug.setChecked(g_regOptions.bDebug);
    connect(&actionDebug,SIGNAL(triggered()),this,SLOT(actionViewDebug()));
    menuView.addAction(&actionDebug);

    QAction actionFloat(QString("Float"),this);
    actionFloat.setCheckable(true);
    actionFloat.setChecked(g_regOptions.bFloat);
    connect(&actionFloat,SIGNAL(triggered()),this,SLOT(actionViewFloat()));
    menuView.addAction(&actionFloat);

    QAction actionXMM(QString("XMM"),this);
    actionXMM.setCheckable(true);
    actionXMM.setChecked(g_regOptions.bXMM);
    connect(&actionXMM,SIGNAL(triggered()),this,SLOT(actionViewXMM()));
    menuView.addAction(&actionXMM);

    contextMenu.addMenu(&menuView);

    contextMenu.exec(viewport()->mapToGlobal(pos));
}
