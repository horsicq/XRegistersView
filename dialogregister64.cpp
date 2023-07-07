/* Copyright (c) 2021-2023 hors<horsicq@gmail.com>
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
#include "dialogregister64.h"

#include "ui_dialogregister64.h"

DialogRegister64::DialogRegister64(QWidget *pParent) : DialogRegisterAbstract(pParent), ui(new Ui::DialogRegister64)
{
    ui->setupUi(this);

    // TODO DEC <-> HEX
    // TODO Show Names Show Bits

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
}

DialogRegister64::~DialogRegister64()
{
    delete ui;
}

void DialogRegister64::on_pushButtonOK_clicked()
{
    if (getXInfoDB()->setCurrentReg(getReg(), getCurrentVariant())) {
        getXInfoDB()->setCurrentRegCache(getReg(), getCurrentVariant());

        accept();
    }
}

void DialogRegister64::on_pushButtonCancel_clicked()
{
    reject();
}

void DialogRegister64::initValue()
{
    setInitVariant(getXInfoDB()->getCurrentRegCache(getReg()));
    setCurrentVariant(getInitVariant());

    setTitle(ui->groupBoxReg64, getReg());
    setTitle(ui->groupBoxReg32_2, XInfoDB::getSubReg32(getReg()));
    setTitle(ui->groupBoxReg16_4, XInfoDB::getSubReg16(getReg()));
    setTitle(ui->groupBoxReg8_7, XInfoDB::getSubReg8H(getReg()));
    setTitle(ui->groupBoxReg8_8, XInfoDB::getSubReg8L(getReg()));

    connect(ui->lineEditReg64, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));

    connect(ui->lineEditReg64, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));
    connect(ui->lineEditReg32_1, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));
    connect(ui->lineEditReg32_2, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));
    connect(ui->lineEditReg16_1, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));
    connect(ui->lineEditReg16_2, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));
    connect(ui->lineEditReg16_3, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));
    connect(ui->lineEditReg16_4, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));
    connect(ui->lineEditReg8_1, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));
    connect(ui->lineEditReg8_2, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));
    connect(ui->lineEditReg8_3, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));
    connect(ui->lineEditReg8_4, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));
    connect(ui->lineEditReg8_5, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));
    connect(ui->lineEditReg8_6, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));
    connect(ui->lineEditReg8_7, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));
    connect(ui->lineEditReg8_8, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));
}

void DialogRegister64::adjustValue()
{
    quint64 nCurrentValue = getCurrentVariant().var.v_uint64;

    if (!(ui->lineEditReg64->isFocused())) ui->lineEditReg64->setValue(nCurrentValue);

    if (!(ui->lineEditReg32_1->isFocused())) ui->lineEditReg32_1->setValue(XBinary::getDwordFromQword(nCurrentValue, 1));
    if (!(ui->lineEditReg32_2->isFocused())) ui->lineEditReg32_2->setValue(XBinary::getDwordFromQword(nCurrentValue, 0));

    if (!(ui->lineEditReg16_1->isFocused())) ui->lineEditReg16_1->setValue(XBinary::getWordFromQword(nCurrentValue, 3));
    if (!(ui->lineEditReg16_2->isFocused())) ui->lineEditReg16_2->setValue(XBinary::getWordFromQword(nCurrentValue, 2));
    if (!(ui->lineEditReg16_3->isFocused())) ui->lineEditReg16_3->setValue(XBinary::getWordFromQword(nCurrentValue, 1));
    if (!(ui->lineEditReg16_4->isFocused())) ui->lineEditReg16_4->setValue(XBinary::getWordFromQword(nCurrentValue, 0));

    if (!(ui->lineEditReg8_1->isFocused())) ui->lineEditReg8_1->setValue(XBinary::getByteFromQword(nCurrentValue, 7));
    if (!(ui->lineEditReg8_2->isFocused())) ui->lineEditReg8_2->setValue(XBinary::getByteFromQword(nCurrentValue, 6));
    if (!(ui->lineEditReg8_3->isFocused())) ui->lineEditReg8_3->setValue(XBinary::getByteFromQword(nCurrentValue, 5));
    if (!(ui->lineEditReg8_4->isFocused())) ui->lineEditReg8_4->setValue(XBinary::getByteFromQword(nCurrentValue, 4));
    if (!(ui->lineEditReg8_5->isFocused())) ui->lineEditReg8_5->setValue(XBinary::getByteFromQword(nCurrentValue, 3));
    if (!(ui->lineEditReg8_6->isFocused())) ui->lineEditReg8_6->setValue(XBinary::getByteFromQword(nCurrentValue, 2));
    if (!(ui->lineEditReg8_7->isFocused())) ui->lineEditReg8_7->setValue(XBinary::getByteFromQword(nCurrentValue, 1));
    if (!(ui->lineEditReg8_8->isFocused())) ui->lineEditReg8_8->setValue(XBinary::getByteFromQword(nCurrentValue, 0));

    ui->pushButtonOK->setEnabled(!XBinary::isXVariantEqual(getInitVariant(), getCurrentVariant()));
}

void DialogRegister64::setValue()
{
    quint64 nCurrentValue = getCurrentVariant().var.v_uint64;

    if (ui->lineEditReg64->isFocused()) nCurrentValue = ui->lineEditReg64->getValue();

    if (ui->lineEditReg32_1->isFocused()) nCurrentValue = XBinary::setDwordToQword(nCurrentValue, ui->lineEditReg32_1->getValue(), 1);
    if (ui->lineEditReg32_2->isFocused()) nCurrentValue = XBinary::setDwordToQword(nCurrentValue, ui->lineEditReg32_2->getValue(), 0);

    if (ui->lineEditReg16_1->isFocused()) nCurrentValue = XBinary::setWordToQword(nCurrentValue, ui->lineEditReg16_1->getValue(), 3);
    if (ui->lineEditReg16_2->isFocused()) nCurrentValue = XBinary::setWordToQword(nCurrentValue, ui->lineEditReg16_2->getValue(), 2);
    if (ui->lineEditReg16_3->isFocused()) nCurrentValue = XBinary::setWordToQword(nCurrentValue, ui->lineEditReg16_3->getValue(), 1);
    if (ui->lineEditReg16_4->isFocused()) nCurrentValue = XBinary::setWordToQword(nCurrentValue, ui->lineEditReg16_4->getValue(), 3);

    if (ui->lineEditReg8_1->isFocused()) nCurrentValue = XBinary::setByteToQword(nCurrentValue, ui->lineEditReg8_1->getValue(), 7);
    if (ui->lineEditReg8_2->isFocused()) nCurrentValue = XBinary::setByteToQword(nCurrentValue, ui->lineEditReg8_2->getValue(), 6);
    if (ui->lineEditReg8_3->isFocused()) nCurrentValue = XBinary::setByteToQword(nCurrentValue, ui->lineEditReg8_3->getValue(), 5);
    if (ui->lineEditReg8_4->isFocused()) nCurrentValue = XBinary::setByteToQword(nCurrentValue, ui->lineEditReg8_4->getValue(), 4);
    if (ui->lineEditReg8_5->isFocused()) nCurrentValue = XBinary::setByteToQword(nCurrentValue, ui->lineEditReg8_5->getValue(), 3);
    if (ui->lineEditReg8_6->isFocused()) nCurrentValue = XBinary::setByteToQword(nCurrentValue, ui->lineEditReg8_6->getValue(), 2);
    if (ui->lineEditReg8_7->isFocused()) nCurrentValue = XBinary::setByteToQword(nCurrentValue, ui->lineEditReg8_7->getValue(), 1);
    if (ui->lineEditReg8_8->isFocused()) nCurrentValue = XBinary::setByteToQword(nCurrentValue, ui->lineEditReg8_8->getValue(), 0);

    setCurrentVariant(XBinary::getXVariant(nCurrentValue));
}
