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

#include <QFileDialog>
#include <QFile>
#include <stdio.h>
#include <string.h>
#include "patcher.h"
#include "ui_patcher.h"
#include "about.h"

const uint8_t Patcher::signature[] = {0xD6, 0xE3, 0x69, 0xA0,
                                   0x53, 0x0E, 0xE5, 0x23,
                                   0x45, 0xB1, 0xA4, 0xCC,
                                   0xC6, 0x79, 0x8E, 0xEC};

Patcher::Patcher(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Patcher)
{
    ui->setupUi(this);
    ui->patch_list->addItem(PATCH_FILE);
    ui->patch_list->addItem(PATCH_RESTOREFILE);
    ui->patch_list->setCurrentIndex(0);
    connect(
        ui->msg_list->model(),
        SIGNAL(rowsInserted ( const QModelIndex &, int, int ) ),
        ui->msg_list,
        SLOT(scrollToBottom ())
    );
}

void Patcher::on_iso_search_clicked() {
    QString file = QFileDialog::getOpenFileName(this,
         tr("Abrir ISO"), NULL, tr("Imagen iso (*.iso)"));
    if(!file.isEmpty()) {
        ui->iso_path->setText(file);
        filename = file;
    }
}

void Patcher::fill_tables(QFile *patchfile) {
    patchfile->seek(0);
    patchfile->read(reinterpret_cast<char *>(&patch_count), 4);
    patch_offset = new quint32[patch_count];
    patch_size = new quint32[patch_count];
    memset(patch_offset, 0, sizeof(quint32) * patch_count);
    memset(patch_size, 0, sizeof(quint32) * patch_count);
    for(unsigned int i = 0; i < patch_count; i++) {
        patchfile->read(reinterpret_cast<char *>(&patch_offset[i]), 4);
        patchfile->read(reinterpret_cast<char *>(&patch_size[i]), 4);
    }
    data_start = ((patch_count + 1) * 8);
    if(data_start % 16 > 0)
        data_start += 16 - (data_start % 16);
}

void Patcher::write_file(QFile *iso, QString patchfile, int mode) {
    char buffer[1024];
    QString message;
    QFile patch(patchfile);
    if(patch.open(QIODevice::ReadOnly)) {
        fill_tables(&patch);
        patch.seek(data_start);
        for(unsigned int i = 0;i < patch_count;i++) {
            quint32 patch_len = patch_size[i];
            iso->seek(DATABIN_OFFSET + patch_offset[i]);
            for(unsigned int j = 0; j < patch_len; j+= 1024) {
                patch.read(buffer, 1024);
                iso->write(buffer, 1024);
            }
            if(mode == 0)
                message = QString(tr("Aplicando parche (%1/%2)...OK")).arg(i+1).arg(patch_count);
            else
                message = QString(tr("Quitando parche (%1/%2)...OK")).arg(i+1).arg(patch_count);
            ui->msg_list->addItem(message);
        }
        patch.close();
        delete []patch_offset;
        delete []patch_size;
        ui->msg_list->addItem(tr("Parcheo completado"));
    } else {
        ui->msg_list->addItem(tr("Archivo de parcheo no encontrado"));
    }
}

void Patcher::on_iso_patch_clicked() {
    ui->msg_list->clear();
    if(!filename.isEmpty()) {
        QFile file(filename);
        if(file.open(QIODevice::ReadWrite)) {
            file.seek(DATABIN_OFFSET);
            char buffer[16];
            if(file.read(buffer, sizeof(buffer)) == 16) {
                if(!memcmp(buffer, signature, 16)) {
                    ui->msg_list->addItem(tr("data.bin encontrado, comenzando parcheo"));
                    write_file(&file, ui->patch_list->currentText(), ui->patch_list->currentIndex());
                } else {
                    ui->msg_list->addItem(tr("data.bin no encontrado dentro del ISO"));
                }
            } else {
                ui->msg_list->addItem(tr("No se pudo leer correctamente la ISO"));
            }
        } else {
            ui->msg_list->addItem(tr("No se pudo abrir la ISO"));
        }
    } else {
        ui->msg_list->addItem(tr("Debe seleccionar una ISO"));
    }
}

void Patcher::on_actionAcerca_de_Qt_triggered() {
    QApplication::aboutQt();
}

void Patcher::on_actionBuscar_triggered() {
    on_iso_search_clicked();
}

void Patcher::on_actionParchear_triggered() {
    on_iso_patch_clicked();
}

void Patcher::on_actionAcerca_de_triggered() {
    About w;
    w.exec();
}

Patcher::~Patcher() {
    delete ui;
}


