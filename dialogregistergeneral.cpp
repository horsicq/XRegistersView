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
#include "dialogregistergeneral.h"
#include "ui_dialogregistergeneral.h"

DialogRegisterGeneral::DialogRegisterGeneral(QWidget *pParent) :
    QDialog(pParent),
    ui(new Ui::DialogRegisterGeneral)
{
    ui->setupUi(this);

    // TODO DEC <-> HEX
    // TODO Show Names Show Bits

    g_pXInfoDB=nullptr;
    g_reg=XInfoDB::XREG_UNKNOWN;
    g_nInitValue=0;
    g_nCurrentValue=0;

    ui->pushButtonOK->setEnabled(false);

    ui->groupBoxReg64->setTitle("63-0");
    ui->groupBoxReg32_1->setTitle("63-32");
    ui->groupBoxReg32_2->setTitle("31-0");
    ui->groupBoxReg16_1->setTitle("63-48");
    ui->groupBoxReg16_2->setTitle("47-32");
    ui->groupBoxReg16_3->setTitle("31-16");
    ui->groupBoxReg16_4->setTitle("15-0");
    ui->groupBoxReg8_1->setTitle("63-56");
    ui->groupBoxReg8_2->setTitle("55-48");
    ui->groupBoxReg8_3->setTitle("47-40");
    ui->groupBoxReg8_4->setTitle("39-32");
    ui->groupBoxReg8_5->setTitle("31-24");
    ui->groupBoxReg8_6->setTitle("23-16");
    ui->groupBoxReg8_7->setTitle("15-8");
    ui->groupBoxReg8_8->setTitle("7-0");

#ifdef Q_PROCESSOR_X86_32
    ui->groupBoxReg64->hide();
    ui->groupBoxReg32_1->hide();
#endif
}

DialogRegisterGeneral::~DialogRegisterGeneral()
{
    delete ui;
}

void DialogRegisterGeneral::setData(XInfoDB *pXInfoDB,XInfoDB::XREG reg)
{
    g_pXInfoDB=pXInfoDB;
    g_reg=reg;

#ifdef Q_PROCESSOR_X86_32
    g_nInitValue=pXInfoDB->getCurrentRegCache(reg).var.v_uint32;
    setTitle(ui->groupBoxReg32_2,reg);
    setTitle(ui->groupBoxReg16_4,XInfoDB::getSubReg16(reg));
    setTitle(ui->groupBoxReg8_7,XInfoDB::getSubReg8H(reg));
    setTitle(ui->groupBoxReg8_8,XInfoDB::getSubReg8L(reg));
#endif
#ifdef Q_PROCESSOR_X86_64
    g_nInitValue=pXInfoDB->getCurrentRegCache(reg).var.v_uint64;
    setTitle(ui->groupBoxReg64,reg);
    setTitle(ui->groupBoxReg32_2,XInfoDB::getSubReg32(reg));
    setTitle(ui->groupBoxReg16_4,XInfoDB::getSubReg16(reg));
    setTitle(ui->groupBoxReg8_7,XInfoDB::getSubReg8H(reg));
    setTitle(ui->groupBoxReg8_8,XInfoDB::getSubReg8L(reg));
#endif

    g_nCurrentValue=g_nInitValue;

    reload();
}

