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
#ifndef DIALOGREGISTER64_H
#define DIALOGREGISTER64_H

#include "dialogregisterabstract.h"

namespace Ui {
class DialogRegister64;
}
// https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/x64-architecture
class DialogRegister64 : public DialogRegisterAbstract {
    Q_OBJECT

public:
    explicit DialogRegister64(QWidget *pParent = nullptr);
    ~DialogRegister64();

private slots:
    void on_pushButtonOK_clicked();
    void on_pushButtonCancel_clicked();

protected:
    virtual void initValue();
    virtual void adjustValue();
    virtual void setValue();

private:
    Ui::DialogRegister64 *ui;
};

#endif  // DIALOGREGISTER64_H
