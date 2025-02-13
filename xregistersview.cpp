/* Copyright (c) 2021-2024 hors<horsicq@gmail.com>
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
    XOptions::setMonoFont(this, 10);  // TODO options

    g_fontText = font();

    g_nCharWidth = XOptions::getCharWidth(this);
    g_nCharHeight = XOptions::getCharHeight(this);

    g_pXInfoDB = nullptr;

    g_nCurrentRegionIndex = 0;

    g_regOptions = {};
    g_regOptions.bGeneral = true;
    g_regOptions.bIP = true;
#ifdef Q_PROCESSOR_X86
    g_regOptions.bFlags = true;
    g_regOptions.bSegments = true;
    g_regOptions.bDebug = true;
    g_regOptions.bFloat = true;
    g_regOptions.bXMM = true;
    g_regOptions.bYMM = true;
#endif
    //    addColumn("",300); // TODO Width
    //    setVerticalLinesVisible(true);
    ////    setHorisontalLinesVisible(true);
    //    setHeaderVisible(false);
    //    setLineDelta(0);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(_customContextMenu(QPoint)));
}

void XRegistersView::setOptions(const XInfoDB::XREG_OPTIONS &regOptions)
{
    g_regOptions = regOptions;
}

XInfoDB::XREG_OPTIONS XRegistersView::getOptions()
{
    return g_regOptions;
}

void XRegistersView::setXInfoDB(XInfoDB *pXInfoDB)
{
    g_pXInfoDB = pXInfoDB;

    setActive(true);
}

XInfoDB *XRegistersView::getXinfoDB()
{
    return g_pXInfoDB;
}

void XRegistersView::reload()
{
    if (isActive()) {
        g_listRegions.clear();
        const QFontMetricsF fm(g_fontText);

        qint32 nMinWidth = 0;
        qint32 nMinHeight = 0;

        qint32 nLeft = 5;
        qint32 nTop = 0;

        qint32 nValueWidthBit = fm.boundingRect("0  ").width();
        qint32 nValueWidth16 = fm.boundingRect("0000  ").width();
        qint32 nValueWidth32 = fm.boundingRect("00000000  ").width();
        nValueWidth32++;  // TODO remove
        qint32 nValueWidth64 = fm.boundingRect("0000000000000000  ").width();
        qint32 nValueWidth128 = fm.boundingRect("00000000000000000000000000000000  ").width();
        qint32 nValueWidth256 = fm.boundingRect("0000000000000000000000000000000000000000000000000000000000000000  ").width();
        qint32 nCommentWidth = g_nCharWidth * 40;  // TODO Check

        bool bFirst = false;

        if (g_regOptions.bGeneral) {
            bFirst = true;

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

            addRegsList(&listGeneralRegs, nLeft, nTop, g_nCharWidth * 3, nValueWidth32, nCommentWidth, XInfoDB::RI_TYPE_GENERAL);

            nTop += listGeneralRegs.count() * g_nCharHeight;
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

            addRegsList(&listGeneralRegs, nLeft, nTop, g_nCharWidth * 3, nValueWidth64, nCommentWidth, XInfoDB::RI_TYPE_GENERAL);

            nTop += listGeneralRegs.count() * g_nCharHeight;
#endif
        }

        if (g_regOptions.bIP) {
            if (bFirst) {
                nTop += g_nCharHeight / 2;  // Empty
            }

            bFirst = true;

#ifdef Q_PROCESSOR_X86_32
            addRegion(XInfoDB::XREG_EIP, nLeft, nTop, g_nCharWidth * 3, nValueWidth32, nCommentWidth, XInfoDB::RI_TYPE_ADDRESS);
#endif
#ifdef Q_PROCESSOR_X86_64
            addRegion(XInfoDB::XREG_RIP, nLeft, nTop, g_nCharWidth * 3, nValueWidth64, nCommentWidth, XInfoDB::RI_TYPE_ADDRESS);
#endif

            nTop += g_nCharHeight;
        }
#ifdef Q_PROCESSOR_X86
        if (g_regOptions.bFlags) {
            if (bFirst) {
                nTop += g_nCharHeight / 2;  // Empty
            }

            bFirst = true;
#ifdef Q_PROCESSOR_X86_32
            addRegion(XInfoDB::XREG_EFLAGS, nLeft, nTop, g_nCharWidth * 6, nValueWidth32, nCommentWidth, XInfoDB::RI_TYPE_UNKNOWN);
#endif
#ifdef Q_PROCESSOR_X86_64
            addRegion(XInfoDB::XREG_RFLAGS, nLeft, nTop, g_nCharWidth * 6, nValueWidth64, nCommentWidth, XInfoDB::RI_TYPE_UNKNOWN);
#endif
            nTop += g_nCharHeight;

            QList<XInfoDB::XREG> listRegs1;
            listRegs1.append(XInfoDB::XREG_FLAGS_ZF);
            listRegs1.append(XInfoDB::XREG_FLAGS_OF);
            listRegs1.append(XInfoDB::XREG_FLAGS_CF);
            addRegsList(&listRegs1, nLeft, nTop, g_nCharWidth * 2, nValueWidthBit, 0, XInfoDB::RI_TYPE_UNKNOWN);

            QList<XInfoDB::XREG> listRegs2;
            listRegs2.append(XInfoDB::XREG_FLAGS_PF);
            listRegs2.append(XInfoDB::XREG_FLAGS_SF);
            listRegs2.append(XInfoDB::XREG_FLAGS_TF);
            addRegsList(&listRegs2, nLeft + g_nCharWidth * 4, nTop, g_nCharWidth * 2, nValueWidthBit, 0, XInfoDB::RI_TYPE_UNKNOWN);

            QList<XInfoDB::XREG> listRegs3;
            listRegs3.append(XInfoDB::XREG_FLAGS_AF);
            listRegs3.append(XInfoDB::XREG_FLAGS_DF);
            listRegs3.append(XInfoDB::XREG_FLAGS_IF);
            addRegsList(&listRegs3, nLeft + g_nCharWidth * 8, nTop, g_nCharWidth * 2, nValueWidthBit, 0, XInfoDB::RI_TYPE_UNKNOWN);

            nTop += (3) * g_nCharHeight;
        }
#endif
#ifdef Q_PROCESSOR_X86
        if (g_regOptions.bSegments) {
            if (bFirst) {
                nTop += g_nCharHeight / 2;  // Empty
            }

            bFirst = true;

            QList<XInfoDB::XREG> listRegs1;
            listRegs1.append(XInfoDB::XREG_GS);
            listRegs1.append(XInfoDB::XREG_ES);
            listRegs1.append(XInfoDB::XREG_CS);
            addRegsList(&listRegs1, nLeft, nTop, g_nCharWidth * 2, nValueWidth16, 0, XInfoDB::RI_TYPE_UNKNOWN);

            QList<XInfoDB::XREG> listRegs2;
            listRegs2.append(XInfoDB::XREG_FS);
            listRegs2.append(XInfoDB::XREG_DS);
            listRegs2.append(XInfoDB::XREG_SS);
            addRegsList(&listRegs2, nLeft + g_nCharWidth * 6, nTop, g_nCharWidth * 2, nValueWidth16, 0, XInfoDB::RI_TYPE_UNKNOWN);

            nTop += (3) * g_nCharHeight;
        }
#endif
#ifdef Q_PROCESSOR_X86
        if (g_regOptions.bFloat) {
            if (bFirst) {
                nTop += g_nCharHeight / 2;  // Empty
            }

            bFirst = true;

            QList<XInfoDB::XREG> listControl;
            listControl.append(XInfoDB::XREG_FPCR);
            listControl.append(XInfoDB::XREG_FPSR);
            listControl.append(XInfoDB::XREG_FPTAG);
            //            listControl.append(XInfoDB::XREG_FPIOFF);
            //            listControl.append(XInfoDB::XREG_FPISEL);
            //            listControl.append(XInfoDB::XREG_FPDOFF);
            //            listControl.append(XInfoDB::XREG_FPDSEL);

            addRegsList(&listControl, nLeft, nTop, g_nCharWidth * 6, nValueWidth32, nCommentWidth, XInfoDB::RI_TYPE_UNKNOWN);
            nTop += listControl.count() * g_nCharHeight;

            QList<XInfoDB::XREG> listFloat;
            listFloat.append(XInfoDB::XREG_ST0);
            listFloat.append(XInfoDB::XREG_ST1);
            listFloat.append(XInfoDB::XREG_ST2);
            listFloat.append(XInfoDB::XREG_ST3);
            listFloat.append(XInfoDB::XREG_ST4);
            listFloat.append(XInfoDB::XREG_ST5);
            listFloat.append(XInfoDB::XREG_ST6);
            listFloat.append(XInfoDB::XREG_ST7);

            addRegsList(&listFloat, nLeft, nTop, g_nCharWidth * 3, nValueWidth128, nCommentWidth, XInfoDB::RI_TYPE_UNKNOWN);

            nTop += listFloat.count() * g_nCharHeight;

            // TODO tagWord
            // TODO StatusWord
            // ControlWord
        }
#endif
#ifdef Q_PROCESSOR_X86
        if (g_regOptions.bDebug) {
            if (bFirst) {
                nTop += g_nCharHeight / 2;  // Empty
            }

            bFirst = true;

            QList<XInfoDB::XREG> listDebugRegs;
            listDebugRegs.append(XInfoDB::XREG_DR0);
            listDebugRegs.append(XInfoDB::XREG_DR1);
            listDebugRegs.append(XInfoDB::XREG_DR2);
            listDebugRegs.append(XInfoDB::XREG_DR3);
            listDebugRegs.append(XInfoDB::XREG_DR6);
            listDebugRegs.append(XInfoDB::XREG_DR7);

#ifdef Q_PROCESSOR_X86_32
            addRegsList(&listDebugRegs, nLeft, nTop, g_nCharWidth * 3, nValueWidth32, nCommentWidth, XInfoDB::RI_TYPE_UNKNOWN);
#endif
#ifdef Q_PROCESSOR_X86_64
            addRegsList(&listDebugRegs, nLeft, nTop, g_nCharWidth * 3, nValueWidth64, nCommentWidth, XInfoDB::RI_TYPE_UNKNOWN);
#endif
            nTop += listDebugRegs.count() * g_nCharHeight;
        }
#endif
#ifdef Q_PROCESSOR_X86
        if (g_regOptions.bXMM) {
            if (bFirst) {
                nTop += g_nCharHeight / 2;  // Empty
            }

            bFirst = true;

            addRegion(XInfoDB::XREG_MXCSR, nLeft, nTop, g_nCharWidth * 5, nValueWidth32, nCommentWidth, XInfoDB::RI_TYPE_UNKNOWN);

            nTop += g_nCharHeight;

            addRegion(XInfoDB::XREG_MXCSR_MASK, nLeft, nTop, g_nCharWidth * 10, nValueWidth32, nCommentWidth, XInfoDB::RI_TYPE_UNKNOWN);

            nTop += g_nCharHeight;

            QList<XInfoDB::XREG> listXmmRegs;
            listXmmRegs.append(XInfoDB::XREG_XMM0);
            listXmmRegs.append(XInfoDB::XREG_XMM1);
            listXmmRegs.append(XInfoDB::XREG_XMM2);
            listXmmRegs.append(XInfoDB::XREG_XMM3);
            listXmmRegs.append(XInfoDB::XREG_XMM4);
            listXmmRegs.append(XInfoDB::XREG_XMM5);
            listXmmRegs.append(XInfoDB::XREG_XMM6);
            listXmmRegs.append(XInfoDB::XREG_XMM7);
#ifdef Q_PROCESSOR_X86_64
            listXmmRegs.append(XInfoDB::XREG_XMM8);
            listXmmRegs.append(XInfoDB::XREG_XMM9);
            listXmmRegs.append(XInfoDB::XREG_XMM10);
            listXmmRegs.append(XInfoDB::XREG_XMM11);
            listXmmRegs.append(XInfoDB::XREG_XMM12);
            listXmmRegs.append(XInfoDB::XREG_XMM13);
            listXmmRegs.append(XInfoDB::XREG_XMM14);
            listXmmRegs.append(XInfoDB::XREG_XMM15);
#endif
            addRegsList(&listXmmRegs, nLeft, nTop, g_nCharWidth * 5, nValueWidth128, nCommentWidth, XInfoDB::RI_TYPE_UNKNOWN);

            nTop += listXmmRegs.count() * g_nCharHeight;

            // TODO MxCsr
        }

        if (g_regOptions.bYMM) {
            if (bFirst) {
                nTop += g_nCharHeight / 2;  // Empty
            }

            bFirst = true;

            QList<XInfoDB::XREG> listYmmRegs;
            listYmmRegs.append(XInfoDB::XREG_YMM0);
            listYmmRegs.append(XInfoDB::XREG_YMM1);
            listYmmRegs.append(XInfoDB::XREG_YMM2);
            listYmmRegs.append(XInfoDB::XREG_YMM3);
            listYmmRegs.append(XInfoDB::XREG_YMM4);
            listYmmRegs.append(XInfoDB::XREG_YMM5);
            listYmmRegs.append(XInfoDB::XREG_YMM6);
            listYmmRegs.append(XInfoDB::XREG_YMM7);
            listYmmRegs.append(XInfoDB::XREG_YMM8);
            listYmmRegs.append(XInfoDB::XREG_YMM9);
            listYmmRegs.append(XInfoDB::XREG_YMM10);
            listYmmRegs.append(XInfoDB::XREG_YMM11);
            listYmmRegs.append(XInfoDB::XREG_YMM12);
            listYmmRegs.append(XInfoDB::XREG_YMM13);
            listYmmRegs.append(XInfoDB::XREG_YMM14);
            listYmmRegs.append(XInfoDB::XREG_YMM15);

            addRegsList(&listYmmRegs, nLeft, nTop, g_nCharWidth * 5, nValueWidth256, nCommentWidth, XInfoDB::RI_TYPE_UNKNOWN);

            nTop += listYmmRegs.count() * g_nCharHeight;
        }
#endif
        qint32 nNumberOfRegions = g_listRegions.count();

        for (qint32 i = 0; i < nNumberOfRegions; i++) {
            nMinWidth =
                qMax(nMinWidth, g_listRegions.at(i).nLeft + g_listRegions.at(i).nTitleWidth + g_listRegions.at(i).nValueWidth + g_listRegions.at(i).nCommentWidth);
            nMinHeight = qMax(nMinHeight, g_listRegions.at(i).nTop + g_listRegions.at(i).nHeight);
        }

        setMinimumWidth(nMinWidth);
        setMinimumHeight(nMinHeight);

        viewport()->update();
    }
}

void XRegistersView::clear()
{
    g_listRegions.clear();
#ifdef QT_DEBUG
    qDebug("void XRegistersView::clear()");
#endif
    setActive(false);

    viewport()->update();
}

void XRegistersView::adjustView()
{
    QFont _font;
    QString sFont = getGlobalOptions()->getValue(XOptions::ID_REGISTERS_FONT).toString();

    if ((sFont != "") && _font.fromString(sFont)) {
        g_fontText = _font;
    }

    g_nCurrentRegionIndex = 0;

    reload();
}

void XRegistersView::addRegion(XInfoDB::XREG reg, qint32 nLeft, qint32 nTop, qint32 nTitleWidth, qint32 nValueWidth, qint32 nCommentWidth, XInfoDB::RI_TYPE riType)
{
    REGION region = {};

    region.reg = reg;
    region.nLeft = nLeft;
    region.nTop = nTop;
    region.nTitleWidth = nTitleWidth;
    region.nValueWidth = nValueWidth;
    region.nCommentWidth = nCommentWidth;
    region.nHeight = g_nCharHeight;
    region.riType = riType;

    g_listRegions.append(region);
}

void XRegistersView::addRegsList(QList<XInfoDB::XREG> *pRegsList, qint32 nLeft, qint32 nTop, qint32 nTitleWidth, qint32 nValueWidth, qint32 nCommentWidth,
                                 XInfoDB::RI_TYPE riType)
{
    qint32 nNumberOfRegs = pRegsList->count();

    for (qint32 i = 0; i < nNumberOfRegs; i++) {
        addRegion(pRegsList->at(i), nLeft, nTop + i * g_nCharHeight, nTitleWidth, nValueWidth, nCommentWidth, riType);
    }
}

XInfoDB::XREG XRegistersView::pointToReg(QPoint pos, qint32 *pIndex)
{
    *pIndex = -1;
    XInfoDB::XREG result = XInfoDB::XREG_UNKNOWN;

    qint32 nNumberOfRegions = g_listRegions.count();

    for (qint32 i = 0; i < nNumberOfRegions; i++) {
        REGION region = g_listRegions.at(i);

        if ((pos.x() > region.nLeft + region.nTitleWidth) && (pos.x() < region.nLeft + region.nTitleWidth + region.nValueWidth) && (pos.y() > region.nTop) &&
            (pos.y() < region.nTop + region.nHeight)) {
            *pIndex = i;
            result = region.reg;

            break;
        }
    }

    return result;
}

void XRegistersView::handleRegister(XInfoDB::XREG reg)
{
    bool bSuccess = false;
#ifdef Q_PROCESSOR_X86
#ifdef Q_PROCESSOR_X86_32
    if ((reg == XInfoDB::XREG_EAX) || (reg == XInfoDB::XREG_EBX) || (reg == XInfoDB::XREG_ECX) || (reg == XInfoDB::XREG_EDX) || (reg == XInfoDB::XREG_ESI) ||
        (reg == XInfoDB::XREG_EDI) || (reg == XInfoDB::XREG_EBP) || (reg == XInfoDB::XREG_ESP)) {
        DialogRegister32 dialogReg(this);
        dialogReg.setData(g_pXInfoDB, reg);

        if (dialogReg.exec() == QDialog::Accepted) {
            reload();
        }

        bSuccess = true;
    }
#endif
#ifdef Q_PROCESSOR_X86_64
    if ((reg == XInfoDB::XREG_RAX) || (reg == XInfoDB::XREG_RBX) || (reg == XInfoDB::XREG_RCX) || (reg == XInfoDB::XREG_RDX) || (reg == XInfoDB::XREG_RSI) ||
        (reg == XInfoDB::XREG_RDI) || (reg == XInfoDB::XREG_RBP) || (reg == XInfoDB::XREG_RSP) || (reg == XInfoDB::XREG_R8) || (reg == XInfoDB::XREG_R9) ||
        (reg == XInfoDB::XREG_R10) || (reg == XInfoDB::XREG_R11) || (reg == XInfoDB::XREG_R12) || (reg == XInfoDB::XREG_R13) || (reg == XInfoDB::XREG_R14) ||
        (reg == XInfoDB::XREG_R15)) {
        DialogRegister64 dialogReg(this);
        dialogReg.setData(g_pXInfoDB, reg);

        if (dialogReg.exec() == QDialog::Accepted) {
            reload();
        }

        bSuccess = true;
    }

    if (!bSuccess) {
        if ((reg == XInfoDB::XREG_DR0) || (reg == XInfoDB::XREG_DR1) || (reg == XInfoDB::XREG_DR2) || (reg == XInfoDB::XREG_DR3) || (reg == XInfoDB::XREG_DR6) ||
            (reg == XInfoDB::XREG_DR7)) {
            DialogDebug_x86 dialogReg(this);
            dialogReg.setData(g_pXInfoDB);

            if (dialogReg.exec() == QDialog::Accepted) {
                reload();
            }

            bSuccess = true;
        }
    }
#endif
    //    if ((reg == XInfoDB::XREG_CS) || (reg == XInfoDB::XREG_DS) || (reg == XInfoDB::XREG_ES) || (reg == XInfoDB::XREG_FS) || (reg == XInfoDB::XREG_GS) ||
    //        (reg == XInfoDB::XREG_SS)) {
    //        DialogRegister16 dialogReg(this);
    //        dialogReg.setData(g_pXInfoDB, reg);

    //        if (dialogReg.exec() == QDialog::Accepted) {
    //            reload();
    //        }
    //    }
    if ((reg == XInfoDB::XREG_FLAGS_CF) || (reg == XInfoDB::XREG_FLAGS_PF) || (reg == XInfoDB::XREG_FLAGS_AF) || (reg == XInfoDB::XREG_FLAGS_ZF) ||
        (reg == XInfoDB::XREG_FLAGS_SF) || (reg == XInfoDB::XREG_FLAGS_TF) || (reg == XInfoDB::XREG_FLAGS_IF) || (reg == XInfoDB::XREG_FLAGS_DF) ||
        (reg == XInfoDB::XREG_FLAGS_OF)) {
#ifdef Q_PROCESSOR_X86_32
        XInfoDB::XREG _reg = XInfoDB::XREG_EFLAGS;
#endif
#ifdef Q_PROCESSOR_X86_64
        XInfoDB::XREG _reg = XInfoDB::XREG_RFLAGS;
#endif
        XBinary::XVARIANT _var = g_pXInfoDB->getCurrentRegCache(_reg);
        XBinary::XVARIANT var = XInfoDB::getFlagFromReg(_var, reg);

        _var = XInfoDB::setFlagToReg(_var, reg, !(var.var.v_bool));

        if (g_pXInfoDB->setCurrentReg(_reg, _var)) {
            g_pXInfoDB->setCurrentRegCache(_reg, _var);

            reload();
        }
    }

    if ((reg == XInfoDB::XREG_DR0) || (reg == XInfoDB::XREG_DR1) || (reg == XInfoDB::XREG_DR2) || (reg == XInfoDB::XREG_DR3) || (reg == XInfoDB::XREG_DR6) ||
        (reg == XInfoDB::XREG_DR7)) {
#ifdef QT_DEBUG
        qDebug("DEBUG registers");
#endif
    }
#endif
}

bool XRegistersView::isClearEnable(XInfoDB::XREG reg)
{
    bool bResult = false;
#ifdef Q_PROCESSOR_X86
#ifdef Q_PROCESSOR_X86_32
    if ((reg == XInfoDB::XREG_EAX) || (reg == XInfoDB::XREG_EBX) || (reg == XInfoDB::XREG_ECX) || (reg == XInfoDB::XREG_EDX) || (reg == XInfoDB::XREG_ESI) ||
        (reg == XInfoDB::XREG_EDI) || (reg == XInfoDB::XREG_EBP) || (reg == XInfoDB::XREG_ESP)) {
        bResult = true;
    }
#endif
#ifdef Q_PROCESSOR_X86_64
    if ((reg == XInfoDB::XREG_RAX) || (reg == XInfoDB::XREG_RBX) || (reg == XInfoDB::XREG_RCX) || (reg == XInfoDB::XREG_RDX) || (reg == XInfoDB::XREG_RSI) ||
        (reg == XInfoDB::XREG_RDI) || (reg == XInfoDB::XREG_RBP) || (reg == XInfoDB::XREG_RSP) || (reg == XInfoDB::XREG_R8) || (reg == XInfoDB::XREG_R9) ||
        (reg == XInfoDB::XREG_R10) || (reg == XInfoDB::XREG_R11) || (reg == XInfoDB::XREG_R12) || (reg == XInfoDB::XREG_R13) || (reg == XInfoDB::XREG_R14) ||
        (reg == XInfoDB::XREG_R15)) {
        bResult = true;
    }
#endif
#endif

    return bResult;
}

bool XRegistersView::isEditEnable(XInfoDB::XREG reg)
{
    bool bResult = false;
#ifdef Q_PROCESSOR_X86
#ifdef Q_PROCESSOR_X86_32
    if ((reg == XInfoDB::XREG_EAX) || (reg == XInfoDB::XREG_EBX) || (reg == XInfoDB::XREG_ECX) || (reg == XInfoDB::XREG_EDX) || (reg == XInfoDB::XREG_ESI) ||
        (reg == XInfoDB::XREG_EDI) || (reg == XInfoDB::XREG_EBP) || (reg == XInfoDB::XREG_ESP)) {
        bResult = true;
    }
#endif
#ifdef Q_PROCESSOR_X86_64
    if ((reg == XInfoDB::XREG_RAX) || (reg == XInfoDB::XREG_RBX) || (reg == XInfoDB::XREG_RCX) || (reg == XInfoDB::XREG_RDX) || (reg == XInfoDB::XREG_RSI) ||
        (reg == XInfoDB::XREG_RDI) || (reg == XInfoDB::XREG_RBP) || (reg == XInfoDB::XREG_RSP) || (reg == XInfoDB::XREG_R8) || (reg == XInfoDB::XREG_R9) ||
        (reg == XInfoDB::XREG_R10) || (reg == XInfoDB::XREG_R11) || (reg == XInfoDB::XREG_R12) || (reg == XInfoDB::XREG_R13) || (reg == XInfoDB::XREG_R14) ||
        (reg == XInfoDB::XREG_R15)) {
        bResult = true;
    }
#endif

    if (!bResult) {
        if ((reg == XInfoDB::XREG_DR0) || (reg == XInfoDB::XREG_DR1) || (reg == XInfoDB::XREG_DR2) || (reg == XInfoDB::XREG_DR3) || (reg == XInfoDB::XREG_DR6) ||
            (reg == XInfoDB::XREG_DR7)) {
            bResult = true;
        }
    }
#endif

    return bResult;
}

qint32 XRegistersView::handleNavi(qint32 nCurrentRegionIndex, QKeySequence::StandardKey key)
{
    qint32 nResult = nCurrentRegionIndex;

    qint32 nNumberOfRegions = g_listRegions.count();

    if (nCurrentRegionIndex < nNumberOfRegions) {
        bool bFound = false;

        if (!bFound) {
            qint32 nCurrentValue = -1;
            for (qint32 i = 0; i < nNumberOfRegions; i++) {
                if ((i != nCurrentRegionIndex) && (g_listRegions.at(i).nTop == g_listRegions.at(nCurrentRegionIndex).nTop)) {
                    if (key == QKeySequence::MoveToNextChar) {
                        if (g_listRegions.at(i).nLeft > g_listRegions.at(nCurrentRegionIndex).nLeft) {
                            if ((nCurrentValue == -1) || (g_listRegions.at(i).nLeft < nCurrentValue)) {
                                nCurrentValue = g_listRegions.at(i).nLeft;
                                nResult = i;
                                bFound = true;
                            }
                        }
                    } else if (key == QKeySequence::MoveToPreviousChar) {
                        if (g_listRegions.at(i).nLeft < g_listRegions.at(nCurrentRegionIndex).nLeft) {
                            if ((nCurrentValue == -1) || (g_listRegions.at(i).nLeft > nCurrentValue)) {
                                nCurrentValue = g_listRegions.at(i).nLeft;
                                nResult = i;
                                bFound = true;
                            }
                        }
                    }
                }
            }
        }

        if (!bFound) {
            if (key == QKeySequence::MoveToNextChar) {
                key = QKeySequence::MoveToNextLine;
            }

            if (key == QKeySequence::MoveToPreviousChar) {
                key = QKeySequence::MoveToPreviousLine;
            }
        }

        if (!bFound) {
            qint32 nCurrentValue = -1;
            for (qint32 i = 0; i < nNumberOfRegions; i++) {
                if ((i != nCurrentRegionIndex) && (g_listRegions.at(i).nLeft == g_listRegions.at(nCurrentRegionIndex).nLeft)) {
                    if (key == QKeySequence::MoveToNextLine) {
                        if (g_listRegions.at(i).nTop > g_listRegions.at(nCurrentRegionIndex).nTop) {
                            if ((nCurrentValue == -1) || (g_listRegions.at(i).nTop < nCurrentValue)) {
                                nCurrentValue = g_listRegions.at(i).nTop;
                                nResult = i;
                                bFound = true;
                            }
                        }
                    } else if (key == QKeySequence::MoveToPreviousLine) {
                        if (g_listRegions.at(i).nTop < g_listRegions.at(nCurrentRegionIndex).nTop) {
                            if ((nCurrentValue == -1) || (g_listRegions.at(i).nTop > nCurrentValue)) {
                                nCurrentValue = g_listRegions.at(i).nTop;
                                nResult = i;
                                bFound = true;
                            }
                        }
                    }
                }
            }
        }

        if (!bFound) {
            qint32 nCurrentValue = -1;
            for (qint32 i = 0; i < nNumberOfRegions; i++) {
                if (i != nCurrentRegionIndex) {
                    if (key == QKeySequence::MoveToNextLine) {
                        if (g_listRegions.at(i).nTop > g_listRegions.at(nCurrentRegionIndex).nTop) {
                            if ((nCurrentValue == -1) || (g_listRegions.at(i).nTop < nCurrentValue)) {
                                nCurrentValue = g_listRegions.at(i).nTop;
                                nResult = i;
                                //                                bFound = true;
                            }
                        }
                    } else if (key == QKeySequence::MoveToPreviousLine) {
                        if (g_listRegions.at(i).nTop < g_listRegions.at(nCurrentRegionIndex).nTop) {
                            if ((nCurrentValue == -1) || (g_listRegions.at(i).nTop > nCurrentValue)) {
                                nCurrentValue = g_listRegions.at(i).nTop;
                                nResult = i;
                                //                                bFound = true;
                            }
                        }
                    }
                }
            }
        }
    }

    return nResult;
}

void XRegistersView::paintEvent(QPaintEvent *pEvent)
{
    Q_UNUSED(pEvent)

    if (g_pXInfoDB) {
        QPainter *pPainter = new QPainter(this->viewport());
        pPainter->setFont(g_fontText);
        pPainter->setPen(viewport()->palette().color(QPalette::WindowText));
        pPainter->setBackgroundMode(Qt::TransparentMode);

        qint32 nNumberOfRegions = g_listRegions.count();

        for (qint32 i = 0; i < nNumberOfRegions; i++) {
            QRect rectTitle;
            rectTitle.setLeft(g_listRegions.at(i).nLeft);
            rectTitle.setTop(g_listRegions.at(i).nTop);
            rectTitle.setWidth(g_listRegions.at(i).nTitleWidth);
            rectTitle.setHeight(g_listRegions.at(i).nHeight);

            QRect rectValue;
            rectValue.setLeft(g_listRegions.at(i).nLeft + g_listRegions.at(i).nTitleWidth);
            rectValue.setTop(g_listRegions.at(i).nTop);
            rectValue.setWidth(g_listRegions.at(i).nValueWidth);
            rectValue.setHeight(g_listRegions.at(i).nHeight);

            QRect rectComment;
            rectComment.setLeft(g_listRegions.at(i).nLeft + g_listRegions.at(i).nTitleWidth + g_listRegions.at(i).nValueWidth);
            rectComment.setTop(g_listRegions.at(i).nTop);
            rectComment.setWidth(g_listRegions.at(i).nCommentWidth);
            rectComment.setHeight(g_listRegions.at(i).nHeight);

            QString sTitle = XInfoDB::regIdToString(g_listRegions.at(i).reg);
            bool bChanged = g_pXInfoDB->isRegChanged(g_listRegions.at(i).reg);
            bool bSelected = (i == g_nCurrentRegionIndex);

            pPainter->save();

            pPainter->setPen(QColor(Qt::gray));
            pPainter->drawText(rectTitle, sTitle);  // TODO Text Optional

            pPainter->restore();

            if (bChanged || bSelected) {
                pPainter->save();

                if (bChanged) {
                    pPainter->setPen(QColor(Qt::red));
                }

                if (bSelected) {
                    pPainter->fillRect(rectValue, getColor(TCLOLOR_SELECTED));
                }
            }

            XInfoDB::XREG reg = g_listRegions.at(i).reg;

            XBinary::XVARIANT xvariant = g_pXInfoDB->getCurrentRegCache(reg);

            // TODO MMX
            pPainter->drawText(rectValue, XBinary::xVariantToHex(xvariant));  // TODO Text Optional

            if (bChanged || bSelected) {
                pPainter->restore();
            }

            QString sComment;

            // if (g_listRegions.at(i).riType != XInfoDB::RI_TYPE_UNKNOWN) {
            //     sComment = XInfoDB::recordInfoToString(g_pXInfoDB->getRecordInfoCache(XBinary::xVariantToQword(xvariant)), g_listRegions.at(i).riType);
            // }

            if (sComment != "") {
                pPainter->drawText(rectComment, sComment);  // TODO Text Optional
            }
        }

        delete pPainter;
    }
}

void XRegistersView::mousePressEvent(QMouseEvent *pEvent)
{
    if ((pEvent->button() == Qt::LeftButton) || (pEvent->button() == Qt::RightButton)) {
        qint32 nIndex = -1;
        pointToReg(pEvent->pos(), &nIndex);

        if (nIndex != -1) {
            g_nCurrentRegionIndex = nIndex;
            viewport()->update();
        }
    }

    XShortcutstScrollArea::mousePressEvent(pEvent);
}

void XRegistersView::mouseReleaseEvent(QMouseEvent *pEvent)
{
    // TODO
    //    QString sReg=XInfoDB::regIdToString(pointToReg(pEvent->pos()));

    //    qDebug("%s",sReg.toLatin1().data());

    XShortcutstScrollArea::mouseReleaseEvent(pEvent);
}

void XRegistersView::keyPressEvent(QKeyEvent *pEvent)
{
    // TODO
    if (pEvent->matches(QKeySequence::MoveToNextChar)) {
        g_nCurrentRegionIndex = handleNavi(g_nCurrentRegionIndex, QKeySequence::MoveToNextChar);
        viewport()->update();
    } else if (pEvent->matches(QKeySequence::MoveToPreviousChar)) {
        g_nCurrentRegionIndex = handleNavi(g_nCurrentRegionIndex, QKeySequence::MoveToPreviousChar);
        viewport()->update();
    } else if (pEvent->matches(QKeySequence::MoveToNextLine)) {
        g_nCurrentRegionIndex = handleNavi(g_nCurrentRegionIndex, QKeySequence::MoveToNextLine);
        viewport()->update();
    } else if (pEvent->matches(QKeySequence::MoveToPreviousLine)) {
        g_nCurrentRegionIndex = handleNavi(g_nCurrentRegionIndex, QKeySequence::MoveToPreviousLine);
        viewport()->update();
    }
}

void XRegistersView::mouseDoubleClickEvent(QMouseEvent *pEvent)
{
    if (pEvent->button() == Qt::LeftButton) {
        qint32 nIndex = -1;
        XInfoDB::XREG reg = pointToReg(pEvent->pos(), &nIndex);

        if (nIndex != -1) {
            g_nCurrentRegionIndex = nIndex;

            if (reg != XInfoDB::XREG_UNKNOWN) {
                handleRegister(reg);
            }
        }
    }

    XShortcutstScrollArea::mousePressEvent(pEvent);
}

void XRegistersView::_customContextMenu(const QPoint &pos)
{
    contextMenu(pos);
}

void XRegistersView::_actionViewGeneral()
{
    g_regOptions.bGeneral = !(g_regOptions.bGeneral);

    adjustView();
}

void XRegistersView::_actionViewIP()
{
    g_regOptions.bIP = !(g_regOptions.bIP);

    adjustView();
}
#ifdef Q_PROCESSOR_X86
void XRegistersView::_actionViewFlags()
{
    g_regOptions.bFlags = !(g_regOptions.bFlags);

    adjustView();
}
#endif
#ifdef Q_PROCESSOR_X86
void XRegistersView::_actionViewSegments()
{
    g_regOptions.bSegments = !(g_regOptions.bSegments);

    adjustView();
}
#endif
#ifdef Q_PROCESSOR_X86
void XRegistersView::_actionViewDebug()
{
    g_regOptions.bDebug = !(g_regOptions.bDebug);

    adjustView();
}
#endif
#ifdef Q_PROCESSOR_X86
void XRegistersView::_actionViewFloat()
{
    g_regOptions.bFloat = !(g_regOptions.bFloat);

    adjustView();
}
#endif
#ifdef Q_PROCESSOR_X86
void XRegistersView::_actionViewXMM()
{
    g_regOptions.bXMM = !(g_regOptions.bXMM);

    adjustView();
}
#endif
#ifdef Q_PROCESSOR_X86
void XRegistersView::_actionViewYMM()
{
    g_regOptions.bYMM = !(g_regOptions.bYMM);

    adjustView();
}
#endif
void XRegistersView::_actionCopy()
{
    if ((g_nCurrentRegionIndex >= 0) && (g_nCurrentRegionIndex < g_listRegions.count())) {
        XInfoDB::XREG reg = g_listRegions.at(g_nCurrentRegionIndex).reg;
        if (reg != XInfoDB::XREG_UNKNOWN) {
            XBinary::XVARIANT var = g_pXInfoDB->getCurrentRegCache(reg);
            QString sValue = XBinary::xVariantToHex(var);

            QApplication::clipboard()->setText(sValue);
        }
    }
}
void XRegistersView::_actionEdit()
{
    if ((g_nCurrentRegionIndex >= 0) && (g_nCurrentRegionIndex < g_listRegions.count())) {
        XInfoDB::XREG reg = g_listRegions.at(g_nCurrentRegionIndex).reg;
        if (reg != XInfoDB::XREG_UNKNOWN) {
            handleRegister(reg);
        }
    }
}

void XRegistersView::_actionClear()
{
    if ((g_nCurrentRegionIndex >= 0) && (g_nCurrentRegionIndex < g_listRegions.count())) {
        XInfoDB::XREG reg = g_listRegions.at(g_nCurrentRegionIndex).reg;
        if (reg != XInfoDB::XREG_UNKNOWN) {
            XBinary::XVARIANT var = g_pXInfoDB->getCurrentRegCache(reg);

            XBinary::clearXVariant(&var);

            if (g_pXInfoDB->setCurrentReg(reg, var)) {
                g_pXInfoDB->setCurrentRegCache(reg, var);
                reload();
            }
        }
    }
}

void XRegistersView::registerShortcuts(bool bState)
{
    Q_UNUSED(bState)
}

void XRegistersView::contextMenu(const QPoint &pos)
{
    // TODO isContextMenuEnable // TODO Check mb remove
    QMenu contextMenu(this);

    QMenu menuView(tr("View"), this);

    QAction actionGeneral(QString("General"), this);
    actionGeneral.setCheckable(true);
    actionGeneral.setChecked(getOptions().bGeneral);
    connect(&actionGeneral, SIGNAL(triggered()), this, SLOT(_actionViewGeneral()));
    menuView.addAction(&actionGeneral);

    QAction actionIP(QString("IP"), this);
    actionIP.setCheckable(true);
    actionIP.setChecked(getOptions().bIP);
    connect(&actionIP, SIGNAL(triggered()), this, SLOT(_actionViewIP()));
    menuView.addAction(&actionIP);

#ifdef Q_PROCESSOR_X86
    QAction actionFlags(QString("Flags"), this);
    actionFlags.setCheckable(true);
    actionFlags.setChecked(getOptions().bFlags);
    connect(&actionFlags, SIGNAL(triggered()), this, SLOT(_actionViewFlags()));
    menuView.addAction(&actionFlags);

    QAction actionSegments(QString("Segments"), this);
    actionSegments.setCheckable(true);
    actionSegments.setChecked(getOptions().bSegments);
    connect(&actionSegments, SIGNAL(triggered()), this, SLOT(_actionViewSegments()));
    menuView.addAction(&actionSegments);

    QAction actionDebug(QString("Debug"), this);
    actionDebug.setCheckable(true);
    actionDebug.setChecked(getOptions().bDebug);
    connect(&actionDebug, SIGNAL(triggered()), this, SLOT(_actionViewDebug()));
    menuView.addAction(&actionDebug);

    QAction actionFloat(QString("Float"), this);
    actionFloat.setCheckable(true);
    actionFloat.setChecked(getOptions().bFloat);
    connect(&actionFloat, SIGNAL(triggered()), this, SLOT(_actionViewFloat()));
    menuView.addAction(&actionFloat);

    QAction actionXMM(QString("XMM"), this);
    actionXMM.setCheckable(true);
    actionXMM.setChecked(getOptions().bXMM);
    connect(&actionXMM, SIGNAL(triggered()), this, SLOT(_actionViewXMM()));
    menuView.addAction(&actionXMM);
#endif

    contextMenu.addMenu(&menuView);

    contextMenu.exec(viewport()->mapToGlobal(pos));
}