void DialogRegisterGeneral::reload()
{
#ifdef Q_PROCESSOR_X86_64
    if(!(ui->lineEditReg64->isFocused()))       ui->lineEditReg64->setValue(g_nCurrentValue);
#endif

    if(!(ui->lineEditReg32_1->isFocused()))     ui->lineEditReg32_1->setValue(XBinary::getDwordFromQword(g_nCurrentValue,1));
    if(!(ui->lineEditReg32_2->isFocused()))     ui->lineEditReg32_2->setValue(XBinary::getDwordFromQword(g_nCurrentValue,0));

    if(!(ui->lineEditReg16_1->isFocused()))     ui->lineEditReg16_1->setValue(XBinary::getWordFromQword(g_nCurrentValue,3));
    if(!(ui->lineEditReg16_2->isFocused()))     ui->lineEditReg16_2->setValue(XBinary::getWordFromQword(g_nCurrentValue,2));
    if(!(ui->lineEditReg16_3->isFocused()))     ui->lineEditReg16_3->setValue(XBinary::getWordFromQword(g_nCurrentValue,1));
    if(!(ui->lineEditReg16_4->isFocused()))     ui->lineEditReg16_4->setValue(XBinary::getWordFromQword(g_nCurrentValue,0));

    if(!(ui->lineEditReg8_1->isFocused()))      ui->lineEditReg8_1->setValue(XBinary::getByteFromQword(g_nCurrentValue,7));
    if(!(ui->lineEditReg8_2->isFocused()))      ui->lineEditReg8_2->setValue(XBinary::getByteFromQword(g_nCurrentValue,6));
    if(!(ui->lineEditReg8_3->isFocused()))      ui->lineEditReg8_3->setValue(XBinary::getByteFromQword(g_nCurrentValue,5));
    if(!(ui->lineEditReg8_4->isFocused()))      ui->lineEditReg8_4->setValue(XBinary::getByteFromQword(g_nCurrentValue,4));
    if(!(ui->lineEditReg8_5->isFocused()))      ui->lineEditReg8_5->setValue(XBinary::getByteFromQword(g_nCurrentValue,3));
    if(!(ui->lineEditReg8_6->isFocused()))      ui->lineEditReg8_6->setValue(XBinary::getByteFromQword(g_nCurrentValue,2));
    if(!(ui->lineEditReg8_7->isFocused()))      ui->lineEditReg8_7->setValue(XBinary::getByteFromQword(g_nCurrentValue,1));
    if(!(ui->lineEditReg8_8->isFocused()))      ui->lineEditReg8_8->setValue(XBinary::getByteFromQword(g_nCurrentValue,0));

    ui->pushButtonOK->setEnabled(g_nInitValue!=g_nCurrentValue);
}

XBinary::XVARIANT DialogRegisterGeneral::getCurrentValue()
{
    XBinary::XVARIANT result={};

#ifdef Q_PROCESSOR_X86_32
    result=XBinary::getXVariant((quint32)g_nCurrentValue);
#endif
#ifdef Q_PROCESSOR_X86_64
    result=XBinary::getXVariant((quint64)g_nCurrentValue);
#endif

    return result;
}

void DialogRegisterGeneral::on_pushButtonOK_clicked()
{
    if(g_pXInfoDB->setCurrentReg(g_reg,getCurrentValue())) // TODO Save register TODO
    {
        g_pXInfoDB->setCurrentRegCache(g_reg,getCurrentValue());

        accept();
    }
}

void DialogRegisterGeneral::on_pushButtonCancel_clicked()
{
    reject();
}

void DialogRegisterGeneral::setTitle(QGroupBox *pGroupBox,XInfoDB::XREG reg)
{
    QString sTitle=XInfoDB::regIdToString(reg);

    if(sTitle!="")
    {
        pGroupBox->setTitle(sTitle);
    }
}

void DialogRegisterGeneral::on_lineEditReg64_textEdited(const QString &sString)
{
    Q_UNUSED(sString)

    adjust();
}

void DialogRegisterGeneral::on_lineEditReg32_1_textEdited(const QString &sString)
{
    Q_UNUSED(sString)

    adjust();
}

void DialogRegisterGeneral::on_lineEditReg32_2_textEdited(const QString &sString)
{
    Q_UNUSED(sString)

    adjust();
}

void DialogRegisterGeneral::on_lineEditReg16_1_textEdited(const QString &sString)
{
    Q_UNUSED(sString)

    adjust();
}

void DialogRegisterGeneral::on_lineEditReg16_2_textEdited(const QString &sString)
{
    Q_UNUSED(sString)

    adjust();
}

void DialogRegisterGeneral::on_lineEditReg16_3_textEdited(const QString &sString)
{
    Q_UNUSED(sString)

    adjust();
}

void DialogRegisterGeneral::on_lineEditReg16_4_textEdited(const QString &sString)
{
    Q_UNUSED(sString)

    adjust();
}

