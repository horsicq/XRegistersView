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
#include "dialogregister32.h"
#include "ui_dialogregister32.h"

DialogRegister32::DialogRegister32(QWidget *pParent) : DialogRegisterAbstract(pParent), ui(new Ui::DialogRegister32)
{
    ui->setupUi(this);

    // TODO DEC <-> HEX
    // TODO Show Names Show Bits

    ui->pushButtonOK->setEnabled(false);

    ui->groupBoxReg32->setTitle("31-0");
    ui->groupBoxReg16_1->setTitle("31-16");
    ui->groupBoxReg16_2->setTitle("15-0");
    ui->groupBoxReg8_1->setTitle("31-24");
    ui->groupBoxReg8_2->setTitle("23-16");
    ui->groupBoxReg8_3->setTitle("15-8");
    ui->groupBoxReg8_4->setTitle("7-0");
}

DialogRegister32::~DialogRegister32()
{
    delete ui;
}

void DialogRegister32::on_pushButtonOK_clicked()
{
    if (getXInfoDB()->setCurrentReg(getReg(), getCurrentVariant())) {
        getXInfoDB()->setCurrentRegCache(getReg(), getCurrentVariant());

        accept();
    }
}

void DialogRegister32::on_pushButtonCancel_clicked()
{
    reject();
}

void DialogRegister32::initValue()
{
    setInitVariant(getXInfoDB()->getCurrentRegCache(getReg()));
    setCurrentVariant(getInitVariant());

    setTitle(ui->groupBoxReg32, getReg());
    setTitle(ui->groupBoxReg16_2, XInfoDB::getSubReg16(getReg()));
    setTitle(ui->groupBoxReg8_3, XInfoDB::getSubReg8H(getReg()));
    setTitle(ui->groupBoxReg8_4, XInfoDB::getSubReg8L(getReg()));

    connect(ui->lineEditReg32, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));

    connect(ui->lineEditReg16_1, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));
    connect(ui->lineEditReg16_2, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));
    connect(ui->lineEditReg8_1, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));
    connect(ui->lineEditReg8_2, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));
    connect(ui->lineEditReg8_3, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));
    connect(ui->lineEditReg8_4, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));
}

void DialogRegister32::adjustValue()
{
    quint32 nCurrentValue = getCurrentVariant().var.v_uint32;

    if (!(ui->lineEditReg32->isFocused())) ui->lineEditReg32->setValue(nCurrentValue);

    if (!(ui->lineEditReg16_1->isFocused())) ui->lineEditReg16_1->setValue(XBinary::getWordFromDword(nCurrentValue, 1));
    if (!(ui->lineEditReg16_2->isFocused())) ui->lineEditReg16_2->setValue(XBinary::getWordFromDword(nCurrentValue, 0));

    if (!(ui->lineEditReg8_1->isFocused())) ui->lineEditReg8_1->setValue(XBinary::getByteFromDword(nCurrentValue, 3));
    if (!(ui->lineEditReg8_2->isFocused())) ui->lineEditReg8_2->setValue(XBinary::getByteFromDword(nCurrentValue, 2));
    if (!(ui->lineEditReg8_3->isFocused())) ui->lineEditReg8_3->setValue(XBinary::getByteFromDword(nCurrentValue, 1));
    if (!(ui->lineEditReg8_4->isFocused())) ui->lineEditReg8_4->setValue(XBinary::getByteFromDword(nCurrentValue, 0));

    ui->pushButtonOK->setEnabled(!XBinary::isXVariantEqual(getInitVariant(), getCurrentVariant()));
}

void DialogRegister32::setValue()
{
    quint32 nCurrentValue = getCurrentVariant().var.v_uint32;

    if (ui->lineEditReg32->isFocused()) nCurrentValue = ui->lineEditReg32->getIntValue();

    if (ui->lineEditReg16_1->isFocused()) nCurrentValue = XBinary::setWordToDword(nCurrentValue, ui->lineEditReg16_1->getIntValue(), 1);
    if (ui->lineEditReg16_2->isFocused()) nCurrentValue = XBinary::setWordToDword(nCurrentValue, ui->lineEditReg16_2->getIntValue(), 0);

    if (ui->lineEditReg8_1->isFocused()) nCurrentValue = XBinary::setByteToDword(nCurrentValue, ui->lineEditReg8_1->getIntValue(), 3);
    if (ui->lineEditReg8_2->isFocused()) nCurrentValue = XBinary::setByteToDword(nCurrentValue, ui->lineEditReg8_2->getIntValue(), 2);
    if (ui->lineEditReg8_3->isFocused()) nCurrentValue = XBinary::setByteToDword(nCurrentValue, ui->lineEditReg8_3->getIntValue(), 1);
    if (ui->lineEditReg8_4->isFocused()) nCurrentValue = XBinary::setByteToDword(nCurrentValue, ui->lineEditReg8_4->getIntValue(), 0);

    setCurrentVariant(XBinary::getXVariant(nCurrentValue));
}
