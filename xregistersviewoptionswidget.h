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
#ifndef XREGISTERSVIEWOPTIONSWIDGET_H
#define XREGISTERSVIEWOPTIONSWIDGET_H

#include <QFontDialog>

#include "xoptions.h"

namespace Ui {
class XRegistersViewOptionsWidget;
}

class XRegistersViewOptionsWidget : public QWidget {
    Q_OBJECT

public:
    explicit XRegistersViewOptionsWidget(QWidget *pParent = nullptr);
    ~XRegistersViewOptionsWidget();

    void setOptions(XOptions *pOptions);

    static void setDefaultValues(XOptions *pOptions);

public slots:
    void save();
    void reload();

private slots:
    void on_toolButtonRegsFont_clicked();

private:
    Ui::XRegistersViewOptionsWidget *ui;
    XOptions *g_pOptions;
};

#endif  // XREGISTERSVIEWOPTIONSWIDGET_H
