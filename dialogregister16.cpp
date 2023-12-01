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
#include "dialogregister16.h"
#include "ui_dialogregister16.h"

DialogRegister16::DialogRegister16(QWidget *pParent) : DialogRegisterAbstract(pParent), ui(new Ui::DialogRegister16)
{
    ui->setupUi(this);

    // TODO DEC <-> HEX
    // TODO Show Names Show Bits

    ui->pushButtonOK->setEnabled(false);

    ui->groupBoxReg16->setTitle("15-0");
    ui->groupBoxReg8_1->setTitle("15-8");
    ui->groupBoxReg8_2->setTitle("7-0");
}

DialogRegister16::~DialogRegister16()
{
    delete ui;
}

void DialogRegister16::on_pushButtonOK_clicked()
{
    if (getXInfoDB()->setCurrentReg(getReg(), getCurrentVariant())) {
        getXInfoDB()->setCurrentRegCache(getReg(), getCurrentVariant());

        accept();
    }
}

void DialogRegister16::on_pushButtonCancel_clicked()
{
    reject();
}

void DialogRegister16::initValue()
{
    setInitVariant(getXInfoDB()->getCurrentRegCache(getReg()));
    setCurrentVariant(getInitVariant());

    setTitle(ui->groupBoxReg16, getReg());
    setTitle(ui->groupBoxReg8_1, XInfoDB::getSubReg8H(getReg()));
    setTitle(ui->groupBoxReg8_2, XInfoDB::getSubReg8L(getReg()));

    connect(ui->lineEditReg16, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));

    connect(ui->lineEditReg16, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));
    connect(ui->lineEditReg8_1, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));
    connect(ui->lineEditReg8_2, SIGNAL(textEdited(QString)), this, SLOT(_on_line_textEdited(QString)));
}

void DialogRegister16::adjustValue()
{
    quint32 nCurrentValue = getCurrentVariant().var.v_uint32;

    if (!(ui->lineEditReg16->isFocused())) ui->lineEditReg16->setValue_uint32(nCurrentValue);

    if (!(ui->lineEditReg8_1->isFocused())) ui->lineEditReg8_1->setValue_uint8(XBinary::getByteFromWord(nCurrentValue, 1));
    if (!(ui->lineEditReg8_2->isFocused())) ui->lineEditReg8_2->setValue_uint8(XBinary::getByteFromWord(nCurrentValue, 0));

    ui->pushButtonOK->setEnabled(!XBinary::isXVariantEqual(getInitVariant(), getCurrentVariant()));
}

void DialogRegister16::setValue()
{
    quint32 nCurrentValue = getCurrentVariant().var.v_uint32;

    if (ui->lineEditReg16->isFocused()) nCurrentValue = ui->lineEditReg16->_getValue().toULongLong();

    if (ui->lineEditReg8_1->isFocused()) nCurrentValue = XBinary::setByteToWord(nCurrentValue, ui->lineEditReg8_1->_getValue().toULongLong(), 1);
    if (ui->lineEditReg8_2->isFocused()) nCurrentValue = XBinary::setByteToWord(nCurrentValue, ui->lineEditReg8_2->_getValue().toULongLong(), 0);

    setCurrentVariant(XBinary::getXVariant(nCurrentValue));
}
