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
#ifndef DIALOGREGISTERGENERAL_H
#define DIALOGREGISTERGENERAL_H

#include <QDialog>
#include <QGroupBox>
#include "xinfodb.h"

namespace Ui {
class DialogRegisterGeneral;
}
// https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/x64-architecture
class DialogRegisterGeneral : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRegisterGeneral(QWidget *pParent=nullptr);
    ~DialogRegisterGeneral();

    void setData(XInfoDB *pXInfoDB,XInfoDB::XREG reg);
    void reload();

private slots:
    void on_pushButtonOK_clicked();
    void on_pushButtonCancel_clicked();
    void setTitle(QGroupBox *pGroupBox,XInfoDB::XREG reg);
    void on_lineEditReg64_textEdited(const QString &sString);
    void on_lineEditReg32_1_textEdited(const QString &sString);
    void on_lineEditReg32_2_textEdited(const QString &sString);
    void on_lineEditReg16_1_textEdited(const QString &sString);
    void on_lineEditReg16_2_textEdited(const QString &sString);
    void on_lineEditReg16_3_textEdited(const QString &sString);
    void on_lineEditReg16_4_textEdited(const QString &sString);
    void on_lineEditReg8_1_textEdited(const QString &sString);
    void on_lineEditReg8_2_textEdited(const QString &sString);
    void on_lineEditReg8_3_textEdited(const QString &sString);
    void on_lineEditReg8_4_textEdited(const QString &sString);
    void on_lineEditReg8_5_textEdited(const QString &sString);
    void on_lineEditReg8_6_textEdited(const QString &sString);
    void on_lineEditReg8_7_textEdited(const QString &sString);
    void on_lineEditReg8_8_textEdited(const QString &sString);

private:
    Ui::DialogRegisterGeneral *ui;
    XInfoDB *g_pXInfoDB;
    XInfoDB::XREG g_reg;
    quint64 g_nInitValue;
    quint64 g_nCurrentValue;
};

#endif // DIALOGREGISTERGENERAL_H
