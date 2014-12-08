#ifndef TRACKADD_H
#define TRACKADD_H

#include <QDialog>
#include <QList>
#include <QDebug>

#include "yondercore.h"

namespace Ui {
class TrackAdd;
}

class TrackAdd : public QDialog
{
    Q_OBJECT

public:
    explicit TrackAdd(int sfx_bit_id, YonderCore *core, QWidget *parent = 0);
    ~TrackAdd();

private:
    Ui::TrackAdd *ui;

    YonderCore *core;
    int sfx_bit_id;

private slots:
    void sfxBitAddTracks();
};

#endif // TRACKADD_H
