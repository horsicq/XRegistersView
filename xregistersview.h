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
#include <QPainter>
#include "xshortcutstscrollarea.h"
#include "xformats.h"
#include "xinfodb.h"

class XRegistersView : public XShortcutstScrollArea
{
    Q_OBJECT

    struct REGION
    {
        qint32 nLeft;
        qint32 nTop;
        qint32 nTitleWidth;
        qint32 nValueWidth;
        qint32 nCommentWidth;
        qint32 nHeight;
        XInfoDB::REG reg;
    };

public:

    XRegistersView(QWidget *pParent=nullptr);
    void setOptions(XInfoDB::REG_OPTIONS regOptions);
    XInfoDB::REG_OPTIONS getOptions();
    void update(XInfoDB *pInfoDB);
    void clear();
    virtual void adjustView() override;

private:
    void addRegion(XInfoDB::REG reg, qint32 nLeft, qint32 nTop, qint32 nTitleWidth, qint32 nValueWidth, qint32 nCommentWidth);
    void addRegsList(QList<XInfoDB::REG> *pRegsList,qint32 nLeft,qint32 nTop,qint32 nTitleWidth,qint32 nValueWidth,qint32 nCommentWidth);

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
    virtual void registerShortcuts(bool bState) override;
    virtual void contextMenu(const QPoint &pos);

private:
    bool g_bActive;
    XInfoDB *g_pInfoDB;
    QList<REGION> g_listRegions;
    qint32 g_nCharWidth;
    qint32 g_nCharHeight;
    XInfoDB::REG_OPTIONS g_regOptions;
};

#endif // XREGISTERSVIEW_H
