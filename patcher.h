/*
 *  MHP3rd patcher PC
 *
 *  Copyright (C) 2010  Codestation
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PATCHER_H
#define PATCHER_H

#include <QMainWindow>
#include <QFile>

#define DATABIN_OFFSET 0x6D50000
#define PATCH_FILE "MHP3RD_DATA.BIN"
#define PATCH_RESTOREFILE "MHP3RD_RESTORE.BIN"

namespace Ui {
    class Patcher;
}

class Patcher : public QMainWindow
{
    Q_OBJECT

public:
    explicit Patcher(QWidget *parent = 0);
    ~Patcher();

private:
    Ui::Patcher *ui;

    QString filename;
    QString patchname;
    static const char signature[];

    quint32 *patch_offset;
    quint32 *patch_size;
    quint32 patch_count;
    qint64 data_start;

    void write_file(QFile *file, QString patchfile, int mode);
    void fill_tables(QFile *patchfile);

private slots:
    void on_iso_search_clicked();
    void on_iso_patch_clicked();
    void on_actionAcerca_de_Qt_triggered();
    void on_actionBuscar_triggered();
    void on_actionParchear_triggered();
    void on_actionAcerca_de_triggered();
};

#endif // PATCHER_H
