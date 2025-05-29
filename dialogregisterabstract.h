/* Copyright (c) 2022-2025 hors<horsicq@gmail.com>
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
#ifndef DIALOGREGISTERABSTRACT_H
#define DIALOGREGISTERABSTRACT_H

#include <QDialog>
#include <QGroupBox>

#include "xinfodb.h"

class DialogRegisterAbstract : public QDialog {
    Q_OBJECT

public:
    explicit DialogRegisterAbstract(QWidget *pParent = nullptr);

    void setTitle(QGroupBox *pGroupBox, XInfoDB::XREG reg);
    void setData(XInfoDB *pXInfoDB, XInfoDB::XREG reg);
    void setInitVariant(XBinary::XVARIANT variant);
    XBinary::XVARIANT getInitVariant();
    void setCurrentVariant(XBinary::XVARIANT variant);
    XBinary::XVARIANT getCurrentVariant();
    XInfoDB *getXInfoDB();
    XInfoDB::XREG getReg();

protected slots:
    void _on_line_textEdited(const QString &sString);

protected:
    virtual void initValue() = 0;
    virtual void adjustValue() = 0;
    virtual void setValue() = 0;

private:
    XInfoDB *g_pXInfoDB;
    XInfoDB::XREG g_reg;
    XBinary::XVARIANT g_initVariant;
    XBinary::XVARIANT g_currentVariant;
};

#endif  // DIALOGREGISTERABSTRACT_H
