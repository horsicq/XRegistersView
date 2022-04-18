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
    g_nInitValue=pXInfoDB->getCurrentReg(reg).var.v_uint32;
    setTitle(ui->groupBoxReg32_2,reg);
    setTitle(ui->groupBoxReg16_4,XInfoDB::getSubReg16(reg));
    setTitle(ui->groupBoxReg8_7,XInfoDB::getSubReg8H(reg));
    setTitle(ui->groupBoxReg8_8,XInfoDB::getSubReg8L(reg));
#endif
#ifdef Q_PROCESSOR_X86_64
    g_nInitValue=pXInfoDB->getCurrentReg(reg).var.v_uint64;
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
    ui->lineEditReg64->setValue(g_nCurrentValue);
#endif

    ui->lineEditReg32_1->setValue(XBinary::getDwordFromQword(g_nCurrentValue,1));
    ui->lineEditReg32_2->setValue(XBinary::getDwordFromQword(g_nCurrentValue,0));

    ui->lineEditReg16_1->setValue(XBinary::getWordFromQword(g_nCurrentValue,3));
    ui->lineEditReg16_2->setValue(XBinary::getWordFromQword(g_nCurrentValue,2));
    ui->lineEditReg16_3->setValue(XBinary::getWordFromQword(g_nCurrentValue,1));
    ui->lineEditReg16_4->setValue(XBinary::getWordFromQword(g_nCurrentValue,0));

    ui->pushButtonOK->setEnabled(g_nInitValue!=g_nCurrentValue);
}

void DialogRegisterGeneral::on_pushButtonOK_clicked()
{
    // TODO Save register
    accept();
}

void DialogRegisterGeneral::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogRegisterGeneral::setTitle(QGroupBox *pGroupBox, XInfoDB::XREG reg)
{
    QString sTitle=XInfoDB::regIdToString(reg);

    if(sTitle!="")
    {
        pGroupBox->setTitle(sTitle);
    }
}