void DialogRegisterGeneral::on_lineEditReg8_1_textEdited(const QString &sString)
{
    Q_UNUSED(sString)

    adjust();
}

void DialogRegisterGeneral::on_lineEditReg8_2_textEdited(const QString &sString)
{
    Q_UNUSED(sString)

    adjust();
}

void DialogRegisterGeneral::on_lineEditReg8_3_textEdited(const QString &sString)
{
    Q_UNUSED(sString)

    adjust();
}

void DialogRegisterGeneral::on_lineEditReg8_4_textEdited(const QString &sString)
{
    Q_UNUSED(sString)

    adjust();
}

void DialogRegisterGeneral::on_lineEditReg8_5_textEdited(const QString &sString)
{
    Q_UNUSED(sString)

    adjust();
}

void DialogRegisterGeneral::on_lineEditReg8_6_textEdited(const QString &sString)
{
    Q_UNUSED(sString)

    adjust();
}

void DialogRegisterGeneral::on_lineEditReg8_7_textEdited(const QString &sString)
{
    Q_UNUSED(sString)

    adjust();
}

void DialogRegisterGeneral::on_lineEditReg8_8_textEdited(const QString &sString)
{
    Q_UNUSED(sString)

    adjust();
}

void DialogRegisterGeneral::adjust()
{
#ifdef Q_PROCESSOR_X86_64
    if(ui->lineEditReg64->isFocused())          g_nCurrentValue=ui->lineEditReg64->getValue();
#endif

    if(ui->lineEditReg32_1->isFocused())        g_nCurrentValue=XBinary::setDwordToQword(g_nCurrentValue,ui->lineEditReg32_1->getValue(),1);
    if(ui->lineEditReg32_2->isFocused())        g_nCurrentValue=XBinary::setDwordToQword(g_nCurrentValue,ui->lineEditReg32_2->getValue(),0);

    if(ui->lineEditReg16_1->isFocused())        g_nCurrentValue=XBinary::setWordToQword(g_nCurrentValue,ui->lineEditReg16_1->getValue(),3);
    if(ui->lineEditReg16_2->isFocused())        g_nCurrentValue=XBinary::setWordToQword(g_nCurrentValue,ui->lineEditReg16_2->getValue(),2);
    if(ui->lineEditReg16_3->isFocused())        g_nCurrentValue=XBinary::setWordToQword(g_nCurrentValue,ui->lineEditReg16_3->getValue(),1);
    if(ui->lineEditReg16_4->isFocused())        g_nCurrentValue=XBinary::setWordToQword(g_nCurrentValue,ui->lineEditReg16_4->getValue(),3);

    if(ui->lineEditReg8_1->isFocused())         g_nCurrentValue=XBinary::setByteToQword(g_nCurrentValue,ui->lineEditReg8_1->getValue(),7);
    if(ui->lineEditReg8_2->isFocused())         g_nCurrentValue=XBinary::setByteToQword(g_nCurrentValue,ui->lineEditReg8_2->getValue(),6);
    if(ui->lineEditReg8_3->isFocused())         g_nCurrentValue=XBinary::setByteToQword(g_nCurrentValue,ui->lineEditReg8_3->getValue(),5);
    if(ui->lineEditReg8_4->isFocused())         g_nCurrentValue=XBinary::setByteToQword(g_nCurrentValue,ui->lineEditReg8_4->getValue(),4);
    if(ui->lineEditReg8_5->isFocused())         g_nCurrentValue=XBinary::setByteToQword(g_nCurrentValue,ui->lineEditReg8_5->getValue(),3);
    if(ui->lineEditReg8_6->isFocused())         g_nCurrentValue=XBinary::setByteToQword(g_nCurrentValue,ui->lineEditReg8_6->getValue(),2);
    if(ui->lineEditReg8_7->isFocused())         g_nCurrentValue=XBinary::setByteToQword(g_nCurrentValue,ui->lineEditReg8_7->getValue(),1);
    if(ui->lineEditReg8_8->isFocused())         g_nCurrentValue=XBinary::setByteToQword(g_nCurrentValue,ui->lineEditReg8_8->getValue(),0);

    reload();
}
