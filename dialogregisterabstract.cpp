/* Copyright (c) 2022 hors<horsicq@gmail.com>
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
#include "dialogregisterabstract.h"

DialogRegisterAbstract::DialogRegisterAbstract(QWidget *pParent)
    : QDialog(pParent)
{
    g_pXInfoDB=nullptr;
    g_reg=XInfoDB::XREG_UNKNOWN;
    g_initVariant={};
    g_currentVariant={};
}

void DialogRegisterAbstract::setTitle(QGroupBox *pGroupBox,XInfoDB::XREG reg)
{
    QString sTitle=XInfoDB::regIdToString(reg);

    if(sTitle!="")
    {
        pGroupBox->setTitle(sTitle);
    }
}

void DialogRegisterAbstract::setData(XInfoDB *pXInfoDB,XInfoDB::XREG reg)
{
    g_pXInfoDB=pXInfoDB;
    g_reg=reg;

    initValue();
    adjustValue();
}

void DialogRegisterAbstract::setInitVariant(XBinary::XVARIANT variant)
{
    g_initVariant=variant;
}

XBinary::XVARIANT DialogRegisterAbstract::getInitVariant()
{
    return g_initVariant;
}

void DialogRegisterAbstract::setCurrentVariant(XBinary::XVARIANT variant)
{
    g_currentVariant=variant;
}

XBinary::XVARIANT DialogRegisterAbstract::getCurrentVariant()
{
    return g_currentVariant;
}

XInfoDB *DialogRegisterAbstract::getXInfoDB()
{
    return g_pXInfoDB;
}

XInfoDB::XREG DialogRegisterAbstract::getReg()
{
    return g_reg;
}

void DialogRegisterAbstract::_on_line_textEdited(const QString &sString)
{
    Q_UNUSED(sString)

    setValue();
    adjustValue();
}
