/* Copyright (c) 2021-2025 hors<horsicq@gmail.com>
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
#include <QPainter>
#include <QRect>
#include <QVariant>
#include <QWidget>

#include "dialogregister16.h"
#include "dialogregister32.h"
#include "dialogregister64.h"
#ifdef Q_PROCESSOR_X86
#include "dialogdebug_x86.h"
#endif
#include "xinfodb.h"
#include "xshortcutstscrollarea.h"

// TODO replace DEBUG regs
class XRegistersView : public XShortcutstScrollArea {
    Q_OBJECT

    struct REGION {
        qint32 nLeft;
        qint32 nTop;
        qint32 nTitleWidth;
        qint32 nValueWidth;
        qint32 nCommentWidth;
        qint32 nHeight;
        XInfoDB::XREG reg;
        XInfoDB::RI_TYPE riType;
    };

public:
    XRegistersView(QWidget *pParent = nullptr);

    void setOptions(const XInfoDB::XREG_OPTIONS &regOptions);
    XInfoDB::XREG_OPTIONS getOptions();
    void setXInfoDB(XInfoDB *pXInfoDB);
    XInfoDB *getXinfoDB();
    XInfoDB::XREG pointToReg(QPoint pos, qint32 *pIndex);
    void reload();
    void clear();
    virtual void adjustView() override;
    bool isClearEnable(XInfoDB::XREG reg);
    bool isEditEnable(XInfoDB::XREG reg);

private:
    void addRegion(XInfoDB::XREG reg, qint32 nLeft, qint32 nTop, qint32 nTitleWidth, qint32 nValueWidth, qint32 nCommentWidth, XInfoDB::RI_TYPE riType);
    void addRegsList(QList<XInfoDB::XREG> *pRegsList, qint32 nLeft, qint32 nTop, qint32 nTitleWidth, qint32 nValueWidth, qint32 nCommentWidth, XInfoDB::RI_TYPE riType);
    void handleRegister(XInfoDB::XREG reg);
    qint32 handleNavi(qint32 nCurrentRegionIndex, QKeySequence::StandardKey key);

protected:
    virtual void paintEvent(QPaintEvent *pEvent) override;
    virtual void mousePressEvent(QMouseEvent *pEvent) override;
    virtual void mouseReleaseEvent(QMouseEvent *pEvent) override;
    virtual void keyPressEvent(QKeyEvent *pEvent) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *pEvent) override;

private slots:
    void _customContextMenu(const QPoint &pos);

public slots:
    void _actionViewGeneral();
    void _actionViewIP();
#ifdef Q_PROCESSOR_X86
    void _actionViewFlags();
    void _actionViewSegments();
    void _actionViewDebug();
    void _actionViewFloat();
    void _actionViewXMM();
    void _actionViewYMM();
#endif
    void _actionCopy();
    void _actionEdit();
    void _actionClear();

protected:
    virtual void registerShortcuts(bool bState) override;
    virtual void contextMenu(const QPoint &pos);

private:
    XInfoDB *g_pXInfoDB;
    QList<REGION> g_listRegions;
    qint32 g_nCharWidth;
    qint32 g_nCharHeight;
    XInfoDB::XREG_OPTIONS g_regOptions;
    QFont g_fontText;
    qint32 g_nCurrentRegionIndex;
};

#endif  // XREGISTERSVIEW_H
